//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include <Csas_Engine/Renderer/RenderPipeline/RenderData/RenderData.h>
#include "PassData.h"

namespace CsasEngine
{
//  data camera
    class Framebuffer;
    class UniformBuffer;
    struct Material_BaseBRDF;
    struct SpotLightComponent;
    struct MeshComponent;
    class PassNode
    {
    public:

        PassNode()=default;
        virtual ~PassNode()=default;
        virtual void OnPrepare(PassData*data)=0;
//        virtual void OnSubmit()=0;
        virtual void OnExecute(PassData*data)=0;
//        virtual void OnFinish()=0;
    };
    class BRDFPassNode:public PassNode
    {
    public:

        struct BRDFPassData:public PassData
        {
            std::vector<Material_BaseBRDF*> material;
            std::vector<MeshComponent*>meshs;
            std::vector<SpotLightComponent*>spots;
            Ref<Framebuffer>fbo;
            CameraPtr camera;

            //light color
            //position
        };
        using DataStruct=BRDFPassData;

        BRDFPassNode()=default;
        BRDFPassNode(BRDFPassData&data);
        ~BRDFPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:
        Ref<UniformBuffer> CameraUBO;

    };

}