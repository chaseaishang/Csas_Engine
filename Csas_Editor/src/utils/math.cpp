//
// Created by chaseaishang on 23-8-16.
//

#include "math.h"
namespace CsasEngine
{
    namespace Utils::math
    {
        // creates RGB color from HSL, see https://en.wikipedia.org/wiki/HSL_and_HSV
        glm::vec3 HSL2RGB(float h, float s, float l)
        {
            glm::vec3 rgb = clamp(abs(mod(h * 6.0f + glm::vec3(0.0f, 4.0f, 2.0f), 6.0f) - 3.0f) - 1.0f, 0.0f, 1.0f);
            return l + s * (rgb - 0.5f) * (1.0f - abs(2.0f * l - 1.0f));

        }
        glm::vec3 HSL2RGB(const glm::vec3& hsl)
        {
            return HSL2RGB(hsl.x, hsl.y, hsl.z);
        }
    }
}