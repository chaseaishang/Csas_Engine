//
// Created by chaseaishang on 23-7-22.
//
#include "Mesh.h"
#include "Csas_Engine/Renderer/VertexArray.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "glm/glm.hpp"
namespace CsasEngine
{


    namespace CubeSpec
    {
        static uint8_t CubeVertexSize=sizeof(Vertex);
        static const uint32_t OneCubeVertices =  24 ;
        static const uint32_t OneCubeIndices  =  36;

    }
    MeshComponent::MeshComponent(Primitive primitive)
    {
        m_primitive=primitive;
        switch (primitive)
        {
            case Primitive::Cube:CreatCube(1);break;
            case Primitive::Sphere:CreatSphere(1.0f);break;
            case Primitive::None: CSAS_ASSERT(false,"error primitive!");
        }
    }

    void MeshComponent::CreatSphere(float radius)
    {

    }

    void MeshComponent::CreatCube(float size)
    {

        constexpr int n_vertices = CubeSpec::OneCubeVertices;  // we only need 24 vertices to triangulate the 6 faces
        constexpr int stride = sizeof(Vertex)/4;  // 3 + 4+3 + 2
        auto& vertices=m_vertices;
        vertices.reserve(n_vertices);

        static const float data[] =
                {
                        // ----position----                     ------color-----------                         -------normal------                ----uv----
                        -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, -1.0f, +0.0f,             0.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, -1.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, -1.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, -1.0f, +0.0f,       1.0f, 0.0f,
                        -0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +1.0f, +0.0f,       1.0f, 0.0f,
                        -0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +1.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +1.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +1.0f, +0.0f,       0.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, -1.0f,       0.0f, 0.0f,
                        -0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, -1.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, -1.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, -1.0f,       1.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, +1.0f,       0.0f, 0.0f,
                        -0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, +1.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, +1.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +0.0f, +0.0f, +1.0f,       1.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       -1.0f, +0.0f, +0.0f,       0.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       -1.0f, +0.0f, +0.0f,       0.0f, 1.0f,
                        -0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       -1.0f, +0.0f, +0.0f,       1.0f, 1.0f,
                        -0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       -1.0f, +0.0f, +0.0f,       1.0f, 0.0f,
                        +0.5f, -0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +1.0f, +0.0f, +0.0f,       0.0f, 0.0f,
                        +0.5f, -0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +1.0f, +0.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,   1.0f,1.0f,1.f,1.0f,       +1.0f, +0.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,   1.0f,1.0f,1.f,1.0f,       +1.0f, +0.0f, +0.0f,       1.0f, 0.0f
                };
        for (unsigned int i = 0; i < n_vertices; i++) {
            unsigned int offset = i * stride;

            Vertex vertex {};
            vertex.Position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]) * size;
            vertex.Color     =glm::vec4(data[offset + 3], data[offset + 4], data[offset + 5],data[offset + 6]);
            vertex.Normal    =glm::vec3(data[offset + 7], data[offset + 8], data[offset + 9]);
            vertex.UV        =glm::vec2(data[offset + 10],data[offset + 11]);
            vertices.push_back(vertex);
        }

        // counter-clockwise winding order
        auto& indices=m_indices;
        indices.reserve(CubeSpec::OneCubeIndices);
        indices={
                +0, +2, +1,   +0, +3, +2,
                +4, +5, +6,   +4, +6, +7,
                +8, +9, 10,   +8, 10, 11,

                12, 15, 14,   12, 14, 13,
                16, 17, 18,   16, 18, 19,
                20, 23, 22,   20, 22, 21
        };
        CreateBuffers(vertices,indices);

    }

    void MeshComponent::CreateBuffers(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices)
    {
        m_VAO=VertexArray::Create();
        m_VBO=VertexBuffer::Create(CubeSpec::CubeVertexSize*sizeof(Vertex));
        BufferLayout layout=
        {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_UV"}
        };
        m_VBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_VBO->SetData(vertices.data(),sizeof(Vertex)*vertices.size());
        m_IBO=IndexBuffer::Create((uint32_t*)indices.data(),indices.size());
        m_VAO->SetIndexBuffer(m_IBO);
        m_Shader=Shader::Create("./assets/shaders/BasePrimitive.glsl");

    }

    void MeshComponent::Update()
    {
        m_VBO->SetData(m_vertices.data(),sizeof(Vertex)*m_vertices.size());

    }


}