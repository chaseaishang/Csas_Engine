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
        auto Cube = m_ActiveScene->CreateEntity("Green Cube");
        Cube.AddComponent<MeshComponent>(Primitive::Cube);
        Cube.AddComponent<Material_BasePBR>();
        m_Cube = Cube;

        m_Quad = m_ActiveScene->CreateEntity("Quad");
        m_Quad.AddComponent<MeshComponent>(Primitive::Quad);
        m_Quad.AddComponent<Material_BasePrimitive>();

        m_Sphere=m_ActiveScene->CreateEntity("Sphere");
        m_Sphere.AddComponent<MeshComponent>(Primitive::Sphere);
        m_Sphere.AddComponent<Material_BasePBR>();

        m_Torus=m_ActiveScene->CreateEntity("Torus");
        m_Torus.AddComponent<MeshComponent>(Primitive::Torus);
        m_Torus.AddComponent<Material_BasePBR>();

        m_teapot=m_ActiveScene->CreateEntity("teapot");
        m_teapot.AddComponent<ModelComponent>("./assets/model/teapot.obj");
        m_teapot.AddComponent<Material_BasePBR>();

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