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
    //temp

    Ref<CubeTexture>hdr_texture= nullptr;
    //Ref<Texture2D>hdr_texture= nullptr;
    void ForwardPass::PrepareRenderer()
    {



//hdr_texture=Texture2D::Create("./assets/textures/HDR/satara_night_2k.hdr",spec);


        auto Spec=render_Target->GetSpecification();
//        Spec.ColorAttachmentSize=1;
//        Spec.Has_Depth=false;
        if(post_processing==nullptr)
        {
            Spec.Hdr=true;
            post_processing=Framebuffer::Create(Spec);
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

        CameraUBO=UniformBuffer::Create(sizeof(GlobalCameraSpec::ViewProjMatrix),0);



    }

    void ForwardPass::ExecuteRenderer()
    {
        CameraUBO->SetData(GlobalCameraSpec::ViewProjMatrix,sizeof(GlobalCameraSpec::ViewProjMatrix));

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

        auto&renderWrap=renderMap[index];
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
                        .spots=m_spots,
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