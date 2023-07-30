//
// Created by chaseaishang on 23-7-17.
//
//    ads test
//
//

#pragma once
#include "CsasEngine.h"
#include "../Example.h"
#include "../../Panels/SceneHierarchyPanel.h"
namespace CsasEngine {
    class Example2:public Example
    {
    public:
        Example2()=default;
        ~Example2()=default;
        void OnAttach() ;

        void OnDetach() ;

        void Update(Timestep ts) ;

        void OnImGuiRender() ;

        void OnEvent(Event &e) ;


    private:
        // Temp


        Ref<Scene> m_ActiveScene;
        Entity m_Cube;

        Entity m_Torus;
        Entity m_Sphere;
        Entity m_teapot;
        Entity SpotLights;
        Entity DirectionLights;
        Entity m_CameraEntity;
        bool m_PrimaryCamera = true;
        Ref <Texture2D> m_CheckerboardTexture;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };


    };

}

