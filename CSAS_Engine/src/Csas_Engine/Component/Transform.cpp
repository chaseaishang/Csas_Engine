//
// Created by chaseaishang on 23-8-20.
//
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include <ImGui/include/imgui.h>
#include <ImGui/include/imgui_internal.h>
namespace CsasEngine
{
    static constexpr glm::vec3 origin        { 0.0f };
    static constexpr glm::mat4 identity      { 1.0f };
    static constexpr glm::quat eye           { 1.0f, 0.0f, 0.0f, 0.0f };  // wxyz
    static constexpr glm::vec3 world_right   { 1.0f, 0.0f, 0.0f };  // +x axis
    static constexpr glm::vec3 world_up      { 0.0f, 1.0f, 0.0f };  // +y axis
    static constexpr glm::vec3 world_forward { 0.0f, 0.0f,-1.0f };  // -z axis

    //Update at once
    void TransformComponent::Rotate(const glm::vec3 &axis, float angle,Space space)
    {
        float radians = glm::radians(angle);
        glm::vec3 v = glm::normalize(axis);

        glm::mat4 R = glm::rotate(glm::mat4(1),radians, v);     // rotation matrix4x4
        glm::quat Q = glm::angleAxis(radians, v);  // rotation quaternion

        // local space rotation: expect v in local space, e.g. right = vec3(1, 0, 0)
        if (space == Space::Local)
        {
            this->Transform = this->Transform * R;
            this->Rotation = glm::normalize(this->Rotation * Q);
        }// world space rotation: expect v in world space, may introduce translation
        else {
            this->Transform = R * this->Transform;
            this->Rotation = glm::normalize(Q * this->Rotation);
            this->Translation = glm::vec3(this->Transform[3]);
        }
        RecalculateBasis();
        RecalculateEuler();

    }
    void TransformComponent::Rotate(const glm::vec3 &eulers)
    {
        this->Euler=eulers;
        glm::vec3 radians = glm::radians(eulers);
        // rotation quaternion
        glm::quat QX = glm::angleAxis(radians.x, world_right);
        glm::quat QY = glm::angleAxis(radians.y, world_up);
        glm::quat QZ = glm::angleAxis(radians.z, world_forward);
        glm::quat Q = QZ * QX * QY;  // Y->X->Z

        // local space rotation: euler angles are relative to local basis vectors


        this->Rotation = glm::normalize(eye * Q);

        SetDirty();

    }

    glm::mat4 TransformComponent::GetTransform()
    {
        Update();
        return this->Transform;
    }

    void TransformComponent::RecalculateBasis(void)
    {
        this->right   = this->Rotation * world_right;
        this->up      = this->Rotation * world_up;
        this->forward = this->Rotation * world_forward;

    }

    void TransformComponent::RecalculateEuler(void)
    {


        // alternatively, we can extract euler angles from our quaternion as follows and it
        // still works. However, be aware that this is not equivalent to the above because
        // the order matters when it comes to euler angles. In particular, `glm::eulerAngles`
        // assumes an implicit order of X->Y->Z, not our Y->X->Z convention, so the values
        // returned are different. In fact both values are correct, they are just different
        // ways to represent the same orientation. It doesn't matter which axis we'd like to
        // rotate around first or last, as long as we are consistent it will be fine, but
        // there's no harm in being explicit.

        glm::vec3 eulers = glm::eulerAngles(this->Rotation);
        this->Euler=glm::degrees(eulers);



    }

    void TransformComponent::Update()
    {
        if(!m_dirty)
        {
            return;
        }
        {
            glm::vec3 radians = glm::radians(this->Euler);
            // rotation quaternion
            glm::quat QX = glm::angleAxis(radians.x, world_right);
            glm::quat QY = glm::angleAxis(radians.y, world_up);
            glm::quat QZ = glm::angleAxis(radians.z, world_forward);
            glm::quat Q = QZ * QX * QY;  // Y->X->Z

            // local space rotation: euler angles are relative to local basis vectors


            this->Rotation = glm::normalize(eye * Q);
        }
        RecalculateBasis();

        this->Transform=glm::translate(glm::mat4(1.0f), Translation)
                       * glm::mat4_cast(this->Rotation)
                       * glm::scale(glm::mat4(1.0f), Scale);
        m_dirty = false;


        if constexpr (false)
        {
            this->right   = glm::normalize(glm::vec3(Transform[0]));
            this->up      = glm::normalize(glm::vec3(Transform[1]));
            this->forward = glm::normalize(glm::vec3(Transform[2])) * (-1.0f);

        }
    }



    void TransformComponent::SetDirty()
    {
        if(!m_dirty)
        {
            m_dirty= true;
        }

    }
    bool TransformComponent::OnImGuiRender()
    {
        auto draw_vec3 =
                [&](const std::string &label, glm::vec3 &values, float resetValue = 0.0f, float columnWidth = 100.0f) -> bool {
                    ImGuiIO &io        = ImGui::GetIO();
//                    auto     bold_font = io.Fonts->Fonts[0];
                    bool     update    = false;

                    ImGui::PushID(label.c_str());

                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, columnWidth);
                    ImGui::Text(label.c_str());
                    ImGui::NextColumn();

                    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

                    float  line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
                    ImVec2 button_size = {line_height + 3.0f, line_height};

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
//                    ImGui::PushFont(bold_font);
                    if (ImGui::Button("X", button_size))
                    {
                        values.x = resetValue;
                        update   = true;
                    }
//                    ImGui::PopFont();
                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    update |= ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.3f");
                    ImGui::PopItemWidth();
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
//                    ImGui::PushFont(bold_font);
                    if (ImGui::Button("Y", button_size))
                    {
                        values.y = resetValue;
                        update   = true;
                    }
//                    ImGui::PopFont();
                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    update |= ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.3f");
                    ImGui::PopItemWidth();
                    ImGui::SameLine();

                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
//                    ImGui::PushFont(bold_font);
                    if (ImGui::Button("Z", button_size))
                    {
                        values.z = resetValue;
                        update   = true;
                    }
//                    ImGui::PopFont();
                    ImGui::PopStyleColor(3);

                    ImGui::SameLine();
                    update |= ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.3f");
                    ImGui::PopItemWidth();

                    ImGui::PopStyleVar();

                    ImGui::Columns(1);

                    ImGui::PopID();

                    return update;
                };

        bool update = false;

        update |= draw_vec3("Translation", this->Translation, 0.f);
        update |= draw_vec3("Rotation", Euler, 0.f);
        update |= draw_vec3("Scale", Scale, 1.f);

        if (update)
        {
            SetDirty();
        }

        return update;
    }

    glm::vec3 TransformComponent::GetAndUpdateBasis(uint direction)
    {
        Update();
        switch (direction) {
            case 0:return up;
            case 1:return forward;
            case 2:return right;
        }

    }

    void TransformComponent::SetPosition(glm::vec3 position)
    {
        this->Translation=position;
        SetDirty();
    }

    void TransformComponent::Translate(const glm::vec3 &vector, TransformComponent::Space space)
    {
        // local space translation: expect vector in local space coordinates
        if (space == Space::Local) {
            glm::vec3 world_v = this->Rotation * vector;
            this->Translation += world_v;
            this->Transform[3] = glm::vec4(Translation, 1.0f);
        }
            // world space translation: position is directly updated by the vector
        else {
            this->Translation += vector;
            this->Transform[3] = glm::vec4(Translation, 1.0f);
        }
    }

}