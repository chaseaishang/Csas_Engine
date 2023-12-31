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
    class BufferLayout;
    class Shader;
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;
    };

    enum class Primitive {None,
            Cube,Sphere,Quad,Torus,FromModel,Plane,
            UnRender,
            Particle


    };
    //class VertexType

    struct MeshComponent
    {
    public:
        friend class Renderer3D;
        enum Primitive m_primitive{Primitive::None};
        uint8_t RenderIndex=0;
        TransformComponent transform{glm::vec3(0)};

    };

    struct MeshComponent_Vertex:public MeshComponent
    {
    public:
        friend class Renderer3D;

        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;

        void Update();
        MeshComponent_Vertex(const MeshComponent_Vertex&mesh);
        MeshComponent_Vertex()=default;
        MeshComponent_Vertex(Primitive primitive=Primitive::None,uint8_t RenderIndex=0);
        MeshComponent_Vertex(std::vector<Vertex> &vertices,std::vector<uint32_t> &indices,BufferLayout&layout,uint8_t RenderIndex);
        ~MeshComponent_Vertex()=default;

    public:
        Ref<VertexArray> m_VAO;
        Ref<VertexBuffer>m_VBO;
        Ref<IndexBuffer>m_IBO;
    private:

        void CreatTorus(float R = 0.5f, float r = 0.17f);
        void CreatCube(float size=1.0);
        void CreatPlane(float size=1.0);
        void CreatSphere(float radius = 0.5f);
        void CreatQuad(float size=1.0);
        void CreateBuffers(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices,BufferLayout &layout);

    };
}
