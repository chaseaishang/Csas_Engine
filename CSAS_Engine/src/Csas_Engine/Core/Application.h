//
// Created by chaseaishang on 23-6-14.
//
#pragma once




#include "Csas_Engine/Core/Core.h"
#include "Csas_Engine/Core/Window.h"
#include "Csas_Engine/Events/include/ApplicationEvent.h"

#include "Csas_Engine/Core/LayerStack.h"
#include "Csas_Engine/ImGui/ImGuiLayer.h"
#include "Csas_Engine/Core/Timestep.h"
int main(int argc, char** argv);
namespace CsasEngine
{
    class Application
    {
    public:
        Application(const std::string& name = "Csas App");
        virtual ~Application();

        void OnEvent(Event& e);
        float GetTime(){return m_LastFrameTime;}
        float GetDeltaTime(){return m_delta_Time;}
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
        inline Window& GetWindow() { return *m_Window; }
        void Close();
        inline static Application& Get() { return *s_Instance; }
    private:
        void Run();
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;
        bool m_Minimized = false;
        LayerStack m_LayerStack;
        float m_LastFrameTime = 0.0f;
        float m_delta_Time = 0.0f;
    private:
        static Application* s_Instance;
        friend int ::main(int argc, char** argv);
    };

    // To be defined in CLIENT
    Application* CreateApplication();




}//namespace CsasEngine