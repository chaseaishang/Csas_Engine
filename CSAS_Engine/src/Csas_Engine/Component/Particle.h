//
// Created by chaseaishang on 23-8-23.
//

#pragma once
#include "Material.h"
#include "glm/glm.hpp"
namespace CsasEngine
{
    struct Particles :public Material
    {
    public:
        Particles(uint count=100,glm::vec3 Gravity=glm::vec3{0.0,-0.005,0.0},float ParticleLifetime=10);
        ~Particles() override =default ;
        void Update(glm::mat4 & model);
    public:
        uint n_particles;

        glm::vec3 Gravity=glm::vec3{0.0,-0.05,0.0};
        float ParticleLifetime; // Max particle lifetime


    };
}