//
// Created by chaseaishang on 23-7-13.
//

#pragma once
#include "entt/include/entt.hpp"
#include "Csas_Engine/Core/Timestep.h"
namespace CsasEngine
{
    class Entity;
    class Scene
    {
    friend class Entity;
    friend class SceneHierarchyPanel;
    public:
        Scene();
        ~Scene();

        Entity CreateEntity(const std::string& name = std::string());
        void OnViewportResize(uint32_t width, uint32_t height);
        void OnUpdate(Timestep ts);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
    };

}




