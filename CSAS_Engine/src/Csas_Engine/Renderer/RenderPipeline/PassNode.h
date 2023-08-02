//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include "PassData.h"

namespace CsasEngine
{
//  data camera
    struct Material_BaseBRDF;
    struct SpotLightComponent;
    class PassNode
    {
    public:

        PassNode()=default;
        virtual ~PassNode()=default;
        virtual void OnPrepare(PassData*data)=0;
//        virtual void OnSubmit()=0;
        virtual void OnExecute()=0;
//        virtual void OnFinish()=0;
    };
    class BRDFPassNode:public PassNode
    {
    public:
        struct BRDFPassData:public PassData
        {
            Material_BaseBRDF* material;
            std::vector<SpotLightComponent>*ptr_spot_vec;
            //light color
            //position
        };
        BRDFPassNode();
        ~BRDFPassNode()override;
        void OnPrepare(PassData*data)override;

        void OnExecute() override;

    };

}