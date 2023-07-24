//
// Created by chaseaishang on 23-7-24.
//
#include "CsasEngine.h"
#include "EditorUI.h"
#include "ImGui/include/imgui.h"
#include "Example.h"
namespace CsasEngine {


    void EditorUI::MenuSetting()
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

                if (ImGui::MenuItem("Exit")) Application::Get().Close();
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::BeginMenu("Scene Example"))
            {
                // Disabling fullscreen would allow the window to be moved to the front of other windows,
                // which we can't undo at the moment without finer window depth/z control.
                //ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
                ExampleMenu::DrawMenuItems();



                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

    }
}