//
// Created by chaseaishang on 23-7-23.
//

#include "UniformBuffer.h"
#include "Csas_Engine/Csaspch.h"
#include "UniformBuffer.h"

#include "Csas_Engine/Renderer/Renderer.h"
#include "Csas_Engine/Platform/OpenGL/OpenGLUniformBuffer.h"
namespace CsasEngine
{
    Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


}