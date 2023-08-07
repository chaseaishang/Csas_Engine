//
// Created by chaseaishang on 23-7-10.
//
#include "Csas_Engine/Csaspch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace CsasEngine {

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
            : m_Specification(spec)
    {
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteTextures(1, &m_DepthAttachment);
    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            glDeleteTextures(1, &m_ColorAttachment);
            glDeleteTextures(1, &m_DepthAttachment);
        }
        glCreateFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0,
        // 	GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        CSAS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Bind()
    {

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, m_Specification.Width, m_Specification.Height);
    }

    void OpenGLFramebuffer::Unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
    static const uint32_t s_MaxFramebufferSize = 8192;
    void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
    {
        if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
        {
            CSAS_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
            return;
        }
        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }

    void OpenGLFramebuffer::AddColorTexture(size_t count)
    {
        size_t n_color_buffs = color_textures.size();
        static const float border[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        for (GLuint i = 0; i < count; i++)
        {
            GLenum target = GL_TEXTURE_2D;
            uint32_t renderID;
            glCreateTextures(GL_TEXTURE_2D, 1, &renderID);
            glTextureStorage2D(renderID, 1, GL_RGBA8,  m_Specification.Width, m_Specification.Height);

            glTextureParameteri(renderID ,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(renderID ,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(renderID ,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTextureParameteri(renderID ,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTextureParameterfv(renderID, GL_TEXTURE_BORDER_COLOR, border);


            glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + n_color_buffs + i, renderID, 0);
            color_textures.push_back(renderID);
        }
        // enable multiple render targets
        if (size_t n = color_textures.size(); n > 0) {
            GLenum* attachments = new GLenum[n];

            for (GLenum i = 0; i < n; i++) {
                *(attachments + i) = GL_COLOR_ATTACHMENT0 + i;
            }

            glNamedFramebufferDrawBuffers(m_RendererID, n, attachments);
            delete[] attachments;
        }
        CSAS_CORE_ASSERT(glCheckNamedFramebufferStatus(m_RendererID,GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        m_ColorAttachment=color_textures[0];

    }

    void OpenGLFramebuffer::AddDepStTexture()
    {

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glTextureStorage2D(m_DepthAttachment, 1, GL_DEPTH24_STENCIL8,  m_Specification.Width, m_Specification.Height);
        glTextureParameteri(m_DepthAttachment, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
        GLint immutable_format;
        glGetTextureParameteriv(m_DepthAttachment, GL_TEXTURE_IMMUTABLE_FORMAT, &immutable_format);

        if (immutable_format != GL_TRUE)
        {
           CSAS_CORE_ERROR("Unable to attach an immutable depth stencil texture...");
            return;
        }
        glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);
        CSAS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    }

}
