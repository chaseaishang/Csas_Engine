//
// Created by chaseaishang on 23-7-14.
//
#pragma once

#include "Csas_Engine/Core/Core.h"
#include "Csas_Engine/Core/Log.h"
#include "Csas_Engine/Scene/Scene.h"
#include "Csas_Engine/Component/Entity.h"

namespace CsasEngine {

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& scene);

        void SetContext(const Ref<Scene>& scene);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
        void DrawComponentsMaterial(Entity entity);
        void DrawSceneNode(const Node&node);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };

}


