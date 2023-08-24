//
// Created by chaseaishang on 23-7-17.
//

#include "Example2.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine {
    void Example2::OnAttach() {
        CSAS_INFO("Switch to Example2");
        CSAS_PROFILE_FUNCTION();



    }

    void CsasEngine::Example2::OnDetach() {

    }

    void CsasEngine::Example2::Update(CsasEngine::Timestep ts) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.Update(ts);

    }

    void CsasEngine::Example2::OnImGuiRender() {


    }

    void CsasEngine::Example2::OnEvent(CsasEngine::Event &e) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.OnEvent(e);
    }

}