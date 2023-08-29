//
// Created by chaseaishang on 23-8-23.
//


#include <Particle.h>
#include "ParticleSystem.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine
{

    ParticleSystem::ParticleSystem()
        :BaseSystem("ParticleSystem")
    {

    }

    void ParticleSystem::Update(MeshComponent_ParticleVertex&mesh,Particles&particle,float now_time)
    {
        auto positon=mesh.transform.Translation;

        if(emit)
        {
            //find and creat more
            //set
            mesh.UpdatePool(now_time,ParticlelifeTime);
            auto live=mesh.AddParticle(now_time,10);
            UpdateLogic(particle,positon,10);
            emit= false;
            mesh.gpu_Live_count+=10;
        }
        else
        {
            UpdateLogic(particle,positon,0);
        }

        particle.live_count=mesh.Live_count;
        Renderer3D::Submit(mesh,particle);
    }

    void ParticleSystem::OnImGui()
    {
        if(ImGui::Button("emit"))
        {
            this->emit= true;
        }
    }

    void ParticleSystem::UpdateLogic(Particles &particle,const glm::vec3& position,const uint emit_count)
    {

        auto&m_pre_sim_idx=particle.m_preIndex;
        auto&m_post_sim_idx=particle.m_postIndex;
        auto&kickoffshader=particle.particle_kickoff;
        kickoffshader->Bind();

        kickoffshader->SetInt("u_ParticlesPerFrame",emit_count);
        kickoffshader->SetInt("u_PreSimIdx",m_pre_sim_idx);
        kickoffshader->SetInt("u_PostSimIdx",m_post_sim_idx);
        particle.ParticleCounter->Bind(1);
        kickoffshader->Dispatch(1,1,1);
        kickoffshader->SyncWait(ComputeShader::ComputeSync::SSBO_FIN);
        {

            float lifetime=10.0f;
            //position
            glm::vec3 velocity=glm::vec3 {0,0.05,0};
            auto&emitShader=particle.particle_emission;
            emitShader->Bind();
            emitShader->SetFloat3("u_EmitterPosition",position);
            emitShader->SetFloat3("u_EmitterVelocity",velocity);
            emitShader->SetFloat("u_EmitterLifetime",lifetime);
            emitShader->SetInt("u_PreSimIdx",m_pre_sim_idx);
            particle.ParticleData->Bind(0);
            particle.ParticleDeadIndices->Bind(1);
            particle.ParticleAlivePreSimIndices[m_pre_sim_idx]->Bind(2);
            particle.ParticleCounter->Bind(3);
            emitShader->Dispatch(1,1,1);
            emitShader->SyncWait(ComputeShader::ComputeSync::SSBO_FIN);
//            uniform vec3  u_EmitterPosition;
//            uniform vec3  u_EmitterVelocity;
//            uniform float u_EmitterLifetime;
//            uniform int   u_PreSimIdx;

        }
        //finally do ping pong
        m_pre_sim_idx  = m_pre_sim_idx == 0 ? 1 : 0;
        m_post_sim_idx = m_post_sim_idx == 0 ? 1 : 0;


    }

    void ParticleSystem::Gpu_emit(const Particles &particle, const glm::vec3 &position)
    {

    }


}