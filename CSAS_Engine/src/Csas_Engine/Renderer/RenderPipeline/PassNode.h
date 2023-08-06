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
    enum class PassNodeType
    {
        BrdfPass,
        Skybox
    };
    class PassNode
    {
    public:
        PassNodeType type;
        PassNode()=default;
        virtual ~PassNode()=default;
        virtual void OnPrepare(PassData*data)=0;
//        virtual void OnSubmit()=0;
        virtual void OnExecute(PassData*data)=0;
//        virtual void OnFinish()=0;
    };
    class SkyboxPassNode:public PassNode
    {
    public:

        struct SkyboxPassData:public PassData
        {

            SkyboxPassData()=default;
            RenderData data;
        };

        SkyboxPassNode(){type=PassNodeType::Skybox;}
        ~SkyboxPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:


    };
    class BRDFPassNode:public PassNode
    {
    public:

        struct BRDFPassData:public PassData
        {
        public:
            BRDFPassData()=default;
            RenderDataVec data_vec;
            std::vector<SpotLightComponent*>spots;

        };

        BRDFPassNode(){type=PassNodeType::BrdfPass;}
        BRDFPassNode(BRDFPassData&data);
        ~BRDFPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:


    };

}