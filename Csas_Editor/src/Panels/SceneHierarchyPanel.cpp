//
// Created by chaseaishang on 23-7-14.
//
#include "SceneHierarchyPanel.h"
#include "SceneMaterialPanel.h"
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
        auto&roots=m_Context->getRoots();
        for(auto&root:roots)
        {
            DrawSceneNode(root);
        }
//        m_Context->m_Registry.each([&](auto entityID)
//               {
//                   Entity entity{ entityID , m_Context.get() };
//                   DrawEntityNode(entity);
//               });
        if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
            m_SelectionContext = {};

        ImGui::End();

        //ImGui::SetNextItemOpen(true,ImGuiCond_Once);

        ImGui::Begin("Properties");
        if (m_SelectionContext)
            DrawComponents(m_SelectionContext);

        ImGui::End();
        ImGui::Begin("Material");
        if (m_SelectionContext)
        {
            DrawComponentsMaterial(m_SelectionContext);
        }

        ImGui::End();
    }
    void SceneHierarchyPanel::DrawSceneNode(const Node &node)
    {
        ImGuiTreeNodeFlags flags =  ImGuiTreeNodeFlags_Selected  |ImGuiTreeNodeFlags_DefaultOpen;

        bool opened = ImGui::TreeNodeEx(std::to_string((uint64_t) &node).c_str(), flags, "%s", node.getName().c_str());
        if(opened)
        {

            //ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 50);

            for(auto&entity:node.getEntities())
            {
                DrawEntityNode(entity);
            }
            //ImGui::PopStyleVar();


            ImGui::TreePop();
        }
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf ;

        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
        if (ImGui::IsItemClicked())
        {
            m_SelectionContext = entity;
        }

        if (opened)
        {
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
                glm::vec3 rotation = tc.Rotation;
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", tc.Scale, 1.0f);

                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<ModelComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(ModelComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
            {
                auto& model = entity.GetComponent<ModelComponent>();
                auto&tc=model.meshComponents[0].transform;
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
                glm::vec3 rotation = tc.Rotation;
 //               ImGui::DragFloat3("Rotation",glm::value_ptr(tc.Rotation));
                DrawVec3Control("Rotation", rotation);
                tc.Rotation = rotation;
                DrawVec3Control("Scale", tc.Scale, 1.0f);




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
                static bool show=false;
                ImGui::Checkbox("Show demo",&show);
                if(show)
                {
                    ImGui::ShowDemoWindow(&show);
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

        if(entity.HasComponent<DirectionLightComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(DirectionLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {


                auto& Direct=entity.GetComponent<DirectionLightComponent>();
                EditorUI::DrawDirect_Light(Direct);

                ImGui::TreePop();
            }
        }


    }

    void SceneHierarchyPanel::DrawComponentsMaterial(Entity entity)
    {
        if(entity.HasComponent<SpotLightComponent>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(SpotLightComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {


                auto& Spot=entity.GetComponent<SpotLightComponent>();
                EditorUI::DrawSpotLight(Spot);

                ImGui::TreePop();
            }
        }
        if(entity.HasComponent<Material_BaseBRDF>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(Material_BaseBRDF).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {

                auto& material=entity.GetComponent<Material_BaseBRDF>();
                EditorUI::DrawMaterial_BRDF(material);

                ImGui::TreePop();
            }
        }
        //Material_BaseBRDF
        if(entity.HasComponent<Material_BasePBR>())
        {
            if (ImGui::TreeNodeEx((void*)typeid(Material_BasePrimitive).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Material"))
            {

                auto& material=entity.GetComponent<Material_BasePBR>();
                EditorUI::DrawMaterial_PBR(material);

                ImGui::TreePop();
            }
        }
    }



}
