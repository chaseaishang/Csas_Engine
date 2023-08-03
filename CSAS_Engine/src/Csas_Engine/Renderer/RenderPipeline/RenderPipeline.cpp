//
// Created by chaseaishang on 23-8-3.
//

#include "RenderPipeline.h"
#include "Csas_Engine/Renderer/Framebuffer.h"
namespace CsasEngine
{

    CameraPtr RenderPipeline::m_camera;
    SpotLightPtrVec RenderPipeline::m_spots;
    RenderPipeline*RenderPipeline::m_Instance=new RenderPipeline();
    static RenderIndex g_nowIndex;

    void RenderPipeline::OnPrepare()
    {

    }

    void RenderPipeline::OnExecute()
    {

    }
    //creat pass node
    void RenderPipeline::Submit(RenderData data,RenderIndex nowIndex)
    {
        g_nowIndex=nowIndex;
        auto& vec=map[g_nowIndex];
        auto& passvec=renderPassMap[g_nowIndex];
        for(auto type:vec)
        {
            switch (type)
            {
                case RenderPassType::ForwardPass:
                {
                    // using MeshVector=std::vector<MeshComponent>;
                    //        using MaterialVector=std::vector<Material_BaseBRDF>;

                    std::vector<Material_BaseBRDF*> vec;
                    for(auto&ptr:data.materialPtrVec)
                    {
                        vec.push_back((Material_BaseBRDF*)ptr);
                    }

                    auto ptr=new ForwardPass(data.meshPtrVec,
                                             vec,render_Target,m_camera,m_spots
                                             );
                    ptr->PrepareRenderer();
                    passvec.push_back(ptr);
                    break;
                }

            }
        }

    }
    //camera
    void RenderPipeline::BeginPipeline(CameraPtr camera,SpotLightPtrVec spots)
    {
        m_camera=camera;
        m_spots=spots;

    }

    // render
    // del pass node
    void RenderPipeline::EndPipeline()
    {
        //map begin
        auto& passvec=renderPassMap[0];
        for(auto &pass:passvec)
        {
            pass->ExecuteRenderer();
            delete pass;
        }
        renderPassMap=RenderPassMap();
    }




    RenderIndex RenderPipeline::BeginPass()
    {

        return renderIndex;
    }

    void RenderPipeline::SubmitPass(RenderPassType renderPass_type)
    {
        auto& vec=map[renderIndex];
        vec.push_back(renderPass_type);
    }

    void RenderPipeline::EndPass()
    {
        renderIndex++;
    }
}