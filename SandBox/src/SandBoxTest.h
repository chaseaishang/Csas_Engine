//
// Created by chaseaishang on 23-6-14.
//

#ifndef SANDBOX_SANDBOXTEST_H
#define SANDBOX_SANDBOXTEST_H
#include "include/imgui.h"
#include <CsasEngine.h>

CsasEngine::Application* CsasEngine::CreateApplication();

class ExampleLayer : public CsasEngine::Layer
{
public:
    ExampleLayer()
            :Layer("Example")
    {}

    void OnUpdate() override
    {
//        CSAS_INFO("example OnUpdate");
    }
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Test");
        ImGui::Text("hello world");
        ImGui::End();

    }
    void OnEvent(CsasEngine::Event& event) override
    {
        CSAS_TRACE("{0}", event);
    }
};

class Sandbox:public CsasEngine::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }

private:

};

CsasEngine::Application* CsasEngine::CreateApplication()
{
    return new Sandbox();
}
#endif //SANDBOX_SANDBOXTEST_H
