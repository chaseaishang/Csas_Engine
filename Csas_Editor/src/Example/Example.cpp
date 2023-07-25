//
// Created by chaseaishang on 23-7-17.
//
#include "../EditorLayer.h"
#include "Example.h"
#include "Example1/Example1.h"
#include "Example2/Example2.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine
{
    CsasEngine::Example* CsasEngine::Example::m_Instance;
    Example* ExampleMenu::last_Example= nullptr;
    uint8_t ExampleMenu::size=0;
    std::vector<std::pair<std::string, std::function<Example*()>>>ExampleMenu::m_Examples;

    void ExampleMenu::RegisterAllExample()
    {
        RegisterExample<Example1>("Example1");
        RegisterExample<Example2>("Example2");
        m_Instance=new Example2;
    }

    void ExampleMenu::UnRegisterAllExample()
    {
        if(m_Instance)
        {
            delete m_Instance;
        }
    }

    void ExampleMenu::DrawMenuItems()
    {
        for(int i=0;i<size;i++)
        {
            auto str=m_Examples[i].first;
            auto fun=m_Examples[i].second;
            if (ImGui::MenuItem(str.c_str()))
            {
                last_Example=m_Instance;
                m_Instance=fun();
                EditorLayer::SetSwitchScene(true);

            }
            if(i!=size-1)
                ImGui::Separator();


        }


    }
}

