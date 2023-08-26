//
// Created by chaseaishang on 23-8-16.
//

#pragma once
#include <random>
#include "glm/glm.hpp"
namespace CsasEngine {
    namespace Utils::math {
        // create random number generator engines and set engine state
        static std::random_device device;               // acquire seed from device()
        static std::mt19937 engine_32(device());  // seed only once at startup
        static std::mt19937_64 engine_64(device());  // seed only once at startup

        // construct static uniform distributions
        static std::uniform_int_distribution<uint32_t> uint32_t_distributor(0, std::numeric_limits<uint32_t>::max());
        static std::uniform_int_distribution<uint64_t> uint64_t_distributor(0, std::numeric_limits<uint64_t>::max());
        static std::uniform_real_distribution<double> double_t_distributor(0.0, 1.0);

        template<typename T>
        T RandomGenerator() {
            if constexpr (std::is_same_v<T, uint64_t>) {
                return uint64_t_distributor(engine_64);
            } else if constexpr (std::is_same_v<T, uint32_t>) {
                return uint32_t_distributor(engine_32);
            } else if constexpr (std::is_same_v<T, double>) {
                return double_t_distributor(engine_32);
            } else if constexpr (std::is_same_v<T, float>) {
                return static_cast<float>(double_t_distributor(engine_32));
            } else {
                CSAS_CORE_ASSERT(true, "Unsupported integral type  ...");
            }

        }

        // explicit template function instantiation
        template uint64_t RandomGenerator();

        template uint32_t RandomGenerator();

        template double RandomGenerator();

        template float RandomGenerator();
    }
}