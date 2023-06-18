//
// Created by chaseaishang on 23-6-18.
//

#include "Csas_Engine/Csaspch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Csas_Engine/Platform/OpenGL/OpenGLVertexArray.h"

namespace CsasEngine {

VertexArray* VertexArray::Create()
{
    switch (Renderer::GetAPI())
    {
        case RendererAPI::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::OpenGL:  return new OpenGLVertexArray();
    }

    CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

}
