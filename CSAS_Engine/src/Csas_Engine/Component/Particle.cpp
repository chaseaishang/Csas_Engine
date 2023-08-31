//
// Created by chaseaishang on 23-8-23.
//

#include "Particle.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Texture.h"
#include "glm/glm.hpp"
namespace CsasEngine
{
    Particles::Particles(glm::vec3 Gravity,float ParticleLifetime)
    :Gravity(Gravity),ParticleLifetime(ParticleLifetime)
    {
        type=MaterialType::Particle;
        m_Shader = Shader::Create("./assets/shaders/particle_effect/basic.glsl");
        m_Shader->SetFloat3("Gravity",Gravity);
        m_Shader->SetFloat("ParticleLifetime",ParticleLifetime);

        {
            ParticleDeadIndices= ShaderStorageBuffer::Create(
                    sizeof(uint)*max_GPUParticle,
                     AccessModel::Dynamic_Draw);
            ParticleCounter= ShaderStorageBuffer::Create(
                    sizeof(uint)*5,
                    AccessModel::Dynamic_Draw);
            ParticleData= ShaderStorageBuffer::Create(
                    sizeof(float )*16*max_GPUParticle,
                    AccessModel::Dynamic_Draw);
            EmissionDispatchArgs= ShaderStorageBuffer::Create(
                    sizeof(uint )*4,
                    AccessModel::Dynamic_Draw);
            SimulationDispatchArgs= ShaderStorageBuffer::Create(
                    sizeof(uint )*4,
                    AccessModel::Dynamic_Draw);
            ParticleAlivePreSimIndices[0]= ShaderStorageBuffer::Create(
                    sizeof(uint)*max_GPUParticle,
                    AccessModel::Dynamic_Draw);
            ParticleAlivePreSimIndices[1]= ShaderStorageBuffer::Create(
                    sizeof(uint)*max_GPUParticle,
                    AccessModel::Dynamic_Draw);
            //ParticleAlivePreSimIndices
            particle_init=ComputeShader::Create("./assets/shaders/particle_effect/particle_initialize.glsl");

            particle_kickoff=ComputeShader::Create("./assets/shaders/particle_effect/particle_update_kickoff.glsl");
            particle_emission=ComputeShader::Create("./assets/shaders/particle_effect/particle_emission.glsl");
            particle_simulation=ComputeShader::Create("./assets/shaders/particle_effect/particle_simulation.glsl");
            GPU_render=Shader::Create("./assets/shaders/particle_effect/gpu_basic.glsl");
        }
        {//init gpu particle
            particle_init->Bind();
            particle_init->SetuInt("u_MaxParticles",this->max_GPUParticle);
            ParticleDeadIndices->Bind(0);
            ParticleCounter->Bind(1);
            double x=glm::ceil(max_GPUParticle/32.0f);
            particle_init->Dispatch((uint)x,1,1);
            particle_init->SyncWait(ComputeShader::ComputeSync::SSBO_FIN);
        }
    }

    void Particles::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);

    }

    void Particles::UpdateGpu_particle(glm::mat4 &model)
    {
        {
            particle_simulation->Bind();
            ParticleData->Bind(0);
            ParticleDeadIndices->Bind(1);
            ParticleAlivePreSimIndices[m_preIndex]->Bind(2);
            ParticleAlivePreSimIndices[m_postIndex]->Bind(3);
            ParticleCounter->Bind(4);
            particle_simulation->SetInt("u_PreSimIdx", m_preIndex);
            particle_simulation->SetInt("u_PostSimIdx", m_postIndex);
            SimulationDispatchArgs->BindForCShader();
            particle_simulation->DispatchComputeIndirect(0);
            particle_simulation->SyncWait(ComputeShader::ComputeSync::ALL_FIN);
        }
        {
            GPU_render->Bind();
            particle_simulation->SetInt("u_PreSimIdx", m_preIndex);
            particle_simulation->SetInt("u_PostSimIdx", m_postIndex);
            ParticleData->Bind(0);
            ParticleAlivePreSimIndices[m_postIndex]->Bind(3);
            GPU_render->SetMat4("model",model);

        }

        //finally do ping pong
        m_preIndex  = m_preIndex == 0 ? 1 : 0;
        m_postIndex = m_postIndex == 0 ? 1 : 0;

    }
}