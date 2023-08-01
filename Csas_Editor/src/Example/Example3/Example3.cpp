//
// Created by chaseaishang on 23-7-26.
//

#include "Example3.h"

#include "ImGui/include/imgui.h"
namespace CsasEngine {
    void Example3::OnAttach() {
        CSAS_INFO("Switch to Example3");
        CSAS_PROFILE_FUNCTION();
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

        m_ActiveScene = CreateRef<Scene>();
        m_Scene = m_ActiveScene;
        // Entity
        SpotLights=m_ActiveScene->CreateEntity("lights");
        SpotLights.AddComponent<SpotLightComponent>();

//        DirectionLights=m_ActiveScene->CreateEntity("DirectionLights");
//        DirectionLights.AddComponent<DirectionLightComponent>();


        auto Cube = m_ActiveScene->CreateEntity("Green Cube");
        Cube.AddComponent<MeshComponent>(Primitive::Cube);
        Cube.AddComponent<Material_BasePBR>();
        m_Cube = Cube;



        m_Sphere=m_ActiveScene->CreateEntity("Sphere");
        m_Sphere.AddComponent<MeshComponent>(Primitive::Sphere);
        m_Sphere.AddComponent<Material_BasePBR>();
        // BRDF
        for(int i=0;i<9;i++)
        {
            std::string name="BRDF_Sphere_"+std::to_string(i+1);
            m_BRDF_Sphere[i]=m_ActiveScene->CreateEntity(name);
            m_BRDF_Sphere[i].AddComponent<MeshComponent>(Primitive::Sphere);
            m_BRDF_Sphere[i].AddComponent<Material_BaseBRDF>();
        }
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                int index=i*3+j;
                auto &material=m_BRDF_Sphere[index].GetComponent<Material_BaseBRDF>();
                auto &metalness=material.metallic;
                metalness=(float)i / (float)3;
                auto &roughness=material.roughness;
                roughness=glm::clamp((float)j / (float)3, 0.05f, 1.0f);


            }
        }



        //Camera
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");


        auto &cc = m_CameraEntity.AddComponent<CameraComponent>(glm::vec3{0, 0, 3});
        cc.Primary = true;
        cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
        float mousex = Input::GetMouseX();
        float mousey = Input::GetMouseY();
        cc.Camera.SetMousePosition(mousex, mousey);
        cc.Camera.SetCamera3D(&cc);


    }

    void CsasEngine::Example3::OnDetach() {

    }

    void CsasEngine::Example3::Update(CsasEngine::Timestep ts) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.Update(ts);

    }

    void CsasEngine::Example3::OnImGuiRender() {


    }

    void CsasEngine::Example3::OnEvent(CsasEngine::Event &e) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.OnEvent(e);
    }

}