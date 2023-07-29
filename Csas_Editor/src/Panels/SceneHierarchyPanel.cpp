//
// Created by chaseaishang on 23-7-14.
//
#include "SceneHierarchyPanel.h"

#include "ImGui/include/imgui.h"
#include <ImGui/include/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
#include "Csas_Engine/Component/AllComponent.h"
#include "../UI/EditorUI.h"
namespace CsasEngine {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
    }

    void SceneHierarchyPanel::OnImGuiRender()
    {
        ImGui::Begin("Scene Hierarchy");

        m_Context->m_Registry.each([&](auto entityID)
                                   {
                                       Entity entity{ entityID , m_Context.get() };
                                       DrawEntityNode(entity);
                                   });
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (opened)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
//            bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
//            if (opened)
//                ImGui::TreePop();
            ImGui::TreePop();
        }

    }




    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }







    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
            {
                tag = std::string(buffer);
            }
        }

        if (entity.HasComponent<TransformComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& tc = entity.GetComponent<TransformComponent>();
                DrawVec3Control("Translation", tc.Translation);
                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<MeshComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& tc = entity.GetComponent<MeshComponent>().transform;
                DrawVec3Control("Translation", tc.Translation);
                glm::vec3 rotation = glm::degrees(tc.Rotation);
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);




                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<Material_BasePrimitive>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(Material_BasePrimitive).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {
                auto& material=entity.GetComponent<Material_BasePrimitive>();
                ImGui::Text("Material_Type    Material_BasePrimitive");

               //float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                //        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
                //ImGui::ShowDemoWindow();

                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<Material_BasePBR>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(Material_BasePrimitive).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {


                auto& material=entity.GetComponent<Material_BasePBR>();
                EditorUI::DrawMaterial_PBR(material);

                //float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                //        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
                //ImGui::ShowDemoWindow();

                ImGui::TreePop();
            }
        }
        if (entity.HasComponent<CameraComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
            {
                auto& cameraComponent = entity.GetComponent<CameraComponent>();
                auto& camera = cameraComponent.Camera;

                ImGui::Checkbox("Primary", &cameraComponent.Primary);



                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
                {
                    float perspectiveVerticalFov = cameraComponent.fov;
                    if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
                        cameraComponent.fov=perspectiveVerticalFov;
                    float perspectiveNear = cameraComponent.near_clip;
                    if (ImGui::DragFloat("Near", &perspectiveNear))
                        cameraComponent.near_clip=perspectiveNear;
                    float perspectiveFar = cameraComponent.far_clip;
                    if (ImGui::DragFloat("Far", &perspectiveFar))
                        cameraComponent.far_clip=perspectiveFar;
                    //position
                    auto& tc = cameraComponent.trans;
                    DrawVec3Control("Translation", tc.Translation);

                    //Spin
                    ImGui::Checkbox("SpinEnale", &camera.SpinEnable);

                }




                ImGui::TreePop();
            }
        }
        if (entity.HasComponent<SpriteRendererComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
            {
                auto& src = entity.GetComponent<SpriteRendererComponent>();
                ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));
                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<SpotLightComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpotLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {


                auto& Spot=entity.GetComponent<SpotLightComponent>();
                EditorUI::DrawSpotLight(Spot);

                //float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                //        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
                //ImGui::ShowDemoWindow();

                ImGui::TreePop();
            }
        }


    }

}
