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

    }

    void RenderPipeline::OnExecute()
    {

    }
    //using RenderSingleData=struct
    //    {
    //        MeshComponent*meshPtr;
    //        Material*materialPtr;
    //    };
    //    using RenderDataVec=std::vector<RenderSingleData>;
    //creat pass node
    //void Submit(RenderDataVec data,RenderIndex nowIndex);
    void RenderPipeline::Submit(RenderDataVec data,RenderIndex nowIndex)
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

                    auto ptr=new ForwardPass(data
                                             ,render_Target,m_camera,m_spots
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
        for(auto&[index,vec]:renderPassMap)
        {
            for(auto&pass:vec)
            {
                pass->ExecuteRenderer();
                delete pass;
            }
        }

        renderPassMap.clear();
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
    // add PassNode
    void RenderPipeline::EndPass()
    {
        renderIndex++;
    }
}