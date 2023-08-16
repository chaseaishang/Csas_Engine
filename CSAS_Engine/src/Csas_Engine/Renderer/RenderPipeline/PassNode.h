//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include <Csas_Engine/Renderer/RenderPipeline/RenderData/RenderData.h>
#include "PassData.h"

namespace CsasEngine
{
    class Texture2D;
    class Shader;
//  data camera
    class Framebuffer;
    class UniformBuffer;
    struct Material_BaseBRDF;
    struct SpotLightComponent;
    struct MeshComponent;
    enum class PassNodeType
    {
        BrdfPass,
        Skybox,
        BlurPass,
        LightPass
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
            ~SkyboxPassData()=default;
            RenderData data;
        };

        SkyboxPassNode(){type=PassNodeType::Skybox;}
        ~SkyboxPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:


    };
    class BlurPassNode:public PassNode
    {
    public:

        struct BlurPassData:public PassData
        {

            BlurPassData()=default;
            ~BlurPassData()=default;
            Ref<Material_Blur>blur_material= nullptr;
            Texture2D*source_tex;
        };

        BlurPassNode(){type=PassNodeType::BlurPass;}
        ~BlurPassNode()override=default;
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
            ~BRDFPassData()=default;
            RenderDataVec data_vec;
            const CubeTexture*irradiance_map;
            const CubeTexture*prefiltered_map;
            const Texture2D*BRDF_LUT;
            
        };

        BRDFPassNode(){type=PassNodeType::BrdfPass;}
        BRDFPassNode(BRDFPassData&data);
        ~BRDFPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:


    };
    class LightPassNode:public PassNode
    {
    public:

        struct LightPassData:public PassData
        {
            RenderDataVec data_vec;
            LightPassData()=default;
            ~LightPassData()=default;
        };

        LightPassNode(){type=PassNodeType::LightPass;}
        ~LightPassNode()override=default;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;
    private:


    };

}