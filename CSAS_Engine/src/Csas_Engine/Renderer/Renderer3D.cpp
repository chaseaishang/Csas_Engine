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

namespace CsasEngine {
    struct CubeVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
    };
    static  float Cubevertices[] =
    {
            // a cube has 8 vertices
            // position attribute                       // color attribute
            -1.0f, -1.0f, -1.0f,             0.971f,  0.572f,  0.833f,1.0,
            -1.0f, +1.0f, -1.0f,          0.359f,  0.583f,  0.152f,1.0,
            +1.0f, +1.0f, -1.0f,        0.393f,  0.621f,  0.362f,1.0,
            +1.0f, -1.0f, -1.0f,       0.014f,  0.184f,  0.576f,1.0,
            -1.0f, -1.0f, +1.0f,       0.583f,  0.771f,  0.014f,1.0,
            -1.0f, +1.0f, +1.0f,       0.543f,  0.021f,  0.978f,1.0,
            +1.0f, +1.0f, +1.0f,       0.435f,  0.602f,  0.223f,1.0,
            +1.0f, -1.0f, +1.0f,        0.055f,  0.953f,  0.042f,1.0











    };
    static  uint32_t indices[] = {
            // a cube has 6 sides, 12 triangles
            0, 1, 2,
            0, 2, 3,
            4, 5, 6,
            4, 6, 7,
            0, 4, 7,
            0, 7, 3,
            1, 5, 6,
            1, 6, 2,
            0, 4, 5,
            0, 5, 1,
            3, 7, 6,
            3, 6, 2

    };

    struct Renderer3DData
    {
        Ref<VertexArray>  CubeVertexArray;
        Ref<VertexBuffer> CubeVertexBuffer;
        Ref<Shader>       CubeShader;
        Renderer3D::Statistics Stats;
        uint32_t CubeIndexCount = 0;
    };
    static Renderer3DData *s_Data;
    void Renderer3D::Init()
    {//float* vertices, uint32_t size
        s_Data=new Renderer3DData;
        s_Data->CubeVertexArray = VertexArray::Create();
        s_Data->CubeVertexBuffer = VertexBuffer::Create(Cubevertices,sizeof(Cubevertices));
        s_Data->CubeVertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float4, "a_Color" }
                                            });
        s_Data->CubeVertexArray->AddVertexBuffer(s_Data->CubeVertexBuffer);
        Ref<IndexBuffer> CubeIB = IndexBuffer::Create(indices, 36);
        s_Data->CubeVertexArray->SetIndexBuffer(CubeIB);
        s_Data->CubeShader = Shader::Create("assets/shaders/Cube.glsl");
    }

    void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 &transform)
    {
        glm::mat4 viewProj = camera.GetViewProjection();

        s_Data->CubeShader->Bind();


        s_Data->CubeShader->SetMat4("u_ViewProjection", viewProj);
        //2.2  0    0   0
        //0    2.4  0   0
        //0    0    -1.0  0
        //0    0    0   -0.02
    }

    void Renderer3D::EndScene()
    {
        Flush();
    }

    void Renderer3D::DrawCube(const glm::mat4 &transform,const glm::vec4 &tintColor )
    {
        s_Data->CubeIndexCount=36;

        RenderCommand::DrawIndexed(s_Data->CubeVertexArray, s_Data->CubeIndexCount);

    }
    void Renderer3D::Flush()
    {
        s_Data->CubeIndexCount=0;
    }
    Renderer3D::Statistics Renderer3D::GetStats()
    {
        return s_Data->Stats;;
    }

    void Renderer3D::ResetStats()
    {
        memset(&s_Data->Stats, 0, sizeof(Statistics));

    }

    void Renderer3D::Shutdown() {

    }



}