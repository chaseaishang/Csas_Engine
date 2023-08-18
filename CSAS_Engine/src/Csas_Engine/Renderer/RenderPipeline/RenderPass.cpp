//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "RenderPass.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/RenderCommand.h"
#include "Csas_Engine/Renderer/UniformBuffer.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Texture.h"
namespace CsasEngine {
    namespace GlobalCameraSpec
    {
        glm::mat4 ViewProjMatrix[2];
    }
    namespace GlobalSpotLightsSpec
    {
        glm::vec4 color[4];
        glm::vec4 position[4];
        int size[4];//0 is use other is just ext
    }
    namespace GlobalRenderInput
    {
        uint size=48;
        uint offset=32;
        int depth_prepass=0;//bool has some problem
    }
    //temp

    Ref<CubeTexture>hdr_texture= nullptr;
    //Ref<Texture2D>hdr_texture= nullptr;
    void ForwardPass::PrepareRenderer()
    {



//hdr_texture=Texture2D::Create("./assets/textures/HDR/satara_night_2k.hdr",spec);


        auto Spec=render_Target->GetSpecification();
//        Spec.ColorAttachmentSize=1;
//        Spec.Has_Depth=false;
        ViewPortWidth=Spec.Width;
        ViewPortHeight=Spec.Height;
        if(post_processing==nullptr)
        {
            Spec.Hdr=true;
            post_processing=Framebuffer::Create(Spec);

            //DepthFBO


        }
        else
        {
            auto Width  =post_processing->GetSpecification().Height;
            auto Height =post_processing->GetSpecification().Width;
            auto NowWidth=Spec.Width;auto NowHeight=Spec.Height;
            if(NowWidth!=Width
            &&NowHeight!=Height)
            {
                post_processing->Resize(NowWidth,NowHeight);
            }
            // change sourceTexture

        }
        if(DepthFBO== nullptr)
        {
            Spec.Height=Spec.Width=1024;
            Spec.Has_Depth= true;
            Spec.ColorAttachmentSize=0;
            DepthFBO=Framebuffer::Create(Spec);
        }
        else
        {
            DepthFBO->Clear(-1);
        }
        CameraUBO=UniformBuffer::Create(sizeof(GlobalCameraSpec::ViewProjMatrix),0);
        Spot_LightsUBO=UniformBuffer::Create(sizeof(glm::vec4)*8+sizeof(int)*4,1);
        Render_InputUBO=UniformBuffer::Create(GlobalRenderInput::size,10);

    }

    void ForwardPass::ExecuteRenderer()
    {
        CameraUBO->SetData(GlobalCameraSpec::ViewProjMatrix,sizeof(GlobalCameraSpec::ViewProjMatrix));
        Spot_LightsUBO->SetData(GlobalSpotLightsSpec::color,sizeof(GlobalSpotLightsSpec::color));
        Spot_LightsUBO->SetData(GlobalSpotLightsSpec::position,sizeof(GlobalSpotLightsSpec::position),sizeof(GlobalSpotLightsSpec::color));
        Spot_LightsUBO->SetData(&GlobalSpotLightsSpec::size,sizeof(int)*4,sizeof(GlobalSpotLightsSpec::color)*2);


        this->render_Target->Bind();
        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::DepthMask(false);
        int index=0;
        if(has_skybox)//@TODO temp need to remove
        {
            //
            RenderCommand::FaceCulling(true, false);
            auto &renderWrap=renderMap[0];
            auto&data=renderWrap.skyboxdata;
            static_cast<SkyboxPassNode*>(renderWrap.passNode_ptr)->OnExecute(&renderWrap.skyboxdata);
            index++;
            //
            RenderCommand::FaceCulling(false);
        }
        RenderCommand::DepthMask(true);
        if(Shadow_Enable)
        {
            DepthFBO->Bind();
            RenderCommand::SetViewport(0,0,1024,1024);
            int Depth_Prepass=true;
            Render_InputUBO->SetData(&Depth_Prepass,sizeof(int),GlobalRenderInput::offset);

            Depth_Prepass= false;
            RenderCommand::SetViewport(0,0,ViewPortWidth,ViewPortHeight);
            Render_InputUBO->SetData(&Depth_Prepass,sizeof(int),GlobalRenderInput::offset);
            render_Target->Bind();
        }
        auto&renderWrap=renderMap[index];//render
        renderWrap.passNode_ptr->OnExecute(&renderWrap.dataVec);
        index++;
        if(has_blur)
        {

            post_processing->Bind();
            auto&Wrap=renderMap[index];
            //
            Wrap.blurPassData.source_tex=render_Target->GetColorAttachment(0);
            Wrap.passNode_ptr->OnExecute(&Wrap.blurPassData);
            post_processing->Unbind();
            Framebuffer::TransferColor(*post_processing,0,*render_Target,0);
        }
        if(auto&Wrap=renderMap[index];true)//Light pass
        {
            Wrap.passNode_ptr->OnExecute(&Wrap.lightPassData);

            index++;
        }




        this->render_Target->Unbind();

    }


