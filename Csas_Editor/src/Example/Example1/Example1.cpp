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
    square.AddComponent<SpriteRendererComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});
    auto redSquare = m_ActiveScene->CreateEntity("Red Square");
    redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    m_SquareEntity = square;

    m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
    auto&cc=m_CameraEntity.AddComponent<CameraComponent>();
    cc.Primary= true;
    cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);



}

void CsasEngine::Example1::OnDetach()
{
    delete m_Instance;
}

void CsasEngine::Example1::Update(CsasEngine::Timestep ts)
{
    auto& translation = m_CameraEntity.GetComponent<TransformComponent>().Translation;
    float speed = 5.0f;

    if (Input::IsKeyPressed(Key::A))
        translation.x -= speed * ts;
    if (Input::IsKeyPressed(Key::D))
        translation.x += speed * ts;
    if (Input::IsKeyPressed(Key::W))
        translation.y += speed * ts;
    if (Input::IsKeyPressed(Key::S))
        translation.y -= speed * ts;

}

void CsasEngine::Example1::OnImGuiRender() {

}

void CsasEngine::Example1::OnEvent(CsasEngine::Event &e)
{
//    EventDispatcher dispatcher(e);
//    dispatcher.Dispatch<MouseScrolledEvent>(
//            [&](MouseScrolledEvent &e)
//    {
//
//        return true;
//    }
//            );
    //dispatcher.Dispatch<WindowResizeEvent>(CSAS_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
}

