//
// Created by chaseaishang on 23-7-2.
//

#include "Csas_Engine/Csaspch.h"
#include "GraphicsContext.h"
#include <OpenGLContext.h>
#include "Renderer.h"

namespace CsasEngine {

    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}
