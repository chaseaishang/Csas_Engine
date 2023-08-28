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
    public:
        Particles(glm::vec3 Gravity=glm::vec3{0.0,-0.005,0.0},float ParticleLifetime=10);
        ~Particles() override =default ;
        void Update(glm::mat4 & model);
    public:
        uint live_count=0;
        Ref<ShaderStorageBuffer> Particle_ColorSSBO;
        glm::vec3 Gravity=glm::vec3{0.0,-0.05,0.0};
        float ParticleLifetime; // Max particle lifetime


    };
}