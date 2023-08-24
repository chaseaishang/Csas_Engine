//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include <glm/gtc/type_ptr.hpp>
#include "Csas_Engine/Core/Application.h"
#include "RenderPass.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/RenderCommand.h"
#include "Csas_Engine/Renderer/UniformBuffer.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Texture.h"
namespace CsasEngine {
    namespace DebugSpec
    {
        uint Depth= 0;
    }

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
    namespace GlobalDirectLightSpec
    {
        uint size=32;
        bool enable;
        uint offset0=0;
        uint offset1=16;
        glm::vec4 color;
        glm::vec4 direction;
        glm::mat4 lightSpaceMatrix;
    }
    namespace GlobalRenderInput
    {
        uint size=80;
        uint offset=64;
        glm::mat4 lightSpaceMatrix;
        float now_time;
        float delta_time;
        float ext0;
        float ext1;
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


        //@TODO temp
        static bool creat=true;
        if(creat)
        {
            CameraUBO=UniformBuffer::Create(sizeof(GlobalCameraSpec::ViewProjMatrix),0);
            Spot_LightsUBO=UniformBuffer::Create(sizeof(glm::vec4)*8+sizeof(int)*4,1);
            Direct_LightsUBO=UniformBuffer::Create(GlobalDirectLightSpec::size,2);
            Render_InputUBO=UniformBuffer::Create(GlobalRenderInput::size,10);
            Shadow_shader=Shader::Create("./assets/shaders/utils/shadow.glsl");
            creat= false;
        }

        RenderCommand::DepthTest(true);
        RenderCommand::SetPointSize(1.0f);
    }

