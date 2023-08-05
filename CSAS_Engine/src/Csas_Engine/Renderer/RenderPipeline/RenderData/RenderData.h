//
// Created by chaseaishang on 23-8-3.
//

#pragma once
#include "Csas_Engine/Component/AllComponent.h"
namespace CsasEngine
{
    typedef  uint8_t RenderIndex;
    //All is pointer
    //Because we never creat OR destroy anything
    using MeshPtrVec    =std::vector<MeshComponent*>;
    using MaterialPtrVec=std::vector<Material*>;
    using CameraPtr     =Camera*;
    using SpotLightPtrVec=std::vector<SpotLightComponent*>;
    struct RenderData
    {
        //noexcept
        RenderData(MeshComponent* &&mesh,Material*&&material)noexcept
            :meshPtr(mesh),materialPtr(material)
        {

        }
        MeshComponent*meshPtr;
        Material*materialPtr;
    };
    using RenderDataVec=std::vector<RenderData>;

}