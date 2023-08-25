//
// Created by chaseaishang on 23-8-23.
//


#include <Particle.h>
#include "ParticleSystem.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
namespace CsasEngine
{

    ParticleSystem::ParticleSystem()
        :BaseSystem("ParticleSystem")
    {

    }

    void ParticleSystem::Update(MeshComponent_ParticleVertex&mesh,Particles&particle,float now_time)
    {



        Renderer3D::Submit(mesh,particle);
    }


}