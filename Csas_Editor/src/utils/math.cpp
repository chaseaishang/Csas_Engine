//
// Created by chaseaishang on 23-8-16.
//
#include "Csas_Engine/Csaspch.h"
#include "math.h"
#include <random>
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
        glm::vec3 HSL2RGB(int h, int s, int l)
        {
            CSAS_CORE_ASSERT(h>=0&&h<=360,"h is out the range 0~360");
            CSAS_CORE_ASSERT(s>=0&&s<=100,"s is out the range 0~100");
            CSAS_CORE_ASSERT(l>=0&&l<=100,"l is out the range 0~100");
            float f_h=float(h)/360;
            float f_s=float(s)/100;
            float f_l=float(l)/100;

            return HSL2RGB(f_h,f_s,f_l);

        }
        glm::vec3 HSL2RGB(const glm::vec3& hsl)
        {
            return HSL2RGB(hsl.x, hsl.y, hsl.z);
        }


    }
}