//
// Created by chaseaishang on 23-7-17.
//

#include "Example2.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine {
    void Example2::OnAttach() {
        CSAS_INFO("Switch to Example2");
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
        Cube.AddComponent<MeshComponent<Vertex>>(Primitive::Cube);
        Cube.AddComponent<Material_BasePBR>();
        m_Cube = Cube;



        m_Sphere=m_ActiveScene->CreateEntity("Sphere");
        m_Sphere.AddComponent<MeshComponent<Vertex>>(Primitive::Sphere);
        m_Sphere.AddComponent<Material_BasePBR>();

        m_Torus=m_ActiveScene->CreateEntity("Torus");
        m_Torus.AddComponent<MeshComponent<Vertex>>(Primitive::Torus);
        m_Torus.AddComponent<Material_BasePBR>();

        m_teapot=m_ActiveScene->CreateEntity("teapot");
        m_teapot.AddComponent<ModelComponent>("./assets/model/teapot.obj");
        m_teapot.AddComponent<Material_BasePBR>();
        //Cartoon
        Cartoon_torus=m_ActiveScene->CreateEntity("Cartoon_torus");
        Cartoon_torus.AddComponent<MeshComponent<Vertex>>(Primitive::Torus);
        Cartoon_torus.AddComponent<Material_Cartoon>();

        Cartoon__Sphere=m_ActiveScene->CreateEntity("Cartoon__Sphere");
        Cartoon__Sphere.AddComponent<MeshComponent<Vertex>>(Primitive::Sphere);
        Cartoon__Sphere.AddComponent<Material_Cartoon>();


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

    void CsasEngine::Example2::OnDetach() {

    }

    void CsasEngine::Example2::Update(CsasEngine::Timestep ts) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.Update(ts);

    }

    void CsasEngine::Example2::OnImGuiRender() {


    }

    void CsasEngine::Example2::OnEvent(CsasEngine::Event &e) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.OnEvent(e);
    }

}