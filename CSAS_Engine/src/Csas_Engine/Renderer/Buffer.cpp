//
// Created by chaseaishang on 23-6-18.
//
#include "Csas_Engine/Csaspch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Csas_Engine/Platform/OpenGL/OpenGLBuffer.h"

namespace CsasEngine {

    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::OpenGL:  return new OpenGLVertexBuffer(vertices, size);
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::OpenGL:  return new OpenGLIndexBuffer(indices, size);
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}