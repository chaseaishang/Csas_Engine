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

        // create random number generator engines and set engine state
        static std::random_device device;               // acquire seed from device()
        static std::mt19937       engine_32(device());  // seed only once at startup
        static std::mt19937_64    engine_64(device());  // seed only once at startup

        // construct static uniform distributions
        static std::uniform_int_distribution<uint32_t> uint32_t_distributor(0, std::numeric_limits<uint32_t>::max());
        static std::uniform_int_distribution<uint64_t> uint64_t_distributor(0, std::numeric_limits<uint64_t>::max());
        static std::uniform_real_distribution<double>  double_t_distributor(0.0, 1.0);

        template<typename T>
        T RandomGenerator()
        {
            if constexpr (std::is_same_v<T, uint64_t>)
            {
                return uint64_t_distributor(engine_64);
            }
            else if constexpr (std::is_same_v<T, uint32_t>)
            {
                return uint32_t_distributor(engine_32);
            }
            else if constexpr (std::is_same_v<T, double>)
            {
                return double_t_distributor(engine_32);
            }
            else if constexpr (std::is_same_v<T, float>)
            {
                return static_cast<float>(double_t_distributor(engine_32));
            }
            else {
                CSAS_CORE_ASSERT(true, "Unsupported integral type  ...");
            }

        }
        // explicit template function instantiation
        template uint64_t RandomGenerator();
        template uint32_t RandomGenerator();
        template double   RandomGenerator();
        template float    RandomGenerator();

    }
}