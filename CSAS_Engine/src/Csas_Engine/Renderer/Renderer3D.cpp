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


    namespace CameraSpec
    {
        glm::mat4 ViewProjMatrix[2];
    }


    struct Renderer3DData
    {
         Ref<UniformBuffer> CameraUBO;
         Renderer3D::Statistics Stats;
    };
    static Renderer3DData *s_Data;

    void Renderer3D::Init()
    {

        s_Data=new Renderer3DData;

        s_Data->CameraUBO=UniformBuffer::Create(sizeof(CameraSpec::ViewProjMatrix),0);


    }

    void Renderer3D::BeginScene(const Camera &camera, const glm::mat4 &transform)
    {

    }

    void Renderer3D::EndScene()
    {
        Flush();
    }

    void Renderer3D::Flush()
    {



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