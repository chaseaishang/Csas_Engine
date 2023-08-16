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
        OpenGLTexture2D(const std::string& path,TextureSpecification Spec);
        OpenGLTexture2D(TextureSpecification Spec);
        //
        virtual ~OpenGLTexture2D();

        uint32_t GetWidth() const override { return m_textureSpecification.width;  }
        uint32_t GetHeight() const override { return m_textureSpecification.height; }
        uint32_t GetRendererID() const override { return m_RendererID; }
        void SetData(void* data, uint32_t size) override;

        void Bind(uint32_t slot = 0) const override;
        void BindILS(uint32_t level, uint32_t index, uint32_t access)const override;
        void UnBindILS(uint32_t index)const;
        bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLTexture2D&)other).m_RendererID;
        }
    private:
        void LoadCacheORSource(const std::string& path);
        void LoadHDRCacheORSource(const std::string& path);
    private:
        std::string m_Path;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
        TextureSpecification m_textureSpecification;
    };
    class OpenGLCubeTexture:public CubeTexture
    {
    public:
        std::tuple<Ref<CubeTexture>,Ref<CubeTexture>,Ref<Texture2D>> PreComputeIBL()override;
        OpenGLCubeTexture(const std::string& path,TextureSpecification Spec);
        OpenGLCubeTexture(TextureSpecification Spec);
        ~OpenGLCubeTexture()override;
        uint32_t GetWidth() const override { return m_Width;  }
        uint32_t GetHeight() const override { return m_Height; }
        uint32_t GetRendererID() const override { return m_RendererID; }
        void SetData(void* data, uint32_t size) override;

        void Bind(uint32_t slot = 0) const override;
        void BindILS(uint32_t level, uint32_t index, uint32_t access)const override;
        void UnBindILS(uint32_t index)const override;
        bool operator==(const Texture& other) const override
        {
            return m_RendererID == ((OpenGLCubeTexture&)other).m_RendererID;
        }
    private:
        void LoadCacheORSource(const std::string& path);
        void LoadHDRCacheORSource(const std::string& path);
    private:
        std::string m_Path;
        uint32_t m_Width, m_Height;
        uint32_t m_RendererID;
        GLenum m_InternalFormat, m_DataFormat;
        TextureSpecification m_textureSpecification;
    };



}

