//
// Created by chaseaishang on 23-6-14.
//
#pragma once



#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Events/include/Event.h"
#include <Csas_Engine/Events/include/ApplicationEvent.h>
#include "Csas_Engine/Core.h"
#include "Csas_Engine/Log.h"
#include "Csas_Engine/Window.h"
#include "Csas_Engine/LayerStack.h"
#include "Csas_Engine/ImGui/ImGuiLayer.h"

#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Buffer.h"
#include "Csas_Engine/Renderer/VertexArray.h"
namespace CsasEngine
{
class Application
{
public:
    Application();
    virtual ~Application();
    void Run();
    void OnEvent(Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline static Application& Get() { return *s_Instance; }
    inline Window& GetWindow() { return *m_Window; }

private:
    bool OnWindowClose(WindowCloseEvent& e);

    std::shared_ptr<Shader> m_Shader;
    std::shared_ptr<VertexArray> m_VertexArray;

    std::shared_ptr<Shader> m_BlueShader;
    std::shared_ptr<VertexArray> m_SquareVA;



    std::unique_ptr<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;

    static Application* s_Instance;

};
Application* CreateApplication();


}//namespace CsasEngine