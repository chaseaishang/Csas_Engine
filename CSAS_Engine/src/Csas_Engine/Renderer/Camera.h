//
// Created by chaseaishang on 23-7-14.
//

#pragma once


#include <glm/glm.hpp>

namespace CsasEngine {

    class Camera
    {
    public:
        Camera() = default;
        Camera(const glm::mat4& projection)
                : m_Projection(projection) {}

        virtual ~Camera() = default;
        const glm::mat4& GetViewProjection() const { return m_ViewProjection; }
        const glm::mat4& GetView() const { return m_View; }
        const glm::mat4& GetProjection() const { return m_Projection; }
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_View= glm::mat4(1.0f);
        glm::mat4 m_ViewProjection=glm::mat4(1.0f);
    };


}
