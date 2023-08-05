//
// Created by chaseaishang on 23-8-3.
//

#include "RenderPipeline.h"
#include "Csas_Engine/Renderer/Framebuffer.h"
#include "RenderData/RenderData.h"
namespace CsasEngine
{

    CameraPtr RenderPipeline::m_camera;
    SpotLightPtrVec RenderPipeline::m_spots;
    RenderPipeline*RenderPipeline::m_Instance=new RenderPipeline();
    static RenderIndex g_nowIndex;

    void RenderPipeline::OnPrepare()
    {
        forwardPass.PrepareRenderer();
    }

    void RenderPipeline::OnExecute()
    {
        forwardPass.ExecuteRenderer();
    }

    void RenderPipeline::Submit(RenderDataVec data,RenderIndex nowIndex)
    {
        forwardPass.SubmitRenderer(data,nowIndex);


    }
    //camera
    void RenderPipeline::BeginPipeline(CameraPtr camera,SpotLightPtrVec spots)
    {
        m_camera=camera;
        m_spots=spots;
        forwardPass.SetConstData(this->render_Target,camera,spots);

    }

    // render
    // del pass node
    void RenderPipeline::EndPipeline()
    {
        OnExecute();
    }




    RenderIndex RenderPipeline::BeginPass()
    {

        return renderIndex;
    }

    void RenderPipeline::SubmitPass(PassNodeType renderPass_type)
    {
        forwardPass.AddPass(renderIndex,renderPass_type);

    }
    // add PassNode
    void RenderPipeline::EndPass()
    {
        renderIndex++;
    }
}