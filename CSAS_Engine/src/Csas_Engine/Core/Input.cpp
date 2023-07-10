//
// Created by chaseaishang on 23-7-2.
//
#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Core/Input.h"

#ifdef CSAS_PLATFORM_LINUX
#include "Csas_Engine/Platform/Linux/LinuxInput.h"
#endif

namespace CsasEngine {

    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {

#ifdef CSAS_PLATFORM_LINUX
        return CreateScope<LinuxInput>();
#else
        CSAS_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
}
