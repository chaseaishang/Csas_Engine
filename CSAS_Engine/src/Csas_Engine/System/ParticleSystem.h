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
    class ParticleSystem:public BaseSystem
    {
    public:
        bool looping=false;
        float now_time;
    public:
        ParticleSystem();
        void Update()override;
        //void Mount(Entity&entity);

    };
}