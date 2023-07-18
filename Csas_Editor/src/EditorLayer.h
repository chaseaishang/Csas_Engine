//
// Created by chaseaishang on 23-7-3.
//

#pragma once

#include "CsasEngine.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Example/Example.h"
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

        // Temp
        Ref <Framebuffer> m_Framebuffer= nullptr;
        Ref<Scene> m_ActiveScene= nullptr;

        SceneCamera m_EditCamera;
        bool m_PrimaryCamera = true;

        bool m_ViewportFocused = false, m_ViewportHovered = false;

        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

        Example*m_example= nullptr;
        // Panels
        SceneHierarchyPanel m_SceneHierarchyPanel;
    };
}


