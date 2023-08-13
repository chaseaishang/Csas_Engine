//
// Created by chaseaishang on 23-8-3.
//

#pragma once
#include "RenderPass.h"
#include "RenderData/RenderData.h"
namespace CsasEngine
{
    class Framebuffer;

    class RenderPipeline
    {

        using RenderPassVec    =std::vector<RenderPass*>;
        using RenderPassMap=std::unordered_map<RenderIndex,RenderPassVec>;

    public:
         void BeginPipeline(CameraPtr camera,SpotLightPtrVec spots,
                            CubeTexture*irradiance_map,
                            CubeTexture*prefiltered_map,
                            Texture2D*BRDF_LUT
         );//camera
        //mesh light material

         void Submit(RenderDataVec data,RenderIndex nowIndex);
         void EndPipeline();
         static RenderPipeline* getInstance(){return m_Instance;}
    public:
        RenderPipeline()=default;
        ~RenderPipeline()=default;
        //@TODO move to pipelineManager
        void BeginPass();
        RenderIndex SubmitPass(PassNodeType);
        void EndPass();
        RenderIndex renderIndex=0;

        ForwardPass forwardPass;
        //end
        void SetRenderTarget(Ref<Framebuffer>&target){render_Target=target;}
        Ref<Framebuffer>&GetRenderTarget(){return render_Target;}
        void OnPrepare();
        void OnExecute();
        bool Enable_blur= false;
        Ref<Framebuffer> render_Target;
        static CameraPtr m_camera;
        static SpotLightPtrVec m_spots;

    private:
        static RenderPipeline*m_Instance;

    };
}