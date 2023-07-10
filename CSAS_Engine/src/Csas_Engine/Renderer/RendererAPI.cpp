//
// Created by chaseaishang on 23-6-19.
//


#include "Csas_Engine/Csaspch.h"
#include "RendererAPI.h"
#include "Csas_Engine/Platform/OpenGL/OpenGLRendererAPI.h"
namespace CsasEngine
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
            case RendererAPI::API::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


}

