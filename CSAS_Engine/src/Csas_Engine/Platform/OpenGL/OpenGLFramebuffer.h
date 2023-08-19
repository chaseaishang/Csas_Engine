//
// Created by chaseaishang on 23-7-10.
//
#pragma once

#include "Csas_Engine/Renderer/Framebuffer.h"

namespace CsasEngine {
    class Texture2D;
    class OpenGLFramebuffer : public Framebuffer
    {
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        void Invalidate();
        void Clear(int index)const override;
        void ClearAll() override;
        virtual void Bind() override;
        virtual void Unbind() override;

        virtual void Resize(uint32_t width, uint32_t height) override;
        Texture2D* GetColorAttachment(uint32_t index) const override;
        virtual uint32_t GetColorAttachmentRendererID() const override { return m_ColorAttachment; }
        virtual uint32_t GetRendererID()const{return m_RendererID;};
        virtual uint32_t GetDepthRendererID() const;
        virtual Texture2D* GetDepthAttachment() const;
        void AddColorTexture(size_t count)override;
        void AddDepStTexture()override;


        virtual const FramebufferSpecification& GetSpecification() const override { return m_Specification; }
    private:
        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0;
        //std::vector<uint32_t>     color_textures;
        std::vector<Ref<Texture2D>>     Color_textures;
        Ref<Texture2D>                  DepthAttachment= nullptr;
        FramebufferSpecification m_Specification;

    };

}
