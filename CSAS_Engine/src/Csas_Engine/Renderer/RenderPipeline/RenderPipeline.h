//
// Created by chaseaishang on 23-8-3.
//

#pragma once
#include "RenderPass.h"
#include "RenderData/RenderData.h"
namespace CsasEngine
{
    class Framebuffer;
    typedef  uint8_t RenderIndex;
    class RenderPipeline
    {
        using RenderPassTypeVec=std::vector<RenderPassType>;
        using RenderPassVec    =std::vector<RenderPass*>;
        using RenderMap=std::unordered_map<RenderIndex,RenderPassTypeVec>;
        using RenderPassMap=std::unordered_map<RenderIndex,RenderPassVec>;
    public:
         void BeginPipeline(CameraPtr camera,SpotLightPtrVec spots );//camera
        //mesh light material

         void Submit(RenderData data,RenderIndex nowIndex);
         void EndPipeline();
         static RenderPipeline* getInstance(){return m_Instance;}
    public:
        RenderPipeline()=default;
        ~RenderPipeline()=default;
        //@TODO move to pipelineManager
        RenderIndex BeginPass();
        void SubmitPass(RenderPassType);
        void EndPass();
        RenderIndex renderIndex=0;

        RenderMap map;
        RenderPassMap renderPassMap;
        //end
        void SetRenderTarget(Ref<Framebuffer>&target){render_Target=target;}
        Ref<Framebuffer>&GetRenderTarget(){return render_Target;}
        void OnPrepare();
        void OnExecute();

        Ref<Framebuffer> render_Target;
        static CameraPtr m_camera;
        static SpotLightPtrVec m_spots;
    private:
        static RenderPipeline*m_Instance;
    };
}