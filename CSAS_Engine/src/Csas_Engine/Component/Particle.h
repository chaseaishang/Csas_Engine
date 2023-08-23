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
        Particles();
    public:
        uint n_particles;



    };
}