//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include <Csas_Engine/Renderer/RenderPipeline/RenderData/RenderData.h>
#include "PassNode.h"
#include "Csas_Engine/Renderer/Framebuffer.h"
namespace CsasEngine
{

    struct Material_BaseBRDF;
    struct MeshComponent;

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


            PassNode* passNode_ptr;
            BRDFPassNode::BRDFPassData dataVec;
            SkyboxPassNode::SkyboxPassData skyboxdata;
        };
        using RenderPassMap=std::unordered_map<RenderIndex,RenderWrap>;

        using MeshVector=std::vector<MeshComponent*>;
        using MaterialVector=std::vector<Material_BaseBRDF*>;


        ForwardPass()=default;
        void SetConstData(const Ref<Framebuffer>& render_Target,
                          const CameraPtr& m_camera,
                          const SpotLightPtrVec& m_spots);

        ~ForwardPass()override;
        void AddPass(RenderIndex index,PassNodeType);
        void PrepareRenderer()override;
        void ExecuteRenderer()override;
        void SubmitRenderer (const RenderDataVec&data,RenderIndex index)override;
        //light info? ok

        static glm::mat4 &get_CameraView();
    private:
        BRDFPassNode brdfNode;
        bool has_skybox=false;
        //const
        Ref<UniformBuffer> CameraUBO;
        CameraPtr m_camera;
        SpotLightPtrVec m_spots;
        Ref<Framebuffer> render_Target;
        RenderPassMap renderMap;
        Ref<Framebuffer> post_processing= nullptr;
    };

}