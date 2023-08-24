//
// Created by chaseaishang on 23-7-17.
//

#include "Example1.h"
#include "ImGui/include/imgui.h"

void CsasEngine::Example1::OnAttach()
{
    CSAS_INFO("Switch to Example1");
    CSAS_PROFILE_FUNCTION();



}

void CsasEngine::Example1::OnDetach()
{

}

void CsasEngine::Example1::Update(CsasEngine::Timestep ts)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.Update(ts);

}

void CsasEngine::Example1::OnImGuiRender()
{


}

void CsasEngine::Example1::OnEvent(CsasEngine::Event &e)
{
    auto& camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
    camera.OnEvent(e);
}

