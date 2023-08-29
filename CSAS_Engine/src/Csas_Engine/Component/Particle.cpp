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
            ParticleAlivePreSimIndices[0]= ShaderStorageBuffer::Create(
                    sizeof(uint)*max_GPUParticle,
                    AccessModel::Dynamic_Draw);
            ParticleAlivePreSimIndices[1]= ShaderStorageBuffer::Create(
                    sizeof(uint)*max_GPUParticle,
                    AccessModel::Dynamic_Draw);
            //ParticleAlivePreSimIndices
            particle_init=Shader::Create("./assets/shaders/particle_effect/particle_initialize.glsl");

            particle_kickoff=Shader::Create("./assets/shaders/particle_effect/particle_update_kickoff.glsl");
            particle_emission=Shader::Create("./assets/shaders/particle_effect/particle_emission.glsl");
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
}