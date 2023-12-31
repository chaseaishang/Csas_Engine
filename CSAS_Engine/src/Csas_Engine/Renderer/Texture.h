//
// Created by chaseaishang on 23-6-24.
//
#pragma once

#include <string>

#include "Csas_Engine/Core/Core.h"

namespace CsasEngine {

    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual uint32_t GetRendererID() const = 0;
        virtual void SetData(void* data, uint32_t size) = 0;

        virtual bool operator==(const Texture& other) const = 0;
        virtual void Bind(uint32_t slot = 0) const = 0;
        virtual void BindILS(uint32_t level, uint32_t index, uint32_t access)const = 0;
        virtual void UnBindILS(uint32_t index)const = 0;
    };
    struct TextureSpecification
    {
        enum class Format
        {
            RGBA,
            RGBA16F,
            DEPTH
        };
        Format format=Format::RGBA16F;
        bool   hdr=true;
        uint  size=1;
        uint32_t width;
        uint32_t height;
    };

    class Texture2D : public Texture
    {
    public:
        static Ref<Texture2D> Create(TextureSpecification Spec=TextureSpecification());
        static Ref<Texture2D> Create(const std::string& path,TextureSpecification Spec=TextureSpecification());



    };
    class CubeTexture:public Texture
    {
    public:
        virtual std::tuple<Ref<CubeTexture>,Ref<CubeTexture>,Ref<Texture2D>> PreComputeIBL()=0;
        static Ref<CubeTexture> Create(const std::string& path,TextureSpecification Spec=TextureSpecification());
        static Ref<CubeTexture> Create( TextureSpecification Spec=TextureSpecification());
        // OpenGLCubeTexture(TextureSpecification Spec);
    };



}
