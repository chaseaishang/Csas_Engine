//
// Created by chaseaishang on 23-7-13.
//

#pragma once


#include "entt/include/entt.hpp"
#include "Csas_Engine/Core/Timestep.h"
#include "glm/glm.hpp"
namespace CsasEngine
{


    class Camera;
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
        void SetSceneType(bool TwoDScene=true){m_2DScene=TwoDScene;};
    private:
        void On3DUpdate(Camera &main_camera,glm::mat4 &cameraTransform);
        void On2DUpdate(Camera &main_camera,glm::mat4 &cameraTransform);

        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        bool m_2DScene= false;


    };

}




