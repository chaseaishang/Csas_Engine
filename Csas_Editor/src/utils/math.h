//
// Created by chaseaishang on 23-8-16.
//

#pragma once
#include "glm/glm.hpp"
namespace CsasEngine {
    namespace Utils::math
    {
        glm::vec3 HSL2RGB(float h, float s, float l);
        glm::vec3 HSL2RGB(const glm::vec3& hsl);

    }
}