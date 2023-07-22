//
// Created by chaseaishang on 23-7-22.
//

#pragma once


#include "Transform.h"
namespace CsasEngine
{
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;
    class Shader;
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec3 Normal;
        glm::vec2 UV;
    };
    enum class Primitive {None,Cube,Sphere};
    struct MeshComponent
    {
    public:
        friend class Renderer3D;
        enum Primitive m_primitive{Primitive::None};
        TransformComponent transform{glm::vec3(0)};
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;
        void Update();
        MeshComponent()=default;
        MeshComponent(Primitive primitive=Primitive::None);
        ~MeshComponent()=default;

    private:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer>m_VBO;
        Ref<IndexBuffer>m_IBO;
        Ref<Shader>m_Shader;
    private:
        void CreatCube(float size=1.0);
        void CreatSphere(float radius = 1.0f);
        void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

    };
}
