//
// Created by chaseaishang on 23-8-23.
//

#include "Particle.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Texture.h"

namespace CsasEngine
{
    Particles::Particles(glm::vec3 Gravity,float ParticleLifetime)
    :Gravity(Gravity),ParticleLifetime(ParticleLifetime)
    {
        type=MaterialType::Particle;
        m_Shader = Shader::Create("./assets/shaders/particle_effect/basic.glsl");
        m_Shader->SetFloat3("Gravity",Gravity);
        m_Shader->SetFloat("ParticleLifetime",ParticleLifetime);
        Particle_ColorSSBO=ShaderStorageBuffer::Create(sizeof(float)*4,AccessModel::Dynamic_Draw);

    }

    void Particles::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
        Particle_ColorSSBO->Bind(0);
    }
}