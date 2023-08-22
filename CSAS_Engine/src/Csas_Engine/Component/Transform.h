//
// Created by chaseaishang on 23-7-18.
//
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>

namespace CsasEngine {
    struct TransformComponent
    {
        enum class Space : char {
            Local = 1 << 0,
            World = 1 << 1
        };

        glm::mat4 Transform=glm::mat4 {1.0f};


        glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
        glm::quat Rotation = {1,0.0f, 0.0f, 0.0f};
        glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
        glm::vec3 Euler = {0,0,0};//x y z

        glm::vec3 up;
        glm::vec3 forward;
        glm::vec3 right;

        TransformComponent() = default;

        TransformComponent(const TransformComponent &) = default;

        TransformComponent(const glm::vec3 &translation)
                : Translation(translation) {SetDirty();}
        //Local to world
        glm::mat4 GetTransform();
        glm::vec3 GetAndUpdateBasis(uint direction=0);
        void SetPosition(glm::vec3 position);
        void Translate(const glm::vec3& vector, Space space=Space::Local);
        void Rotate(const glm::vec3& axis, float angle,Space space=Space::Local);
        void Rotate(const glm::vec3& eulers);

        bool OnImGuiRender();
        void SetDirty();
    private:
        void Update();
        void RecalculateBasis(void);
        void RecalculateEuler(void);
        bool m_dirty= false;


    };
}