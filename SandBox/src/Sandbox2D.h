//
// Created by chaseaishang on 23-6-24.
//
#pragma once

#include "CsasEngine.h"

class Sandbox2D : public CsasEngine::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(CsasEngine::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(CsasEngine::Event& e) override;
private:
    CsasEngine::OrthographicCameraController m_CameraController;

    // Temp
    CsasEngine::Ref<CsasEngine::VertexArray> m_SquareVA;
    CsasEngine::Ref<CsasEngine::Shader> m_FlatColorShader;
    CsasEngine::Ref<CsasEngine::Framebuffer> m_Framebuffer;
    CsasEngine::Ref<CsasEngine::Texture2D> m_CheckerboardTexture;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};


