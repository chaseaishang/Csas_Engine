//
// Created by chaseaishang on 23-7-18.
//
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace CsasEngine {
    struct TransformComponent {
        glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

        TransformComponent() = default;

        TransformComponent(const TransformComponent &) = default;

        TransformComponent(const glm::vec3 &translation)
                : Translation(translation) {}

        glm::mat4 GetTransform() const {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0})
                                 * glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0})
                                 * glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});

            return glm::translate(glm::mat4(1.0f), Translation)
                   * rotation
                   * glm::scale(glm::mat4(1.0f), Scale);
        }
        void Rotate(const glm::vec3& axis, float angle)
        {
            auto temp=glm::normalize(glm::vec4{this->Rotation,0}*
                                               glm::rotate(glm::mat4(1),angle,axis)
            );
            this->Rotation=(glm::vec3(temp));
        }
    };
}