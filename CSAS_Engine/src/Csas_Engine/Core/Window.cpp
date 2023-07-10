//
// Created by chaseaishang on 23-7-2.
//
#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Core/Window.h"
#ifdef CSAS_PLATFORM_LINUX
    #include "Csas_Engine/Platform/Linux/LinuxWindow.h"
#endif
namespace CsasEngine
{

    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef CSAS_PLATFORM_LINUX
        return CreateScope<LinuxWindow>(props);
#else
        CSAS_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

}
