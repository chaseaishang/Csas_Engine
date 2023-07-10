//
// Created by chaseaishang on 23-7-10.
//
#include "Csas_Engine/Csaspch.h"
#include "Framebuffer.h"

#include "Csas_Engine/Renderer/Renderer.h"

#include "Csas_Engine/Platform/OpenGL/OpenGLFramebuffer.h"

namespace CsasEngine {

    Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    CSAS_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateRef<OpenGLFramebuffer>(spec);
        }

        CSAS_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

}

