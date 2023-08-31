//
// Created by chaseaishang on 23-6-18.
//

#pragma once

#include "Csas_Engine/Renderer/Buffer.h"

namespace CsasEngine {

    class OpenGLVertexBuffer : public VertexBuffer
    {
    public:
        OpenGLVertexBuffer(float* vertices, uint32_t size);
        OpenGLVertexBuffer( uint32_t size);
        virtual ~OpenGLVertexBuffer();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void SetData(const void* data, uint32_t size) override;
        virtual uint32_t GetRenderID()const{return m_RendererID;}
        virtual const BufferLayout& GetLayout() const override { return m_Layout; }
        virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
    private:
        uint32_t m_RendererID;
        BufferLayout m_Layout;
    };

    class OpenGLIndexBuffer : public IndexBuffer
    {
    public:
        OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();
        virtual uint32_t GetRenderID()const{return m_RendererID;}
        virtual void Bind() const;
        virtual void Unbind() const;

        virtual uint32_t GetCount() const { return m_Count; }
    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };
    class OpenGLShaderStorageBuffer:public ShaderStorageBuffer
    {
    public:
        OpenGLShaderStorageBuffer(uint32_t size,AccessModel access);
        virtual ~OpenGLShaderStorageBuffer();
        void Bind(uint index) const override;
        void Get_Data(uint size,uint offset,void*data) const override;


        void BindForCShader() const override;
        uint32_t GetRenderID()const{return m_RendererID;}
    private:
        uint32_t m_RendererID;
        uint32_t m_Index;
        uint target;
    };



}