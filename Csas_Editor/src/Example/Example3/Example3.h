//
// Created by chaseaishang on 23-7-26.
//

#pragma once

#pragma once
#include "CsasEngine.h"
#include "../Example.h"
#include "../../Panels/SceneHierarchyPanel.h"
namespace CsasEngine {
    class Example3:public Example
    {
    public:
        Example3()=default;
        ~Example3()=default;
        void OnAttach() ;

        void OnDetach() ;

        void Update(Timestep ts) ;

        void OnImGuiRender() ;

        void OnEvent(Event &e) ;


    private:
        // Temp


        Ref<Scene> m_ActiveScene;

        Entity m_BRDF_Sphere[9];

        Entity m_Cube;
        Entity m_Sphere;

        Entity SpotLights[4];

        Entity m_CameraEntity;


        bool m_PrimaryCamera = true;
        Ref <Texture2D> m_CheckerboardTexture;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };


    };

}

