//
// Created by chaseaishang on 23-6-14.
//

#pragma once
#ifdef CSAS_ENABLE_ASSERTS
#define CSAS_ASSERT(x, ...) { if(!(x)) { CSAS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
	#define CSAS_CORE_ASSERT(x, ...) { if(!(x)) { CSAS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __builtin_trap(); } }
#else
#define CSAS_ASSERT(x, ...)
#define CSAS_CORE_ASSERT(x, ...)
#endif
#define BIT(x) (1 << x)
//[this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
//std::bind(&fn, this, std::placeholders::_1)
#define CSAS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace CsasEngine {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }


}
