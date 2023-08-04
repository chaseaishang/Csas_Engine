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
    enum class RenderPassType
    {
        ForwardPass
    };
    class RenderPass
    {
    public:
        RenderPass()=default;
        virtual ~RenderPass()=default;
        virtual void PrepareRenderer()=0;
        virtual void ExecuteRenderer()=0;
//        virtual void SubmitRenderer()=0;
//        virtual void FinishRenderer()=0;
    };
   //dispatch
   //all need resource
    class ForwardPass : public RenderPass
    {
    public:
        using MeshVector=std::vector<MeshComponent*>;
        using MaterialVector=std::vector<Material_BaseBRDF*>;
        using DataStruct=BRDFPassNode::BRDFPassData;

        ForwardPass(RenderDataVec data,Ref<Framebuffer> render_Target,
        CameraPtr m_camera,
        SpotLightPtrVec m_spots
        );
        ~ForwardPass()override;
        void PrepareRenderer()override;
        void ExecuteRenderer()override;
        //light info? ok


    private:
        BRDFPassNode brdfNode;
        DataStruct m_data;
        //
        //mesh data vector
        //material
    };

}