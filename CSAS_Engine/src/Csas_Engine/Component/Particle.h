//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "Material.h"
#include "glm/glm.hpp"
namespace CsasEngine
{
    class ShaderStorageBuffer;
    struct Particles :public Material
    {
        friend class ParticleSystem;
    public:
        Particles(glm::vec3 Gravity=glm::vec3{0.0,-0.005,0.0},float ParticleLifetime=10);
        ~Particles() override =default ;
        void Update(glm::mat4 & model);
    public:
        uint live_count=0;
        glm::vec3 Gravity=glm::vec3{0.0,-0.05,0.0};
        float ParticleLifetime; // Max particle lifetime
    private:
        uint max_GPUParticle=1000;
        Ref<Shader> particle_init;
        Ref<Shader> particle_emission;
        Ref<Shader> particle_simulation;
        Ref<Shader> particle_kickoff;
        Ref<ShaderStorageBuffer>ParticleDeadIndices;
        Ref<ShaderStorageBuffer>ParticleCounter;
        Ref<ShaderStorageBuffer>ParticleData;
        Ref<ShaderStorageBuffer>ParticleAlivePreSimIndices[2];
        uint m_preIndex=0;
        uint m_postIndex=1;


    };
}