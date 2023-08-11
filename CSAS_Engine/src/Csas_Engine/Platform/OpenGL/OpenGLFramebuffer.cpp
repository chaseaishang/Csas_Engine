//
// Created by chaseaishang on 23-7-10.
//
#include "Csas_Engine/Csaspch.h"
#include "OpenGLFramebuffer.h"
#include "Csas_Engine/Renderer/Texture.h"
#include <glad/glad.h>

namespace CsasEngine {

    OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
            : m_Specification(spec)
    {
        glDisable(GL_FRAMEBUFFER_SRGB);  // important! turn off colorspace correction globally
        Invalidate();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        glDeleteFramebuffers(1, &m_RendererID);





    }

    void OpenGLFramebuffer::Invalidate()
    {
        if (m_RendererID)
        {
            glDeleteFramebuffers(1, &m_RendererID);

            DepthAttachment= nullptr;
            Color_textures.clear();


            m_ColorAttachment=0;
        }
        glCreateFramebuffers(1, &m_RendererID);
        if(size_t size=m_Specification.ColorAttachmentSize;m_Specification.Has_Depth)
        {
            AddColorTexture(size);
            AddDepStTexture();
        }



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
        size_t n_color_buffs = Color_textures.size();
        static const float border[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        TextureSpecification spec;
        if(m_Specification.Hdr)
        {
            spec.target=GL_RGBA16F;
            spec.hdr= true;

        }
        else
        {
            spec.target=GL_RGBA8;

        }
        spec.size=1;
        spec.height=m_Specification.Height;
        spec.width=m_Specification.Width;

        for (GLuint i = 0; i < count; i++)
        {
            GLenum target = GL_TEXTURE_2D;
            Ref<Texture2D>texture=Texture2D::Create(spec);

            const auto&renderID=texture->GetRendererID();


            glTextureParameteri(renderID ,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(renderID ,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(renderID ,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTextureParameteri(renderID ,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTextureParameterfv(renderID, GL_TEXTURE_BORDER_COLOR, border);


            glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + n_color_buffs + i, renderID, 0);
            Color_textures.push_back(texture);

        }
        // enable multiple render targets
        if (size_t n = Color_textures.size(); n > 0) {
            GLenum* attachments = new GLenum[n];

            for (GLenum i = 0; i < n; i++) {
                *(attachments + i) = GL_COLOR_ATTACHMENT0 + i;
            }

            glNamedFramebufferDrawBuffers(m_RendererID, n, attachments);
            delete[] attachments;
        }
        CSAS_CORE_ASSERT(glCheckNamedFramebufferStatus(m_RendererID,GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        m_ColorAttachment=Color_textures[0]->GetRendererID();

    }

    void OpenGLFramebuffer::AddDepStTexture()
    {


        TextureSpecification spec;
        spec.target=GL_DEPTH24_STENCIL8;
        spec.size=1;
        spec.height=m_Specification.Height;
        spec.width=m_Specification.Width;
        spec.hdr= false;
        DepthAttachment=Texture2D::Create(spec);
        glTextureParameteri(DepthAttachment->GetRendererID(), GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
        GLint immutable_format;
        glGetTextureParameteriv(DepthAttachment->GetRendererID(), GL_TEXTURE_IMMUTABLE_FORMAT, &immutable_format);

        if (immutable_format != GL_TRUE)
        {
           CSAS_CORE_ERROR("Unable to attach an immutable depth stencil texture...");
            return;
        }
        glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, DepthAttachment->GetRendererID(), 0);
        CSAS_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

    }

    void OpenGLFramebuffer::Clear(uint8_t index) const
    {
        static GLfloat clear_color[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
        static GLfloat clear_depth = 1.0f;
        static GLint clear_stencil = 0;
        // a framebuffer always has a depth buffer, a stencil buffer and all color buffers,
        // an empty one just doesn't have any textures attached to it, but all buffers are
        // still there. It's ok to clear a buffer even if there's no textures attached, we
        // don't need to check `index < color_textures.size()` or `depst_texture != nullptr`



        // clear one of the color attachments
        if (index >= 0 ) {
            glClearNamedFramebufferfv(m_RendererID, GL_COLOR, index, clear_color);
        }
            // clear the depth buffer
        else if (index == -1) {
            glClearNamedFramebufferfv(m_RendererID, GL_DEPTH, 0, &clear_depth);
        }
            // clear the stencil buffer
        else if (index == -2) {
            glClearNamedFramebufferiv(m_RendererID, GL_STENCIL, 0, &clear_stencil);
        }
        else
        {
            CSAS_CORE_ERROR("Buffer index {0} is not valid in the framebuffer!", index);

        }


    }

    void OpenGLFramebuffer::ClearAll() {
        for (int i = 0; i < Color_textures.size(); i++)
        {
            Clear(i);
        }
        m_ColorAttachment=0;
        Clear(-1);
        Clear(-2);

    }

    Texture2D* OpenGLFramebuffer::GetColorAttachment(uint32_t index) const
    {
        CSAS_CORE_ASSERT(index<Color_textures.size(),"error");

        return Color_textures[index].get();

    }

    void Framebuffer::TransferColor(const Framebuffer& fr, uint fr_idx, const Framebuffer& to, uint to_idx)
    {
        auto&FrSpec=fr.GetSpecification();
        auto&ToSpec=to.GetSpecification();
        GLuint fw = FrSpec.Width, fh = FrSpec.Height;
        GLuint tw = ToSpec.Width, th = ToSpec.Height;
        glNamedFramebufferReadBuffer(fr.GetRendererID(), GL_COLOR_ATTACHMENT0 + fr_idx);
        glNamedFramebufferDrawBuffer(to.GetRendererID(), GL_COLOR_ATTACHMENT0 + to_idx);
        glBlitNamedFramebuffer(fr.GetRendererID(), to.GetRendererID(),
                               0, 0, fw, fh,
                               0, 0, tw, th,
                               GL_COLOR_BUFFER_BIT, GL_NEAREST);

    }


}
