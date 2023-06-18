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
#define CSAS_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)