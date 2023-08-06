//
// Created by chaseaishang on 23-6-24.
//
#pragma once

#include "Csas_Engine/Renderer/Texture.h"

#include <glad/glad.h>

namespace CsasEngine {

    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const std::string& path);
        virtual ~OpenGLTexture2D();

        uint32_t GetWidth() const override { return m_Width;  }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetRendererID() const override { return m_RendererID; }
        void SetData(void* data, uint32_t size) override;

        void Bind(uint32_t slot = 0) const override;
        bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };
    class OpenGLCubeTexture:public CubeTexture
    {
    public:
        OpenGLCubeTexture(const std::string& path);
        uint32_t GetWidth() const override { return m_Width;  }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetRendererID() const override { return m_RendererID; }
        void SetData(void* data, uint32_t size) override;

        void Bind(uint32_t slot = 0) const override;
        bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLCubeTexture&)other).m_RendererID;
        }
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
    };



}

