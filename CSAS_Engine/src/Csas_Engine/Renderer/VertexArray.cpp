//
// Created by chaseaishang on 23-6-18.
//

#include "Csas_Engine/Csaspch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Csas_Engine/Platform/OpenGL/OpenGLVertexArray.h"

namespace CsasEngine {


    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}



