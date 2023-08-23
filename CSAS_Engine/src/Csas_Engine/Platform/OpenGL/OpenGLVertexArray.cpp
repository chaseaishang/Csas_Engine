//
// Created by chaseaishang on 23-6-18.
//

#include "Csas_Engine/Csaspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace CsasEngine {
    uint32_t OpenGLVertexArray::curr_bound_vertex_array=0;
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        CSAS_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        CSAS_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        CSAS_PROFILE_FUNCTION();
        this->Unbind();
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        CSAS_PROFILE_FUNCTION();
        if(curr_bound_vertex_array!=m_RendererID)
        {
            glBindVertexArray(m_RendererID);
            curr_bound_vertex_array=m_RendererID;
        }

    }

    void OpenGLVertexArray::Unbind() const
    {
        CSAS_PROFILE_FUNCTION();
        if (curr_bound_vertex_array == m_RendererID)
        {
            curr_bound_vertex_array = 0;
            glBindVertexArray(0);
        }

    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        CSAS_PROFILE_FUNCTION();

        CSAS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        auto vbo_ID=vertexBuffer->GetRenderID();
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexArrayAttrib(m_RendererID,m_VertexBufferIndex);

                    glVertexArrayAttribFormat(m_RendererID,
                                              m_VertexBufferIndex,
                                              element.GetComponentCount(),
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              element.Offset);

                    glVertexArrayAttribBinding(m_RendererID,
                                               m_VertexBufferIndex,m_BindingIndex);

//                    glVertexAttribPointer(m_VertexBufferIndex,
//                                          element.GetComponentCount(),
//                                          ShaderDataTypeToOpenGLBaseType(element.Type),
//                                          element.Normalized ? GL_TRUE : GL_FALSE,
//                                          layout.GetStride(),
//                                          (const void*)element.Offset);
                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    CSAS_CORE_ASSERT(true,"We didn't refactoring this haha!");
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexArrayAttrib(m_RendererID,m_VertexBufferIndex);

                        glVertexArrayVertexBuffer(m_RendererID,
                                                  m_VertexBufferIndex,
                                                  vbo_ID,
                                                  element.Offset+ sizeof(float) * count * i,
                                                  layout.GetStride());

                        glVertexArrayAttribFormat(m_RendererID,
                                                  m_VertexBufferIndex,  count,
                                                  ShaderDataTypeToOpenGLBaseType(element.Type),
                                                  element.Normalized ? GL_TRUE : GL_FALSE,
                                                  element.Offset + sizeof(float) * count * i);

                        glVertexArrayAttribBinding(m_RendererID,
                                                   m_VertexBufferIndex,m_VertexBufferIndex);

//                        glVertexAttribPointer(m_VertexBufferIndex,
//                                              count,
//                                              ShaderDataTypeToOpenGLBaseType(element.Type),
//                                              element.Normalized ? GL_TRUE : GL_FALSE,
//                                              layout.GetStride(),
//                                              (const void*)(element.Offset + sizeof(float) * count * i));
                        //glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
                default:
                    CSAS_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        glVertexArrayVertexBuffer(m_RendererID,
                                  m_BindingIndex,
                                  vbo_ID,
                                  0,
                                  layout.GetStride());

        m_VertexBuffers.push_back(vertexBuffer);
        m_BindingIndex++;
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        CSAS_PROFILE_FUNCTION();
        glVertexArrayElementBuffer(m_RendererID,indexBuffer->GetRenderID());


        m_IndexBuffer = indexBuffer;
    }



}
