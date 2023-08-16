//
// Created by chaseaishang on 23-7-22.
//
#include "Mesh.h"
#include "Csas_Engine/Renderer/VertexArray.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
namespace CsasEngine
{



    namespace SphereSpec
    {
        static uint8_t SphereVertexSize=sizeof(Vertex);
        constexpr float PI = glm::pi<float>();
        constexpr float PI_2 = glm::half_pi<float>();
        constexpr unsigned int n_rows  = 100;
        constexpr unsigned int n_cols  = 100;
        constexpr unsigned int n_verts = (n_rows + 1) * (n_cols + 1);
        constexpr unsigned int n_tris  = n_rows * n_cols * 2;
        static const uint32_t OneSphereVertices =  n_verts ;
        static const uint32_t OneSphereIndices  =  n_tris * 3;
    }
    MeshComponent::MeshComponent(Primitive primitive,uint8_t RenderIndex)
    {
        m_primitive=primitive;
        this->RenderIndex=RenderIndex;
        switch (primitive)
        {
            case Primitive::Cube:CreatCube(1);break;
            case Primitive::Sphere:CreatSphere(0.5f);break;
            case Primitive::Quad:CreatQuad(1.0);break;
            case Primitive::Torus:CreatTorus(0.5f,0.17);break;
            case Primitive::Plane:CreatPlane(1.0);break;
            case Primitive::None: CSAS_ASSERT(false,"error primitive!");
        }
    }
    MeshComponent::MeshComponent(std::vector<Vertex> &vertices, std::vector<uint32_t> &indices,BufferLayout&layout,uint8_t RenderIndex)
    {
        m_primitive=Primitive::FromModel;
        this->RenderIndex=RenderIndex;
        CreateBuffers(vertices,indices,layout);
    }
    void MeshComponent::CreatSphere(float radius)
    {

        constexpr float PI = SphereSpec::PI;
        constexpr float PI_2 = SphereSpec::PI_2;

        // default LOD = 100x100 mesh grid size
        unsigned int n_rows  = SphereSpec::n_rows ;
        unsigned int n_cols  = SphereSpec::n_cols ;
        unsigned int n_verts = SphereSpec::n_verts;
        unsigned int n_tris  = SphereSpec::n_tris ;

        std::vector<Vertex>& vertices=m_vertices;
        std::vector<uint32_t> &indices=m_indices;
        vertices.reserve(n_verts);
        indices.reserve(n_tris * 3);

        for (unsigned int col = 0; col <= n_cols; ++col)
        {
            for (unsigned int row = 0; row <= n_rows; ++row)
            {
                // unscaled uv coordinates ~ [0, 1]
                float u = static_cast<float>(col) / n_cols;  //fix
                float v = static_cast<float>(row) / n_rows;

                float theta = PI * v - PI_2;  // ~ [-PI/2, PI/2], latitude from south to north pole
                float phi = PI * 2 * u;       // ~ [0, 2PI], longitude around the equator circle   fix

                float x = cos(phi) * cos(theta);
                float y = sin(theta);
                float z = sin(phi) * cos(theta) * (-1);

                // for a unit sphere centered at the origin, normal = position
                // binormal is normal rotated by 90 degrees along the latitude (+theta)

                Vertex vertex {};
                vertex.Position = glm::vec3(x, y, z) * radius;
                vertex.Normal   = glm::vec3(x, y, z);
                vertex.UV       = glm::vec2(u, v);


                vertices.push_back(vertex);
            }
        }

        for (unsigned int col = 0; col < n_cols; ++col) {
            for (unsigned int row = 0; row < n_rows; ++row) {
                auto index = col * (n_rows + 1);

                // counter-clockwise winding order
                indices.push_back(index + row + 1);//1
                indices.push_back(index + row);// 0
                indices.push_back(index + row + 1 + n_rows);//101

                // counter-clockwise winding order
                indices.push_back(index + row + 1 + n_rows + 1);//102
                indices.push_back(index + row + 1);   //1
                indices.push_back(index + row + 1 + n_rows);//101
            }
        }
        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Normal"},
                        {ShaderDataType::Float2, "a_UV"}
                };
        CreateBuffers(vertices,indices,layout);
    }
    namespace CubeSpec
    {
        static uint8_t CubeVertexSize=sizeof(Vertex);
        static const uint32_t OneCubeVertices =  24 ;
        static const uint32_t OneCubeIndices  =  36;

    }
    void MeshComponent::CreatCube(float size)
    {

        constexpr int n_vertices = CubeSpec::OneCubeVertices;  // we only need 24 vertices to triangulate the 6 faces
        constexpr int stride = sizeof(Vertex)/4;  // 3 +3 + 2
        auto& vertices=m_vertices;
        vertices.reserve(n_vertices);

        static const float data[] =
                {
                        // ----position----                             -------normal------                ----uv----
                        -0.5f, -0.5f, -0.5f,         +0.0f, -1.0f, +0.0f,             0.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,         +0.0f, -1.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, -0.5f, +0.5f,         +0.0f, -1.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, -0.5f,         +0.0f, -1.0f, +0.0f,       1.0f, 0.0f,
                        -0.5f, +0.5f, -0.5f,         +0.0f, +1.0f, +0.0f,       1.0f, 0.0f,
                        -0.5f, +0.5f, +0.5f,         +0.0f, +1.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,         +0.0f, +1.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,         +0.0f, +1.0f, +0.0f,       0.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,         +0.0f, +0.0f, -1.0f,       0.0f, 0.0f,
                        -0.5f, +0.5f, -0.5f,         +0.0f, +0.0f, -1.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,         +0.0f, +0.0f, -1.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, -0.5f,         +0.0f, +0.0f, -1.0f,       1.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,         +0.0f, +0.0f, +1.0f,       0.0f, 0.0f,
                        -0.5f, +0.5f, +0.5f,         +0.0f, +0.0f, +1.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,         +0.0f, +0.0f, +1.0f,       1.0f, 1.0f,
                        +0.5f, -0.5f, +0.5f,         +0.0f, +0.0f, +1.0f,       1.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,         -1.0f, +0.0f, +0.0f,       0.0f, 0.0f,
                        -0.5f, -0.5f, +0.5f,         -1.0f, +0.0f, +0.0f,       0.0f, 1.0f,
                        -0.5f, +0.5f, +0.5f,         -1.0f, +0.0f, +0.0f,       1.0f, 1.0f,
                        -0.5f, +0.5f, -0.5f,         -1.0f, +0.0f, +0.0f,       1.0f, 0.0f,
                        +0.5f, -0.5f, -0.5f,         +1.0f, +0.0f, +0.0f,       0.0f, 0.0f,
                        +0.5f, -0.5f, +0.5f,         +1.0f, +0.0f, +0.0f,       0.0f, 1.0f,
                        +0.5f, +0.5f, +0.5f,         +1.0f, +0.0f, +0.0f,       1.0f, 1.0f,
                        +0.5f, +0.5f, -0.5f,         +1.0f, +0.0f, +0.0f,       1.0f, 0.0f
                };
        for (unsigned int i = 0; i < n_vertices; i++) {
            unsigned int offset = i * stride;

            Vertex vertex {};
            vertex.Position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]) * size;
            vertex.Normal    =glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
            vertex.UV        =glm::vec2(data[offset + 6],data[offset + 7]);
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

        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Normal"},
                        {ShaderDataType::Float2, "a_UV"}
                };
        CreateBuffers(vertices,indices,layout);

    }

    void MeshComponent::CreateBuffers(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices,BufferLayout &layout)
    {
        m_VAO=VertexArray::Create();

        m_VBO=VertexBuffer::Create(vertices.size()*sizeof(Vertex));
        m_VBO->SetLayout(layout);
        m_VAO->AddVertexBuffer(m_VBO);
        m_VBO->SetData(vertices.data(),sizeof(Vertex)*vertices.size());
        m_IBO=IndexBuffer::Create((uint32_t*)indices.data(),indices.size());
        m_VAO->SetIndexBuffer(m_IBO);
    }

    void MeshComponent::Update()
    {
        m_VBO->SetData(m_vertices.data(),sizeof(Vertex)*m_vertices.size());

    }
    namespace QuadSpec
    {
        static uint8_t QuadVertexSize=sizeof(Vertex);
        static const uint32_t OneQuadVertices =  4 ;
        static const uint32_t OneQuadIndices  =  6;

    }
    void MeshComponent::CreatQuad(float size)
    {
        constexpr int n_vertices = QuadSpec::OneQuadVertices;
        constexpr int stride = 4;  // 2 + 2

        static const float data[] = {
                // position        uv
                -1.0f, -1.0f,  0.0f, 0.0f,
                +1.0f, -1.0f,  1.0f, 0.0f,
                +1.0f, +1.0f,  1.0f, 1.0f,
                -1.0f, +1.0f,  0.0f, 1.0f
        };

        std::vector<Vertex>& vertices=m_vertices;
        vertices.reserve(n_vertices);

        for (unsigned int i = 0; i < n_vertices; i++)
        {
            unsigned int offset = i * stride;
            Vertex vertex {};
            vertex.Position = glm::vec3(data[offset + 0], data[offset + 1], 0.0f) * size;
            vertex.Normal =glm::vec3(0,0,1);
            vertex.UV       = glm::vec2(data[offset + 2], data[offset + 3]);
            vertices.push_back(vertex);
        }

        // counter-clockwise winding order
        std::vector<uint32_t> &indices=m_indices;
        indices.reserve(QuadSpec::OneQuadIndices);
        indices={ 0, 1, 2, 2, 3, 0 };
        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Normal"},
                        {ShaderDataType::Float2, "a_UV"}
                };
        CreateBuffers(vertices, indices,layout);
    }


    void MeshComponent::CreatTorus(float R, float r)
    {

        // default LOD = 60x60 faces, step size = 6 degrees
        uint8_t  n_rings = 60;
        uint8_t  n_sides = 60;
        uint8_t  n_faces = n_sides * n_rings;  // quad face (2 triangles)
        uint8_t  n_verts = n_sides * n_rings + n_sides;

        float delta_phi   = glm::two_pi<float>() / n_rings;
        float delta_theta = glm::two_pi<float>() / n_sides;

        std::vector<Vertex> &vertices=m_vertices;
        vertices.reserve(n_verts);

        for (int ring = 0; ring <= n_rings; ring++) {
            float phi = ring * delta_phi;
            float cos_phi = cos(phi);
            float sin_phi = sin(phi);

            for (int side = 0; side < n_sides; side++) {
                float theta = side * delta_theta;
                float cos_theta = cos(theta);
                float sin_theta = sin(theta);

                float d = (R + r * cos_theta);  // distance from the vertex to the torus center

                float x = d * cos_phi;
                float y = d * sin_phi;
                float z = r * sin_theta;

                float a = d * cos_theta * cos_phi;
                float b = d * cos_theta * sin_phi;
                float c = d * sin_theta;

                float u = glm::one_over_two_pi<float>() * phi;
                float v = glm::one_over_two_pi<float>() * theta;

                Vertex vertex {};
                vertex.Position = glm::vec3(x, y, z);
                vertex.Normal   = normalize(glm::vec3(a, b, c));
                vertex.UV       = glm::vec2(u, v);

                vertices.push_back(vertex);
            }
        }

        std::vector<uint32_t> &indices=m_indices;
        indices.reserve(n_faces * 6);

        for (int ring = 0; ring < n_rings; ring++) {
            int offset = n_sides * ring;

            for (int side = 0; side < n_sides; side++) {
                int next_side = (side + 1) % n_sides;

                indices.push_back(offset + side);
                indices.push_back(offset + n_sides + side);
                indices.push_back(offset + n_sides + next_side);

                indices.push_back(offset + side);
                indices.push_back(offset + next_side + n_sides);
                indices.push_back(offset + next_side);
            }
        }


        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Normal"},
                        {ShaderDataType::Float2, "a_UV"}
                };
        CreateBuffers(vertices,indices,layout);

    }
    namespace PlaneSpec
    {
        static uint8_t PlaneVertexSize=sizeof(Vertex);
        static const uint32_t OnePlaneVertices =  8 ;
        static const uint32_t OnePlaneIndices  =  6;

    }
    void MeshComponent::CreatPlane(float size) {

        std::vector<Vertex> &vertices=m_vertices;
        vertices.reserve(PlaneSpec::OnePlaneVertices);

        const static int stride = 8;  // 3 + 3 + 2

        const static float data[] = {
                // ---position----    ------normal-----    ----uv----
                -1.0f, 0.0f, +1.0f,   0.0f, +1.0f, 0.0f,   0.0f, 0.0f,
                +1.0f, 0.0f, +1.0f,   0.0f, +1.0f, 0.0f,   1.0f, 0.0f,
                +1.0f, 0.0f, -1.0f,   0.0f, +1.0f, 0.0f,   1.0f, 1.0f,
                -1.0f, 0.0f, -1.0f,   0.0f, +1.0f, 0.0f,   0.0f, 1.0f,
                -1.0f, 0.0f, +1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
                +1.0f, 0.0f, +1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
                +1.0f, 0.0f, -1.0f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
                -1.0f, 0.0f, -1.0f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f
        };
        for (unsigned int i = 0; i < PlaneSpec::OnePlaneVertices; i++) {
            unsigned int offset = i * stride;

            Vertex vertex {};
            vertex.Position  = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]) * size;
            vertex.Normal    = glm::vec3(data[offset + 3], data[offset + 4], data[offset + 5]);
            vertex.UV        = glm::vec2(data[offset + 6], data[offset + 7]);  // keep in [0, 1] range
            vertices.push_back(vertex);
        }


        std::vector<uint32_t> &indices=m_indices;
        // counter-clockwise winding order
        indices = { 0, 1, 2, 2, 3, 0, 6, 5, 4, 4, 7, 6 };





        BufferLayout layout=
                {
                        {ShaderDataType::Float3, "a_Position"},
                        {ShaderDataType::Float3, "a_Normal"},
                        {ShaderDataType::Float2, "a_UV"}
                };
        CreateBuffers(vertices,indices,layout);

    }

    MeshComponent::MeshComponent(const MeshComponent &mesh)
    {
        this->m_VAO=mesh.m_VAO;
        this->m_VBO=mesh.m_VBO;
        this->m_IBO=mesh.m_IBO;
        m_primitive=mesh.m_primitive;
        transform=mesh.transform;
        m_vertices=mesh.m_vertices;
        m_indices=mesh.m_indices;
        this->RenderIndex=mesh.RenderIndex;
    }


}