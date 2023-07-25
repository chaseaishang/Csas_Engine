//
// Created by chaseaishang on 23-7-15.
//
#include "Csas_Engine/Csaspch.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
//Temp TODO remove
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/gtx/string_cast.hpp"
namespace CsasEngine {
    static bool Debug= false;

    struct CubeVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec3 Normal;
        glm::vec2 UV;
    };
    namespace CameraSpec
    {
        glm::mat4 ViewProjMatrix[2];
    }
    namespace SphereSpec
    {
        static uint8_t SphereVertexSize=sizeof(CubeVertex);
        constexpr float PI = glm::pi<float>();
        constexpr float PI_2 = glm::half_pi<float>();
        constexpr unsigned int n_rows  = 100;
        constexpr unsigned int n_cols  = 100;
        constexpr unsigned int n_verts = (n_rows + 1) * (n_cols + 1);
        constexpr unsigned int n_tris  = n_rows * n_cols * 2;
        static const uint32_t MaxSphere = 10;
        static const uint32_t MaxVertices = MaxSphere* n_verts ;
        static const uint32_t MaxIndices  =  MaxSphere * n_tris * 3;

        static const uint32_t OneSphereVertices =  n_verts ;
        static const uint32_t OneSphereIndices  =  n_tris * 3;
    }
    namespace CubeSpec
    {
        static uint8_t CubeVertexSize=sizeof(CubeVertex);
        static const uint32_t MaxCube = 10;
        static const uint32_t OneCubeVertices =  24 ;
        static const uint32_t OneCubeIndices  =  36;
        static const uint32_t MaxVertices =  MaxCube * OneCubeVertices;
        static const uint32_t MaxIndices  =  MaxCube * OneCubeIndices;
    }

    static void CreatCube(std::vector<CubeVertex>&VBO,std::vector<uint32_t>&IBO)
    {
        constexpr int n_vertices = CubeSpec::OneCubeVertices;  // we only need 24 vertices to triangulate the 6 faces
        constexpr int stride = sizeof(CubeVertex)/4;  // 3 + 4+3 + 2
        auto& vertices=VBO;
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
        float size=1;
        for (unsigned int i = 0; i < n_vertices; i++) {
            unsigned int offset = i * stride;

            CubeVertex vertex {};
            vertex.Position = glm::vec3(data[offset + 0], data[offset + 1], data[offset + 2]) * size;
            vertex.Color     =glm::vec4(data[offset + 3], data[offset + 4], data[offset + 5],data[offset + 6]);
            vertex.Normal    =glm::vec3(data[offset + 7], data[offset + 8], data[offset + 9]);
            vertex.UV        =glm::vec2(data[offset + 10],data[offset + 11]);
            vertices.push_back(vertex);
        }

        // counter-clockwise winding order
        std::vector<uint32_t>& indices =IBO;
        indices.reserve(36);
        indices={
                +0, +2, +1,   +0, +3, +2,
                +4, +5, +6,   +4, +6, +7,
                +8, +9, 10,   +8, 10, 11,

                12, 15, 14,   12, 14, 13,
                16, 17, 18,   16, 18, 19,
                20, 23, 22,   20, 22, 21
                };


    }
    static void  CreatSphere(std::vector<CubeVertex>&VBO,std::vector<uint32_t>&IBO)
    {
        float radius=0.5f;
        constexpr float PI = SphereSpec::PI;
        constexpr float PI_2 = SphereSpec::PI_2;

        // default LOD = 100x100 mesh grid size
        unsigned int n_rows  = SphereSpec::n_rows ;
        unsigned int n_cols  = SphereSpec::n_cols ;
        unsigned int n_verts = SphereSpec::n_verts;
        unsigned int n_tris  = SphereSpec::n_tris ;

        std::vector<CubeVertex>& vertices=VBO;
        std::vector<uint32_t> &indices=IBO;
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

                CubeVertex vertex {};
                vertex.Position = glm::vec3(x, y, z) * radius;
                vertex.Color    = glm::vec4(1.0f);
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

    }

    static glm::vec4 Cubecolor[]=
    {
            {0.971f,  0.572f,  0.833f,1.0},
            {0.359f,  0.583f,  0.152f,1.0},
            {0.393f,  0.621f,  0.362f,1.0},
            {0.014f,  0.184f,  0.576f,1.0},
            {0.583f,  0.771f,  0.014f,1.0},
            {0.543f,  0.021f,  0.978f,1.0},
            {0.435f,  0.602f,  0.223f,1.0},
            {0.055f,  0.953f,  0.042f,1.0},
            {0.971f,  0.572f,  0.833f,1.0},
            {0.359f,  0.583f,  0.152f,1.0},
            {0.393f,  0.621f,  0.362f,1.0},
            {0.014f,  0.184f,  0.576f,1.0},
            {0.583f,  0.771f,  0.014f,1.0},
            {0.543f,  0.021f,  0.978f,1.0},
            {0.435f,  0.602f,  0.223f,1.0},
            {0.055f,  0.953f,  0.042f,1.0},
            {0.971f,  0.572f,  0.833f,1.0},
            {0.359f,  0.583f,  0.152f,1.0},
            {0.393f,  0.621f,  0.362f,1.0},
            {0.014f,  0.184f,  0.576f,1.0},
            {0.583f,  0.771f,  0.014f,1.0},
            {0.543f,  0.021f,  0.978f,1.0},
            {0.435f,  0.602f,  0.223f,1.0},
            {0.055f,  0.953f,  0.042f,1.0}
    };

        struct Renderer3DData
        {
             Ref<UniformBuffer> CameraUBO;

             Ref <VertexArray> CubeVertexArray;
             Ref <VertexBuffer> CubeVertexBuffer;
             Ref <Shader> CubeShader;
             CubeVertex *CubeVertexBufferBase = nullptr;
             CubeVertex *CubeVertexBufferPtr = nullptr;
             std::vector<CubeVertex> CubeVBO;
             std::vector<uint32_t> CubeIBO;
             glm::vec4 CubeVertexPositions[24];
             Renderer3D::Statistics Stats;
             uint32_t CubeIndexCount = 0;

            //Sphere
            Ref <VertexArray> SphereVertexArray;
            Ref <VertexBuffer> SphereVertexBuffer;
            Ref <Shader> SphereShader;
            CubeVertex *SphereVertexBufferBase = nullptr;
            CubeVertex *SphereVertexBufferPtr = nullptr;
            std::vector<CubeVertex> SphereVBO;
            std::vector<uint32_t> SphereIBO;
            glm::vec4 SphereVertexPositions[SphereSpec::OneSphereVertices];
            uint32_t SphereIndexCount = 0;
        };
        static Renderer3DData *s_Data;

    void Renderer3D::Init()
    {//float* vertices, uint32_t size

        s_Data=new Renderer3DData;
        //Cube Init Begin==================================================================================================
        {
            s_Data->CubeVertexArray = VertexArray::Create();

            std::vector<CubeVertex> &VBO = s_Data->CubeVBO;
            std::vector<uint32_t>&IBO=s_Data->CubeIBO;

            CreatCube(VBO, IBO);
            s_Data->CubeShader = Shader::Create("assets/shaders/Cube.glsl");

            s_Data->CubeVertexBuffer = VertexBuffer::Create(CubeSpec::MaxVertices * sizeof(CubeVertex));
            s_Data->CubeVertexBuffer->SetLayout({
                                                        {ShaderDataType::Float3, "a_Position"},
                                                        {ShaderDataType::Float4, "a_Color"},
                                                        {ShaderDataType::Float3, "a_Normal"},
                                                        {ShaderDataType::Float2, "a_UV"}
                                                });
            uint8_t CubeLayoutSize=                 (3+4+3+2                 )*4;
            if(CubeLayoutSize!=CubeSpec::CubeVertexSize)
                CSAS_CORE_WARN("CubeLayoutSize!=CubeSpec::CubeVertexSize");
            s_Data->CubeVertexBufferBase = new CubeVertex[CubeSpec::MaxVertices];
            s_Data->CubeVertexArray->AddVertexBuffer(s_Data->CubeVertexBuffer);
            //Init cube Vertex data
            for (int i = 0; i < CubeSpec::OneCubeVertices; i++)
                s_Data->CubeVertexPositions[i] = {VBO[i].Position, 1.0f};

            uint32_t *CubeIndices = new uint32_t[CubeSpec::MaxIndices];
            //IBO 36
            int CubeIndex = 0;
            for (int i = 0; i < CubeSpec::MaxIndices; i += CubeSpec::OneCubeIndices) {
                for (int offset = 0; offset < CubeSpec::OneCubeIndices; offset++) {
                    CubeIndices[i + offset] =
                            IBO[offset] + CubeIndex * CubeSpec::OneCubeVertices;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                CubeIndex++;

            }


            Ref <IndexBuffer> CubeIB = IndexBuffer::Create(CubeIndices, CubeSpec::MaxIndices);
            s_Data->CubeVertexArray->SetIndexBuffer(CubeIB);
            delete CubeIndices;

        }
        //Sphere Init Begin==================================================================================================
        {
            s_Data->SphereVertexArray = VertexArray::Create();

            std::vector<CubeVertex> &VBO = s_Data->SphereVBO;
            std::vector<uint32_t>&IBO=s_Data->SphereIBO;

            CreatSphere(VBO, IBO);
            s_Data->SphereShader = Shader::Create("assets/shaders/Cube.glsl");

            s_Data->SphereVertexBuffer = VertexBuffer::Create(SphereSpec::MaxVertices * sizeof(CubeVertex));
            s_Data->SphereVertexBuffer->SetLayout({
                                                        {ShaderDataType::Float3, "a_Position"},
                                                        {ShaderDataType::Float4, "a_Color"},
                                                        {ShaderDataType::Float3, "a_Normal"},
                                                        {ShaderDataType::Float2, "a_UV"}
                                                });
            uint8_t SphereLayoutSize=                 (3+4+3+2                 )*4;
            if(SphereLayoutSize!=SphereSpec::SphereVertexSize)
                CSAS_CORE_WARN("SphereLayoutSize!=SphereSpec::SphereVertexSize");
            s_Data->SphereVertexBufferBase = new CubeVertex[SphereSpec::MaxVertices];
            s_Data->SphereVertexArray->AddVertexBuffer(s_Data->SphereVertexBuffer);
            //Init cube Vertex data
            for (int i = 0; i < SphereSpec::OneSphereVertices; i++)
                s_Data->SphereVertexPositions[i] = {VBO[i].Position, 1.0f};

            uint32_t *SphereIndices = new uint32_t[SphereSpec::MaxIndices];
            //IBO 36
            int SphereIndex = 0;
            for (int i = 0; i < SphereSpec::MaxIndices; i += SphereSpec::OneSphereIndices) {
                for (int offset = 0; offset < SphereSpec::OneSphereIndices; offset++) {
                    SphereIndices[i + offset] =
                            IBO[offset] + SphereIndex * SphereSpec::OneSphereVertices;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                }
                SphereIndex++;

            }


            Ref <IndexBuffer> SphereIB = IndexBuffer::Create(SphereIndices, SphereSpec::MaxIndices);
            s_Data->SphereVertexArray->SetIndexBuffer(SphereIB);
            delete SphereIndices;

        }
        s_Data->CameraUBO=UniformBuffer::Create(sizeof(CameraSpec::ViewProjMatrix),0);
        //

    }

    void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 &transform)
    {
        glm::mat4 viewProj = camera.GetViewProjection();

        s_Data->CubeShader->Bind();


        s_Data->CubeShader->SetMat4("u_ViewProjection", viewProj);
        s_Data->SphereShader->Bind();
        s_Data->SphereShader->SetMat4("u_ViewProjection", viewProj);

        s_Data->CubeShader->Unbind();
        //2.2  0    0   0
        //0    2.4  0   0
        //0    0    -1.0  0
        //0    0    0   -0.02
        //TODO Remove

        s_Data->CubeIndexCount = 0;
        s_Data->CubeVertexBufferPtr = s_Data->CubeVertexBufferBase;
        s_Data->SphereIndexCount=0;
        s_Data->SphereVertexBufferPtr = s_Data->SphereVertexBufferBase;
    }

    void Renderer3D::EndScene()
    {
        Flush();
    }
    void Renderer3D::DrawSphere(const glm::mat4 &transform, const glm::vec4 &tintColor)
    {
        for(int i=0;i<SphereSpec::OneSphereVertices;i++)
        {
            s_Data->SphereVertexBufferPtr->Position=transform*s_Data->SphereVertexPositions[i];
            s_Data->SphereVertexBufferPtr->Color=glm::vec4({0.971f,  0.572f,  0.833f,1.0});
            s_Data->SphereVertexBufferPtr->Normal=s_Data->SphereVBO[i].Normal;
            s_Data->SphereVertexBufferPtr->UV=s_Data->SphereVBO[i].UV;
            s_Data->SphereVertexBufferPtr++;
        }
        s_Data->SphereIndexCount += SphereSpec::OneSphereIndices;
        s_Data->Stats.SphereCount++;

    }
    void Renderer3D::DrawCube(const glm::mat4 &transform,const glm::vec4 &tintColor )
    {
        //update data

        for(int i=0;i<CubeSpec::OneCubeVertices;i++)
        {//                               4X4             1X4
            s_Data->CubeVertexBufferPtr->Position=transform*s_Data->CubeVertexPositions[i];
            s_Data->CubeVertexBufferPtr->Color=Cubecolor[i];
            s_Data->CubeVertexBufferPtr->Normal=s_Data->CubeVBO[i].Normal;
            s_Data->CubeVertexBufferPtr->UV=s_Data->CubeVBO[i].UV;
            s_Data->CubeVertexBufferPtr++;
        }

        s_Data->CubeIndexCount += 36;
        s_Data->Stats.CubeCount++;
        s_Data->Stats.z=(s_Data->CubeVertexBufferPtr-1)->Position.x;
        

    }
    void Renderer3D::Flush()
    {
        if(s_Data->CubeIndexCount)
        {
            s_Data->CubeVertexArray->Bind();
            s_Data->CubeShader->Bind();
            uint32_t dataSize=(uint32_t)((uint8_t*)(s_Data->CubeVertexBufferPtr) - (uint8_t*)(s_Data->CubeVertexBufferBase));
            //   dataSize   <==>   sizeof(CubeVertex)*static_cast<CubeVertex*>(BufferPtr-BufferBase)
            //uint32_t dataSize1=(uint32_t)((CubeVertex*)(s_Data->CubeVertexBufferPtr) - (CubeVertex*)(s_Data->CubeVertexBufferBase));
            s_Data->CubeVertexBuffer->SetData(s_Data->CubeVertexBufferBase,dataSize);
            RenderCommand::DrawIndexed(s_Data->CubeVertexArray, s_Data->CubeIndexCount);
            s_Data->Stats.DrawCalls++;
        }
        if(s_Data->SphereIndexCount)
        {
            s_Data->SphereVertexArray->Bind();
            s_Data->SphereShader->Bind();
            uint32_t dataSize=(uint32_t)((uint8_t*)(s_Data->SphereVertexBufferPtr) - (uint8_t*)(s_Data->SphereVertexBufferBase));
            s_Data->SphereVertexBuffer->SetData(s_Data->SphereVertexBufferBase,dataSize);
            RenderCommand::DrawIndexed(s_Data->SphereVertexArray, s_Data->SphereIndexCount);
            s_Data->Stats.DrawCalls++;
        }


    }
    Renderer3D::Statistics Renderer3D::GetStats()
    {
        return s_Data->Stats;;
    }

    void Renderer3D::ResetStats()
    {
        memset(&s_Data->Stats, 0, sizeof(Statistics));

    }

    void Renderer3D::Shutdown()
    {
        if(!Debug)
        {
            delete [] s_Data->CubeVertexBufferBase;
            delete [] s_Data->SphereVertexBufferBase;
            delete s_Data;
        }
        else
        {
            delete s_Data;
        }



    }

    void Renderer3D::DrawTestUniforBLock()
    {

    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera)
    {
        auto &vao=mesh.m_VAO;
        auto&shader=mesh.m_Shader;


       auto transform=mesh.transform.GetTransform();

        auto& viewProj=camera.GetViewProjection();

        mesh.Update();
        shader->Bind();
        switch (mesh.m_primitive)
        {
            case Primitive::Quad:
            {
                s_Data->CameraUBO->SetData(glm::value_ptr(viewProj),sizeof(CameraSpec::ViewProjMatrix));

                shader->SetMat4("model",transform);
                break;
            }
            case Primitive::Cube:
            {
                s_Data->CameraUBO->SetData(glm::value_ptr(viewProj),sizeof(CameraSpec::ViewProjMatrix));
                shader->SetMat4("model",transform);
                break;
            }

        }




        vao->Bind();
        RenderCommand::DrawIndexed(vao);

    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera, Material_BasePrimitive &material)
    {
        auto &vao=mesh.m_VAO;
        CameraSpec::ViewProjMatrix[0]=camera.GetView();
        CameraSpec::ViewProjMatrix[1]=camera.GetProjection();

        s_Data->CameraUBO->SetData(glm::value_ptr(CameraSpec::ViewProjMatrix[0]),sizeof(CameraSpec::ViewProjMatrix));
        auto transform=mesh.transform.GetTransform();



        mesh.Update();
        //material Update
        material.Update(transform);




        vao->Bind();
        RenderCommand::DrawIndexed(vao);
    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera, Material_BasePBR &material)
    {
        auto &vao=mesh.m_VAO;
        CameraSpec::ViewProjMatrix[0]=camera.GetView();
        CameraSpec::ViewProjMatrix[1]=camera.GetProjection();

        s_Data->CameraUBO->SetData(glm::value_ptr(CameraSpec::ViewProjMatrix[0]),sizeof(CameraSpec::ViewProjMatrix));
        auto transform=mesh.transform.GetTransform();


        mesh.Update();
        material.light.Position={5.0f,5.0f,2.0f};
        material.light.La={ 0.4f, 0.4f, 0.4f};
        material.light.Ld={1.0f, 1.0f, 1.0f};
        material.light.Ls={1.0f, 1.0f, 1.0f};



        //material Update
        material.Update(transform);

        vao->Bind();
        RenderCommand::DrawIndexed(vao);
    }


}