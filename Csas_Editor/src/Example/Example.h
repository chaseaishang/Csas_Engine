//
// Created by chaseaishang on 23-7-17.
//

#pragma once
#include "CsasEngine.h"
namespace CsasEngine {
    class Example {
    public:
        Example() = default;

        virtual ~Example() = default;

        virtual void OnAttach() = 0;

        virtual void OnDetach() = 0;

        virtual void Update(Timestep ts) = 0;

        virtual void OnImGuiRender() = 0;

        virtual void OnEvent(Event &e) = 0;
        static Example*getInstance(){return m_Instance;};
        Ref<Scene>get_Scene(){return m_Scene;};
    protected:
        static Example*m_Instance;
        Ref<Scene> m_Scene;

    };
}
