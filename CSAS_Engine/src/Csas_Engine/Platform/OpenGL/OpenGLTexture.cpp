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
    namespace Utils{
        using TexSpecFormat=TextureSpecification::Format;
        uint TextureSpec2Opengl(TexSpecFormat format)
        {
            switch (format)
            {
                case TexSpecFormat::RGBA:
                    return GL_RGBA;
                case TexSpecFormat::RGBA16F:
                    return GL_RGBA16F;
                case TexSpecFormat::DEPTH:
                    return GL_DEPTH24_STENCIL8;
                default: CSAS_CORE_ASSERT(true,"Error TexSpecFormat");
            }
            return 0;
        }
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
        m_textureSpecification.width=width;
        m_textureSpecification.height=height;
        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4)
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            m_textureSpecification.format=TextureSpecification::Format::RGBA;
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
        glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

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
        m_textureSpecification.width=width;
        m_textureSpecification.height=height;

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
        glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_FLOAT, data);

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
        m_InternalFormat=Utils::TextureSpec2Opengl(Spec.format);
        if(m_InternalFormat==GL_DEPTH24_STENCIL8)
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
            glTextureParameteri(m_RendererID, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glTextureStorage2D(m_RendererID,
                               Spec.size,
                               GL_DEPTH24_STENCIL8,
                               Spec.width, Spec.height);
            return;
        }
        uint target;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureStorage2D(m_RendererID,
                           Spec.size,
                           m_InternalFormat,
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
        auto&width=m_textureSpecification.width;
        auto&height=m_textureSpecification.height;
        CSAS_CORE_ASSERT(size == width * height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, data);
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

    void OpenGLTexture2D::UnBindILS(uint32_t index) const
    {
        // If texture is zero, then any existing binding to the image unit is broken.
        glBindImageTexture(index, 0, 0, GL_TRUE, 0, GL_READ_ONLY, m_InternalFormat);
    }


    void OpenGLCubeTexture::SetData(void *data, uint32_t size)
    {
        CSAS_ASSERT(true,"We don't support now");

    }

    void OpenGLCubeTexture::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot,m_RendererID);
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
        shader.SyncWait(GL_ALL_BARRIER_BITS);
        GlCheckError();
        m_RendererID=cubeTexture.m_RendererID;
        cubeTexture.m_RendererID=0;
        m_Height=512;
        m_Width=512;
        shader.Unbind();
    }

    void OpenGLCubeTexture::BindILS(uint32_t level, uint32_t index, uint32_t access) const
    {
        glBindImageTexture(index, m_RendererID, level, GL_TRUE, 0, access, m_InternalFormat);
    }
    void OpenGLCubeTexture::UnBindILS(uint32_t index) const
    {
        // If texture is zero, then any existing binding to the image unit is broken.
        glBindImageTexture(index, 0, 0, GL_TRUE, 0, GL_READ_ONLY, m_InternalFormat);
    }

    OpenGLCubeTexture::OpenGLCubeTexture(TextureSpecification Spec)
    {

        m_InternalFormat=Utils::TextureSpec2Opengl(Spec.format);
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        if(Spec.size==1)
        {
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else
        {
            glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }


        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTextureStorage2D(m_RendererID,
                           Spec.size,
                           m_InternalFormat,
                           Spec.width, Spec.height);
        m_Height=Spec.height;
        m_Width=Spec.width;
        if(Spec.size>1)
        {
            glGenerateTextureMipmap(m_RendererID);
            GlCheckError();
        }


    }

    OpenGLCubeTexture::~OpenGLCubeTexture()
    {
        glDeleteTextures(1,&m_RendererID);

    }



    std::tuple<Ref<CubeTexture>,Ref<CubeTexture>,Ref<Texture2D>> OpenGLCubeTexture::PreComputeIBL()
    {
        const uint irradiance_map_width=128;
        const uint prefiltered_width=512;
        const uint preBRDF_width=1024;
        Timer timer;
        GLClearError();
        TextureSpecification spec;
        spec.height=spec.width=irradiance_map_width;
        spec.hdr= true;
        spec.size=1;
        spec.format=TextureSpecification::Format::RGBA16F;
        Ref<OpenGLCubeTexture> irradiance_map= CreateRef<OpenGLCubeTexture>(spec);
        OpenGLShader irradiance_shader("./assets/shaders/utils/irradiance_map.glsl");

        spec.height=spec.width=prefiltered_width;
        spec.size=6;
        Ref<OpenGLCubeTexture> prefiltered_map= CreateRef<OpenGLCubeTexture>(spec);
        OpenGLShader prefiltered_shader("./assets/shaders/utils/prefilter_envmap.glsl");

        spec.height=spec.width=preBRDF_width;
        spec.size=1;
        Ref<OpenGLTexture2D> BRDF_LUT= CreateRef<OpenGLTexture2D>(spec);
        OpenGLShader precompute_BRDF_shader("./assets/shaders/utils/precompute_brdf.glsl");

        this->Bind(0);
        irradiance_map->BindILS(0,0,GL_WRITE_ONLY);
        if(irradiance_shader.Bind();true)
        {
            //cubeTexture
            irradiance_shader.Dispatch(irradiance_map_width/32,irradiance_map_width/32,6);
            irradiance_shader.SyncWait(GL_ALL_BARRIER_BITS);
            irradiance_map->UnBindILS(0);
            GlCheckError();
        }

        this->Bind(0);
        prefiltered_shader.Bind();
        GLuint resolution = prefiltered_width;
        uint max_level=6-1;
        for(int level=0;level<=max_level;level++,resolution/=2)
        {
            float roughness = level / static_cast<float>(max_level);
            uint n_groups = glm::max<uint>(resolution / 32, 1);

            prefiltered_map->BindILS(level, 1, GL_WRITE_ONLY);
            prefiltered_shader.SetFloat("roughness", roughness);
            prefiltered_shader.Dispatch(n_groups, n_groups, 6);
            prefiltered_shader.SyncWait(GL_ALL_BARRIER_BITS);

        }
        prefiltered_map->UnBindILS(1);
        GlCheckError();

        BRDF_LUT->BindILS(0, 2, GL_WRITE_ONLY);
        GlCheckError();
        if (precompute_BRDF_shader.Bind(); true) {
            precompute_BRDF_shader.Dispatch(preBRDF_width / 32, preBRDF_width / 32, 1);
            precompute_BRDF_shader.SyncWait(GL_ALL_BARRIER_BITS);
            BRDF_LUT->UnBindILS(2);
            GlCheckError();

        }
        precompute_BRDF_shader.Unbind();

        CSAS_CORE_WARN("IBL creation took {0} ms", timer.ElapsedMillis());
        return {irradiance_map,prefiltered_map,BRDF_LUT};
    }




}
