//
// Created by chaseaishang on 23-7-3.
//

#pragma once

#include "CsasEngine.h"
#include "Panels/SceneHierarchyPanel.h"
namespace CsasEngine {


    class EditorLayer : public Layer {
    public:
        EditorLayer();

        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        void OnUpdate(Timestep ts) override;

        virtual void OnImGuiRender() override;

        void OnEvent(Event &e) override;

    private:
        CsasEngine::OrthographicCameraController m_CameraController;

        // Temp
        Ref <VertexArray> m_SquareVA;
        Ref <Shader> m_FlatColorShader;
        Ref <Framebuffer> m_Framebuffer;
        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;

        bool m_PrimaryCamera = true;

        bool m_ViewportFocused = false, m_ViewportHovered = false;
        Ref <Texture2D> m_CheckerboardTexture;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};

        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}


