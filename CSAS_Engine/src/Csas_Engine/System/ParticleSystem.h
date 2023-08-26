//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "BaseSystem.h"
//           also Component It needs   some attributes
//           manage some particle emitter
//           can kill them   recycle them
//           All particle should in its leaf node
//           Main attribute :    looping.            loop emit or just once
//                               interval time       the time between two emit

//TO be done Timer

namespace CsasEngine
{

    class Particles;
    class MeshComponent_ParticleVertex;
    class ParticleSystem:public BaseSystem
    {
        friend class Scene;
    public:
        bool emit= false;
        bool looping=false;
        bool stop= false;
        float now_time;
        uint ParticlelifeTime=10;
    public:
        void OnImGui()override;
        ParticleSystem();
        void SyncUpdate()override{};
        void Update(MeshComponent_ParticleVertex&mesh,Particles&particle,float now_time);
        //void Mount(Entity&entity);

    };
}