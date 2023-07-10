//
// Created by chaseaishang on 23-7-3.
//

#pragma once

#include "CsasEngine.h"

class ExampleLayer : public CsasEngine::Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(CsasEngine::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(CsasEngine::Event& e) override;
private:
    CsasEngine::ShaderLibrary m_ShaderLibrary;
    CsasEngine::Ref<CsasEngine::Shader> m_Shader;
    CsasEngine::Ref<CsasEngine::VertexArray> m_VertexArray;

    CsasEngine::Ref<CsasEngine::Shader> m_FlatColorShader;
    CsasEngine::Ref<CsasEngine::VertexArray> m_SquareVA;

    CsasEngine::Ref<CsasEngine::Texture2D> m_Texture, m_ChernoLogoTexture;

    CsasEngine::OrthographicCameraController m_CameraController;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

