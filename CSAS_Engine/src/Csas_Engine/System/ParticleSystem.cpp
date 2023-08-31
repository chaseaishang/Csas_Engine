//
// Created by chaseaishang on 23-8-23.
//


#include <Particle.h>
#include "ParticleSystem.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Csas_Engine/Utils/math.h"
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
        uint gpu_count=0;
        if(emit)
        {
            //find and creat more
            //set
            mesh.UpdatePool(now_time,ParticlelifeTime);
            auto live=mesh.AddParticle(now_time,10);
            gpu_count=50;

            emit= false;

        }
        UpdateLogic(particle,positon,gpu_count);


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

        kickoffshader->SetuInt("u_ParticlesPerFrame",emit_count);
        kickoffshader->SetInt("u_PreSimIdx",m_pre_sim_idx);
        kickoffshader->SetInt("u_PostSimIdx",m_post_sim_idx);
        particle.ParticleCounter->Bind(0);
        particle.EmissionDispatchArgs->Bind(1);
        particle.SimulationDispatchArgs->Bind(2);
        kickoffshader->Dispatch(1,1,1);
        kickoffshader->SyncWait(ComputeShader::ComputeSync::SSBO_FIN);


            float lifetime=10.0f;
            //position
            float velocity=0;
            glm::vec3 v;
            {
                using namespace Utils::math;
                auto theta = glm::mix(0.0f, glm::pi<float>()/ 6.0f, RandomGenerator<float>());
                auto phi = glm::mix(0.0f, glm::pi<float>(), RandomGenerator<float>());
                v.x = sinf(theta) * cosf(phi);
                v.y = cosf(theta);
                v.z = sinf(theta) * sinf(phi);
// Scale to set the magnitude of the velocity (speed)
                velocity = glm::mix(0.5f,1.0f,RandomGenerator<float>());
                v = v * velocity;
            }
            auto&emitShader=particle.particle_emission;
            emitShader->Bind();
            glm::vec3 bias{0,1,0};
            emitShader->SetFloat3("u_EmitterPosition",position+bias);
            emitShader->SetFloat3("u_EmitterVelocity",v);
            emitShader->SetFloat("u_EmitterLifetime",lifetime);
            emitShader->SetInt("u_PreSimIdx",m_pre_sim_idx);
            particle.ParticleData->Bind(0);
            particle.ParticleDeadIndices->Bind(1);
            particle.ParticleAlivePreSimIndices[m_pre_sim_idx]->Bind(2);
            particle.ParticleCounter->Bind(3);
            particle.EmissionDispatchArgs->BindForCShader();
            emitShader->DispatchComputeIndirect(0);
            emitShader->SyncWait(ComputeShader::ComputeSync::ALL_FIN);



        particle.ParticleCounter->Get_Data(sizeof(uint),12,&particle.gpu_Live_count);
//        if(emit_count>0)
//        {
//            CSAS_CORE_INFO("Count {0}",particle.gpu_Live_count);
//        }

    }

    void ParticleSystem::Gpu_emit(const Particles &particle, const glm::vec3 &position)
    {

    }


}