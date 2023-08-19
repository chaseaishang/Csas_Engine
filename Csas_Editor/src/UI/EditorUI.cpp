//
// Created by chaseaishang on 23-7-24.
//
#include "CsasEngine.h"
#include "EditorUI.h"
#include "ImGui/include/imgui.h"
#include "Example.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
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
    void DrawMaterialInfo_PBR(Material_BasePBR::MaterialInfo &info)
    {
        Material_BasePBR::MaterialInfo temp=info;
        if(ImGui::DragFloat3("Ka",glm::value_ptr(temp.Ka),0.2,0,1))
            info.Ka=temp.Ka;
        if(ImGui::DragFloat3("Kd",glm::value_ptr(temp.Kd),0.2,0,1))
            info.Kd=temp.Kd;
        if(ImGui::DragFloat3("Ks",glm::value_ptr(temp.Ks),0.2,0,1))
            info.Ks=temp.Ks;
        if(ImGui::DragFloat("Shininess",&temp.Shininess,5.0f,0.0f,150))
            info.Shininess=temp.Shininess;
    }
    void EditorUI::DrawMaterial_PBR(Material_BasePBR & material)
    {
        ImGui::Text("Material_Type    Material_BasePBR");
        ImGui::Text("Shader_name%s",material.GetShader_name().c_str());
        ImGui::SameLine();
        ImGui::Button("Load_shader");
        ImGui::NewLine();
        auto &materialInfo=material.materialInfo;
        DrawMaterialInfo_PBR(materialInfo);
    }

    void EditorUI::DrawSpotLight(SpotLightComponent &spot)
    {



        ImGui::DragFloat3("Color",glm::value_ptr(spot.color),0.05,0,1);
        ImGui::DragFloat("intensity",&spot.intensity,5,0,500);


    }

    void EditorUI::DrawDirect_Light(DirectionLightComponent &direct_light)
    {

        ImGui::DragFloat3("Color",glm::value_ptr(direct_light.color),0.05,0,1);
        ImGui::DragFloat("intensity",&direct_light.intensity,5,0,500);

    }

    void EditorUI::DrawMaterial_BRDF(Material_BaseBRDF &material)
    {
       
        ImGui::Text("Material_Type    Material_BaseBRDF");
        ImGui::DragFloat3("albedo",glm::value_ptr(material.albedo),0.01,0,1);
        ImGui::DragFloat("metallic",&material.metallic,0.01,0,1);
        ImGui::DragFloat("roughness",&material.roughness,0.01,0.0001,1,"%.4f");
    }




}