    ForwardPass::~ForwardPass()
    {
        for(auto &[render,vec]:renderMap)
        {
            auto&pass=vec.passNode_ptr;

            delete pass;

        }
    }

    void ForwardPass::SubmitRenderer(const RenderDataVec&data,RenderIndex index)
    {
        auto&wrap=renderMap[index];
        auto&type=wrap.passNode_ptr->type;
        switch (type)
        {
            case PassNodeType::Skybox:
            {
                has_skybox= true;
                auto&Sky_box_data=wrap.skyboxdata;
                RenderData data1;
                data1.meshPtr=data[0].meshPtr;
                data1.materialPtr=data[0].materialPtr;
                Sky_box_data.data=std::move(data1);
                break;
            }
            case PassNodeType::BrdfPass:
            {
                auto&datavec=wrap.dataVec;
                datavec={
                        .data_vec=std::move(data),
                        .irradiance_map= this->irradiance_map,
                        .prefiltered_map=this->prefiltered_map,
                        .BRDF_LUT=this->BRDF_LUT
                };

                break;
            }
            case PassNodeType::BlurPass:
            {
                has_blur= true;
                auto&blurdata=wrap.blurPassData;
                blurdata.blur_material= CreateRef<Material_Blur>();
                break;
            }
            case PassNodeType::LightPass:
            {
                auto&Lightdata=wrap.lightPassData;
                Lightdata.data_vec=std::move(data);
                break;
            }
            default:
            {
                CSAS_CORE_ASSERT(true,"Unknown RenderPass! ");
            }
        }



        //datavec=std::move(data);
    }

    void ForwardPass::SetConstData(const Ref<Framebuffer>& render_Target,
                                   const CameraPtr& m_camera,
                                   const SpotLightPtrVec& m_spots,
                                   const MeshVector &mesh,
                                   const CubeTexture*irradiance_map,
                                   const CubeTexture*prefiltered_map,
                                   const Texture2D*BRDF_LUT
                                   )
    {

        this->m_spots=m_spots;
        this->render_Target=render_Target;
        this->m_camera=m_camera;
        GlobalCameraSpec::ViewProjMatrix[0]=m_camera->GetView();
        GlobalCameraSpec::ViewProjMatrix[1]=m_camera->GetProjection();
        GlobalSpotLightsSpec::size[0]=m_spots.size();
        for(int i=0;i<GlobalSpotLightsSpec::size[0];i++)
        {
            GlobalSpotLightsSpec::color[i]=m_spots[i]->color*m_spots[i]->intensity;
            GlobalSpotLightsSpec::position[i]=
                    GlobalCameraSpec::ViewProjMatrix[0]*
                    glm::vec4{mesh[i]->transform.Translation,1.0};
        }

        this->irradiance_map=irradiance_map;
        this->prefiltered_map=prefiltered_map;
        this->BRDF_LUT=BRDF_LUT;

    }

    void ForwardPass::AddPass(ForwardPass::RenderIndex index, PassNodeType type)
    {
        auto&passnode=renderMap[index].passNode_ptr;
        switch (type)
        {
            case PassNodeType::BrdfPass:
            {

                passnode=new BRDFPassNode();
                break;
            }
            case PassNodeType::Skybox:
            {

                passnode=new SkyboxPassNode();
                break;
            }
            case PassNodeType::BlurPass:
            {
                passnode=new BlurPassNode();
                RenderDataVec vec;
                SubmitRenderer(vec,index);


                break;
            }
            case PassNodeType::LightPass:
            {
                passnode=new LightPassNode();
                break;
            }


        }
    }

    glm::mat4 &ForwardPass::get_CameraView()
    {
        return GlobalCameraSpec::ViewProjMatrix[0];
    }


}