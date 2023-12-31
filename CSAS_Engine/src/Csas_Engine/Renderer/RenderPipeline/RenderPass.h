//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include <Csas_Engine/Renderer/RenderPipeline/RenderData/RenderData.h>
#include "PassNode.h"
#include "Csas_Engine/Renderer/Framebuffer.h"
#include "ParticlePass/ParticlePassNode.h"
namespace CsasEngine
{

    struct Material_BaseBRDF;


    class RenderPass
    {
    public:
        RenderPass()=default;
        virtual ~RenderPass()=default;
        virtual void PrepareRenderer()=0;
        virtual void ExecuteRenderer()=0;
        virtual void SubmitRenderer(const RenderDataVec&data,RenderIndex index)=0;
//        virtual void FinishRenderer()=0;
    };
   //dispatch
   //all need resource
    class ForwardPass : public RenderPass
    {
    public:
        typedef  uint8_t RenderIndex;

        using PassNodeVec    =std::vector<PassNode*>;
        using DataVec=BRDFPassNode::BRDFPassData;

        struct RenderWrap
        {
        public:
            RenderWrap()=default;
            ~RenderWrap()=default;

            PassNode* passNode_ptr;

            BRDFPassNode::BRDFPassData dataVec;
            SkyboxPassNode::SkyboxPassData skyboxdata;
            BlurPassNode::BlurPassData     blurPassData;
            LightPassNode::LightPassData   lightPassData;


        };
        using RenderPassMap=std::unordered_map<RenderIndex,RenderWrap>;

        using MeshVector=std::vector<MeshComponent*>;
        using MaterialVector=std::vector<Material_BaseBRDF*>;


        ForwardPass()=default;
        void SetConstData(const Ref<Framebuffer>& render_Target,
                          const CameraPtr& m_camera,
                          const Light_Data&lightData,
                          const CubeTexture*irradiance_map,
                          const CubeTexture*prefiltered_map,
                          const Texture2D*BRDF_LUT
                          );

        ~ForwardPass()override;
        void AddPass(RenderIndex index,PassNodeType);
        void PrepareRenderer()override;
        void ExecuteRenderer()override;
        void SubmitRenderer (const RenderDataVec&data,RenderIndex index)override;
        //light info? ok

        static glm::mat4 &get_CameraView();

        static uint GetDepthDebug();//@TODO remove
    private:
        BRDFPassNode brdfNode;
        bool has_skybox=false;
        bool has_blur= false;
        //const
        Ref<UniformBuffer> CameraUBO;
        Ref<UniformBuffer> Spot_LightsUBO;
        Ref<UniformBuffer> Direct_LightsUBO;
        Ref<UniformBuffer> Render_InputUBO;
        CameraPtr m_camera;
        SpotLightPtrVec m_spots;
        Ref<Framebuffer> render_Target;
        RenderPassMap renderMap;
        Ref<Framebuffer> post_processing= nullptr;
        const CubeTexture*irradiance_map;
        const CubeTexture*prefiltered_map;
        const Texture2D*BRDF_LUT;

    private:
        bool Shadow_Enable=true;
        Ref<Framebuffer> DepthFBO= nullptr;
        ShadowPassNode ShadowPass;
        ShadowPassNode::ShadowPassData shadow_data;
        Ref<Shader>Shadow_shader;
        uint ViewPortWidth,ViewPortHeight;
        //particles
        ParticlePassNode::ParticlePassData particlePassData;

    };

}