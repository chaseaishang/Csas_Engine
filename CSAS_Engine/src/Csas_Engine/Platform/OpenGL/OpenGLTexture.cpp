//
// Created by chaseaishang on 23-6-24.
//
#include "Csas_Engine/Csaspch.h"
#include "OpenGLTexture.h"
#include "OpenGLShader.h"

#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <Csas_Engine/Core/Timer.h>

namespace CsasEngine {

    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height,TextureSpecification Spec)
            : m_Width(width), m_Height(height),m_textureSpecification(Spec)
    {
        CSAS_PROFILE_FUNCTION();

        m_InternalFormat = GL_RGBA8;
        m_DataFormat = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    void OpenGLTexture2D::LoadCacheORSource(const std::string &path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            CSAS_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        CSAS_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        CSAS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

        stbi_image_free(data);

    }

    void OpenGLTexture2D::LoadHDRCacheORSource(const std::string &path)
    {
        int width, height, channels;
        stbi_set_flip_vertically_on_load(0);
        bool flag=stbi_is_hdr(path.c_str());
        float* data = nullptr;
        {
            CSAS_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");
            data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);

        }
        CSAS_CORE_ASSERT(data,"STBI failure reason: {0}", stbi_failure_reason());
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA16F;
            dataFormat = GL_RGBA;
        }
        else
        {
            internalFormat = GL_RGB16F;
            dataFormat = GL_RGB;
        }

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        CSAS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT, data);

        stbi_image_free(data);

    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path,TextureSpecification Spec)
            : m_Path(path),m_textureSpecification(Spec)
    {
        CSAS_PROFILE_FUNCTION();
        if(Spec.hdr)
        {
            LoadHDRCacheORSource(path);
        }
        else
        {//hdr
            LoadCacheORSource(path);

        }


    }
    OpenGLTexture2D::OpenGLTexture2D(TextureSpecification Spec)
    :m_textureSpecification(Spec)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID,
                           Spec.size,
                           Spec.target,
                           Spec.width, Spec.height);

    }
    OpenGLTexture2D::~OpenGLTexture2D()
    {
        CSAS_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, uint32_t size)
    {
        CSAS_PROFILE_FUNCTION();

        uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        CSAS_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        CSAS_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }

    void OpenGLTexture2D::BindILS(uint32_t level, uint32_t index, uint32_t access) const
    {

        glBindImageTexture(index, m_RendererID, level, GL_TRUE, 0, access, m_InternalFormat);
    }


    void OpenGLCubeTexture::SetData(void *data, uint32_t size)
    {
        CSAS_ASSERT(true,"We don't support now");

    }

    void OpenGLCubeTexture::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot,m_RendererID);
    }
    void GLClearError()
    {
        while (glGetError() != GL_NO_ERROR);
    }
    void GlCheckError()
    {
        auto re=glGetError();
        if(re!=GL_NO_ERROR)
        {
            CSAS_CORE_ERROR("{0}",re);
        }
    }
    OpenGLCubeTexture::OpenGLCubeTexture(const std::string &path,TextureSpecification Spec)
            : m_Path(path),m_textureSpecification(Spec)
    {
        {
            std::string suffix=path.substr(path.find_last_of('.')+1);
            if(suffix=="hdr")
            {
                Spec.hdr=true;
            }


            Timer timer;
            if(Spec.hdr)
            {
                LoadHDRCacheORSource(path);
            }
            else
            {//hdr
                LoadCacheORSource(path);
            }

            CSAS_CORE_WARN("CubeMapTexture took {0} s", timer.Elapsed());
        }

    }

    void OpenGLCubeTexture::LoadCacheORSource(const std::string &path)
    {
        GLClearError();
        CSAS_PROFILE_FUNCTION();

        std::vector<std::string>skybox
                {"posx","negx","posy","negy","posz","negz"
                };
        //{"right","left","top","bottom","front","back"
        //            };
        stbi_uc* datas[6];for(auto & data : datas){data= nullptr;}
        for(int i=0;i<skybox.size();i++)
        {
            auto filename=path+"/"+skybox[i]+".png";
            int width, height, channels;
            stbi_set_flip_vertically_on_load(false);
            auto & data = datas[i];
            {
                CSAS_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std:string&)");
                data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
            }
            CSAS_CORE_ASSERT(data, "Failed to load image!");
            m_Width = width;
            m_Height = height;

            GLenum internalFormat = 0, dataFormat = 0;
            if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            m_InternalFormat = internalFormat;
            m_DataFormat = dataFormat;
            CSAS_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");
        }

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_REPEAT);
        GlCheckError();
        glTextureStorage2D(m_RendererID,1,m_InternalFormat,m_Width,m_Height);
        for(int i=0;i<6;i++)
        {


            GlCheckError();


            glTextureSubImage3D(m_RendererID,0,0,0,0+i,
                                m_Width,m_Height,1,
                                m_DataFormat,GL_UNSIGNED_BYTE,datas[i]

            );
            GlCheckError();
        }


        for(auto&data:datas)
        {
            stbi_image_free(data);
        }

    }

    void OpenGLCubeTexture::LoadHDRCacheORSource(const std::string &path)
    {
        GLClearError();
        OpenGLTexture2D texture(path,m_textureSpecification);
        OpenGLShader shader("./assets/shaders/utils/equirect2cube.glsl");
        texture.Bind(0);
        m_textureSpecification.height=512;
        m_textureSpecification.width=512;
        m_textureSpecification.hdr= true;
        OpenGLCubeTexture cubeTexture(m_textureSpecification);
        cubeTexture.BindILS(0,0,GL_WRITE_ONLY);
        shader.Bind();
        GlCheckError();
        //cubeTexture
        uint wight=texture.GetWidth();
        uint height=texture.GetHeight();
        shader.Dispatch(wight/32,height/32,6);
        GlCheckError();
        shader.SyncWait(GL_SHADER_STORAGE_BARRIER_BIT);
        GlCheckError();
        m_RendererID=cubeTexture.m_RendererID;
        cubeTexture.m_RendererID=0;

        shader.Unbind();
    }

    void OpenGLCubeTexture::BindILS(uint32_t level, uint32_t index, uint32_t access) const
    {
        glBindImageTexture(index, m_RendererID, level, GL_TRUE, 0, access, m_InternalFormat);
    }

    OpenGLCubeTexture::OpenGLCubeTexture(TextureSpecification Spec)
    {
        uint target=0;
        if(Spec.hdr)
        {
            target=GL_RGBA16F;
        }
        else
        {
            target=GL_RGBA;
        }
        m_InternalFormat=target;
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureStorage2D(m_RendererID,
                           Spec.size,
                           target,
                           Spec.width, Spec.height);

    }

    OpenGLCubeTexture::~OpenGLCubeTexture()
    {
        glDeleteTextures(1,&m_RendererID);

    }


}
