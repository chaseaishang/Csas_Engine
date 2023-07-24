//
// Created by chaseaishang on 23-7-17.
//

#include "Example1.h"
#include "ImGui/include/imgui.h"

void CsasEngine::Example1::OnAttach()
{
    CSAS_INFO("Switch to Example1");
    CSAS_PROFILE_FUNCTION();
    m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

    m_ActiveScene = CreateRef<Scene>();
    m_Scene=m_ActiveScene;
    // Entity

    auto Cube=m_ActiveScene->CreateEntity("Green Cube");
    Cube.AddComponent<MeshComponent>(Primitive::Cube);
    m_Cube=Cube;
    m_Quad=m_ActiveScene->CreateEntity("Quad");
    m_Quad.AddComponent<MeshComponent>(Primitive::Quad);
    //Camera
    m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");


    auto&cc=m_CameraEntity.AddComponent<CameraComponent>(glm::vec3 {0,0,3});
    cc.Primary= true;
    cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
    float mousex=Input::GetMouseX();
    float mousey=Input::GetMouseY();
    cc.Camera.SetMousePosition(mousex,mousey);
    cc.Camera.SetCamera3D(&cc);


}

void CsasEngine::Example1::OnDetach()
{

}

void CsasEngine::Example1::Update(CsasEngine::Timestep ts)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.Update(ts);

}

void CsasEngine::Example1::OnImGuiRender()
{


}

void CsasEngine::Example1::OnEvent(CsasEngine::Event &e)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.OnEvent(e);
}

