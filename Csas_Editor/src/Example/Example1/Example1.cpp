//
// Created by chaseaishang on 23-7-17.
//

#include "Example1.h"

CsasEngine::Example* CsasEngine::Example::m_Instance=new Example1();
void CsasEngine::Example1::OnAttach()
{
    CSAS_PROFILE_FUNCTION();
    m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

    m_ActiveScene = CreateRef<Scene>();
    m_Scene=m_ActiveScene;
    // Entity
    auto square = m_ActiveScene->CreateEntity("Green Square");
    square.AddComponent<TransformComponent>(glm::vec3{-0.5,0,0});
    square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
    auto redSquare = m_ActiveScene->CreateEntity("Red Square");
    redSquare.AddComponent<TransformComponent>(glm::vec3{+0.5,0,0});
    redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    m_SquareEntity = square;

//    auto Sphere=m_ActiveScene->CreateEntity("Color Sphere");
//    Sphere.AddComponent<TransformComponent>();
//    auto &Sprite=Sphere.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
//    Sprite.style=1;


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
    delete m_Instance;
}

void CsasEngine::Example1::Update(CsasEngine::Timestep ts)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.Update(ts);

}

void CsasEngine::Example1::OnImGuiRender() {

}

void CsasEngine::Example1::OnEvent(CsasEngine::Event &e)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.OnEvent(e);
}

