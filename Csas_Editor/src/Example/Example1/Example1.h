//
// Created by chaseaishang on 23-7-17.
//

#ifndef CSAS_EDITOR_EXAMPLE1_H
#define CSAS_EDITOR_EXAMPLE1_H
#include "CsasEngine.h"
#include "../Example.h"
#include "../../Panels/SceneHierarchyPanel.h"
namespace CsasEngine {
    class Example1:public Example
    {
    public:
        Example1()=default;
        ~Example1()=default;
        void OnAttach() ;

        void OnDetach() ;

        void Update(Timestep ts) ;

        void OnImGuiRender() ;

        void OnEvent(Event &e) ;


    private:
        // Temp


        Ref<Scene> m_ActiveScene;
        Entity m_Cube;
        Entity m_Quad;
        Entity m_CameraEntity;
        bool m_PrimaryCamera = true;
        Ref <Texture2D> m_CheckerboardTexture;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };


    };

}
#endif //CSAS_EDITOR_EXAMPLE1_H
