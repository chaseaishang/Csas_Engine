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
    using DirectLightPtrVec=std::vector<DirectionLightComponent*>;
    struct Light_Data
    {
        const SpotLightPtrVec Spots;
        const MeshPtrVec Spot_meshes;
        const DirectLightPtrVec Directs;
        const MeshPtrVec Direct_meshes;
    };
    struct RenderData
    {
        RenderData()=default;
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