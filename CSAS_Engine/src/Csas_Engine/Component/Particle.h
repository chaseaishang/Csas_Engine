//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "Material.h"
#include "glm/glm.hpp"
namespace CsasEngine
{
    class ShaderStorageBuffer;
    class ComputeShader;
    struct Particles :public Material
    {
        friend class ParticleSystem;
    public:
        Particles(glm::vec3 Gravity=glm::vec3{0.0,-0.005,0.0},float ParticleLifetime=10);
        ~Particles() override =default ;
        void Update(glm::mat4 & model);
        void UpdateGpu_particle(glm::mat4 & model);
    public:
        uint live_count=0;
        uint gpu_Live_count=0;
        glm::vec3 Gravity=glm::vec3{0.0,-0.05,0.0};
        float ParticleLifetime; // Max particle lifetime
    private:
        uint max_GPUParticle=1000;
        Ref<ComputeShader> particle_init;
        Ref<ComputeShader> particle_emission;
        Ref<ComputeShader> particle_simulation;
        Ref<ComputeShader> particle_kickoff;
        Ref<Shader> GPU_render;
        Ref<ShaderStorageBuffer>ParticleDeadIndices;
        Ref<ShaderStorageBuffer>ParticleCounter;
        Ref<ShaderStorageBuffer>ParticleData;
        Ref<ShaderStorageBuffer>EmissionDispatchArgs;
        Ref<ShaderStorageBuffer>SimulationDispatchArgs;
        Ref<ShaderStorageBuffer>ParticleAlivePreSimIndices[2];
        uint m_preIndex=0;
        uint m_postIndex=1;


    };
}