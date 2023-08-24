//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "../PassNode.h"
namespace CsasEngine {
    class ParticlePassNode:public PassNode
    {
    public:
        struct ParticlePassData:public PassData
        {

            ParticlePassData()=default;
            ~ParticlePassData()=default;
            RenderDataVec data;
        };

        ParticlePassNode(){type=PassNodeType::ParticlePass;}
        ~ParticlePassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute(PassData*data) override;

    };
}
