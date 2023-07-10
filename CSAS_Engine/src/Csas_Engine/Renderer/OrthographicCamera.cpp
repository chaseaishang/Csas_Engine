//
// Created by chaseaishang on 23-6-24.
//
#include "Csas_Engine/Csaspch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace CsasEngine {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
            : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
    {
        CSAS_PROFILE_FUNCTION();

        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        CSAS_PROFILE_FUNCTION();

        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        CSAS_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }



}