    void ForwardPass::ExecuteRenderer()
    {
        CameraUBO->SetData(GlobalCameraSpec::ViewProjMatrix, sizeof(GlobalCameraSpec::ViewProjMatrix));

        Spot_LightsUBO->SetData(GlobalSpotLightsSpec::color, sizeof(GlobalSpotLightsSpec::color));
        Spot_LightsUBO->SetData(GlobalSpotLightsSpec::position, sizeof(GlobalSpotLightsSpec::position),
                                sizeof(GlobalSpotLightsSpec::color));
        Spot_LightsUBO->SetData(&GlobalSpotLightsSpec::size, sizeof(int) * 4, sizeof(GlobalSpotLightsSpec::color) * 2);

        auto time=Application::Get().GetTime();
        Render_InputUBO->SetData(&time,sizeof(float),GlobalRenderInput::offset);


        GlobalDirectLightSpec::color.w = GlobalDirectLightSpec::enable;
        Direct_LightsUBO->SetData(glm::value_ptr(GlobalDirectLightSpec::color),
                                  sizeof(glm::vec4), GlobalDirectLightSpec::offset0);
        Direct_LightsUBO->SetData(glm::value_ptr(GlobalDirectLightSpec::direction),
                                  sizeof(glm::vec4), GlobalDirectLightSpec::offset1);


        this->render_Target->Bind();
        RenderCommand::Clear();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::DepthMask(false);
        int index = 0;
        if (has_skybox)//@TODO temp need to remove
        {
            //
            RenderCommand::FaceCulling(true, false);
            auto &renderWrap = renderMap[0];
            auto &data = renderWrap.skyboxdata;
            static_cast<SkyboxPassNode *>(renderWrap.passNode_ptr)->OnExecute(&renderWrap.skyboxdata);
            index++;
            //
            RenderCommand::FaceCulling(false);
        }
        RenderCommand::DepthMask(true);
        if (Shadow_Enable) {


            DepthFBO->Bind();
            RenderCommand::DepthTest(true);
            DepthFBO->Clear(-2);
            //RenderCommand::Clear();
            RenderCommand::SetViewport(0, 0, 1024, 1024);

            auto &renderWrap = renderMap[index];//render
            shadow_data.data_vec = renderWrap.dataVec.data_vec;
            shadow_data.m_shader = Shadow_shader.get();
            shadow_data.light_SpaceMatrix = GlobalDirectLightSpec::lightSpaceMatrix;
            ShadowPass.OnExecute(&shadow_data);

            DebugSpec::Depth = DepthFBO->GetDepthRendererID();



            RenderCommand::SetViewport(0, 0, ViewPortWidth, ViewPortHeight);

            render_Target->Bind();
        }
        if (true) //Brdf
        {
            auto depth_tex=DepthFBO->GetDepthAttachment();
            depth_tex->Bind(16);
            Render_InputUBO->SetData(glm::value_ptr(GlobalRenderInput::lightSpaceMatrix)
            ,sizeof(glm::mat4));


            auto &renderWrap = renderMap[index];//render
            renderWrap.passNode_ptr->OnExecute(&renderWrap.dataVec);
            index++;
        }
        if(has_blur)
        {
            RenderCommand::DepthTest(false);
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
        if(auto&Wrap=renderMap[index];true)
        {
            RenderCommand::DepthTest(false);
            RenderCommand::SetPointSize(10.0f);
            RenderCommand::SetBlend(true,BlendFun::ONE_MINUS_SRC_ALPHA);
            Wrap.passNode_ptr->OnExecute(&this->particlePassData);
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
            case PassNodeType::ParticlePass:
            {
                auto&datavec=this->particlePassData;
                datavec.data=std::move(data);
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
                                   const Light_Data&lightData,
                                   const CubeTexture*irradiance_map,
                                   const CubeTexture*prefiltered_map,
                                   const Texture2D*BRDF_LUT
                                   )
    {

        this->m_spots=lightData.Spots;
        this->render_Target=render_Target;
        this->m_camera=m_camera;
        GlobalCameraSpec::ViewProjMatrix[0]=m_camera->GetView();
        GlobalCameraSpec::ViewProjMatrix[1]=m_camera->GetProjection();
        //SpotLight
        GlobalSpotLightsSpec::size[0]=m_spots.size();
        for(int i=0;i<GlobalSpotLightsSpec::size[0];i++)
        {
            GlobalSpotLightsSpec::color[i]=m_spots[i]->color*m_spots[i]->intensity;
            GlobalSpotLightsSpec::position[i]=
                    GlobalCameraSpec::ViewProjMatrix[0]*
                    glm::vec4{lightData.Spot_meshes[i]->transform.Translation,1.0};
        }
        //DirectLight
        if(lightData.Directs.size()>0)
        {
            auto&direct=lightData.Directs[0];
            GlobalDirectLightSpec::enable= true;
            GlobalDirectLightSpec::color=direct->color*direct->intensity;
            auto&trans=lightData.Direct_meshes[0]->transform;
            auto forward=trans.GetAndUpdateBasis(1);
            GlobalDirectLightSpec::direction=glm::vec4{-forward,0};



            if(Shadow_Enable)
            {
                float near_plane = 0.1f, far_plane = 7.0f;
                glm::mat4 lightProjection = glm::ortho(-6.0f, 6.0f, -6.0f, 6.0f, near_plane, far_plane);

                auto&position=trans.Translation;
                auto up=trans.GetAndUpdateBasis(0);
                glm::mat4 lightView = glm::lookAt(position,
                                                  position+forward,
                                                  up);
                GlobalDirectLightSpec::lightSpaceMatrix=lightProjection * lightView;

                GlobalRenderInput::lightSpaceMatrix=GlobalDirectLightSpec::lightSpaceMatrix;

            }


        }
        else
        {
            GlobalDirectLightSpec::enable= false;
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
            case PassNodeType::ParticlePass:
            {
                passnode=new ParticlePassNode();
                break;
            }


        }
    }

    glm::mat4 &ForwardPass::get_CameraView()
    {
        return GlobalCameraSpec::ViewProjMatrix[0];
    }

    uint ForwardPass::GetDepthDebug() {
        return DebugSpec::Depth;
    }


}