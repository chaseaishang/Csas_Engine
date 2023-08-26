//
// Created by chaseaishang on 23-8-23.
//


#include <Particle.h>
#include "ParticleSystem.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine
{

    ParticleSystem::ParticleSystem()
        :BaseSystem("ParticleSystem")
    {

    }

    void ParticleSystem::Update(MeshComponent_ParticleVertex&mesh,Particles&particle,float now_time)
    {

        mesh.UpdatePool(now_time,ParticlelifeTime);
        if(emit)
        {
            //find and creat more
            //set
            mesh.AddParticle(now_time,10);
            emit= false;
        }
        Renderer3D::Submit(mesh,particle);
    }

    void ParticleSystem::OnImGui()
    {
        if(ImGui::Button("emit"))
        {
            this->emit= true;
        }
    }


}