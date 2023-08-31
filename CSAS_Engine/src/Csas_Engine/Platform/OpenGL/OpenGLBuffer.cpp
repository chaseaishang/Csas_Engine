//
// Created by chaseaishang on 23-6-18.
//
#include "Csas_Engine/Csaspch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace CsasEngine {
    /////////////////////////////////////////////////////////////////////////////
    // VertexBuffer /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
    {
        CSAS_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        //glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        CSAS_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);
        //glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glNamedBufferData(m_RendererID, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        CSAS_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        CSAS_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        CSAS_PROFILE_FUNCTION();
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
    {
        glNamedBufferSubData(this->m_RendererID,0,size,data);
    }


    /////////////////////////////////////////////////////////////////////////////
    // IndexBuffer //////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
            : m_Count(count)
    {
        CSAS_PROFILE_FUNCTION();

        glCreateBuffers(1, &m_RendererID);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        //glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glNamedBufferData(m_RendererID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        CSAS_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        CSAS_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        CSAS_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
///////////////////////
///////////////////////     OpenGLShaderStorageBuffer             //////////////////////////////////
///////////////////////
    OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(uint32_t size,AccessModel access)
    {
        uint flag;
//GL_DYNAMIC_STORAGE_BIT
        glCreateBuffers(1,&m_RendererID);
        target=GL_SHADER_STORAGE_BUFFER;
        glNamedBufferStorage(m_RendererID, size, nullptr, GL_DYNAMIC_STORAGE_BIT); // TODO: investigate usage hint
        //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, m_RendererID);

    }

    OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
    {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLShaderStorageBuffer::Bind(uint index) const
    {
        glBindBufferBase(target,index,m_RendererID);
    }

    void OpenGLShaderStorageBuffer::BindForCShader() const
    {
        uint target_=GL_DISPATCH_INDIRECT_BUFFER;
        glBindBuffer(target_,m_RendererID);
    }

    void OpenGLShaderStorageBuffer::Get_Data(uint size, uint offset, void *data) const
    {
        glGetNamedBufferSubData(m_RendererID,offset,size,data);
    }
}
