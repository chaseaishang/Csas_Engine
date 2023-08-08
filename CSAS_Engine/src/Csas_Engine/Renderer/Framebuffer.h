//
// Created by chaseaishang on 23-7-10.
//

#pragma once

#include "Csas_Engine/Core/Core.h"

namespace CsasEngine {

    struct FramebufferSpecification
    {
        uint32_t Width = 0, Height = 0;
        // FramebufferFormat Format =
        bool Has_Depth=true;
        uint32_t ColorAttachmentSize=1;
        uint32_t Samples = 1;

        bool SwapChainTarget = false;
    };
    class Texture2D;
    class Framebuffer
    {
    public:
        virtual ~Framebuffer() = default;
        virtual void Clear(uint8_t index)const=0;
        virtual void ClearAll()=0;
        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        virtual void Resize(uint32_t width, uint32_t height) = 0;
        virtual Texture2D* GetColorAttachment(uint32_t index) const =0;
        virtual uint32_t GetColorAttachmentRendererID() const = 0;
        virtual uint32_t GetRendererID()const=0;
        virtual void AddColorTexture(size_t count)=0;
        virtual void AddDepStTexture()=0;
        //for opengl
        static  void TransferColor(const Framebuffer& fr, uint fr_idx, const Framebuffer& to, uint to_idx);
        virtual const FramebufferSpecification& GetSpecification() const = 0;

        static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
    };


}


