//
// Created by chaseaishang on 23-7-14.
//
#include "Csas_Engine/Csaspch.h"
#include "SceneCamera.h"
#include "Csas_Engine/Component/Camera3D.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/string_cast.hpp"
#include <Csas_Engine/Core/Input.h>
#include <Csas_Engine/Events/include/MouseEvent.h>


namespace CsasEngine {

    SceneCamera::SceneCamera()
    {
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Orthographic;
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar = farClip;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
    {
        m_ProjectionType = ProjectionType::Perspective;
        m_PerspectiveFOV = verticalFOV;
        m_PerspectiveNear = nearClip;
        m_PerspectiveFar = farClip;
        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
    {
        m_AspectRatio = (float)width / (float)height;
        float &fov    =this->m_Camera3D->fov;
        auto&near=m_Camera3D->near_clip;
        auto&far=m_Camera3D->far_clip;
        m_Projection=glm::perspective(glm::radians(fov),m_AspectRatio,near,far);
        m_ViewProjection=m_Projection*m_View;
    }

    void SceneCamera::RecalculateProjection()
    {
        if (m_ProjectionType == ProjectionType::Perspective)
        {



            m_Projection = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);


        }


    }

    void SceneCamera::Spin(int delta_x, int delta_y)//自 旋
    {
        if (delta_x == 0 && delta_y == 0)
        {
            return;
        }
        float &euler_x    =this->m_Camera3D->euler_x;
        float &euler_y    =this->m_Camera3D->euler_y;
        glm::vec3 &forward=this->m_Camera3D->forward;
        glm::vec3 &right=this->m_Camera3D->right;
        glm::vec3 &up=this->m_Camera3D->up;
        glm::vec3 &position=this->m_Camera3D->trans.Translation;
        float &sensitivity=this->m_Camera3D->sensitivity;
        euler_y += delta_x * sensitivity;
        euler_x += delta_y * sensitivity;
        euler_x = glm::clamp(euler_x, -90.0f, 90.0f);  // clamp vertical rotation

        forward = glm::normalize(
                glm::vec3(
                        cos(glm::radians(euler_y)) * cos(glm::radians(euler_x)),
                        sin(glm::radians(euler_x)),
                        sin(glm::radians(euler_y)) * cos(glm::radians(euler_x))
                            )
        );

        right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, forward));

        m_View=glm::lookAt(position,position+forward,up);
        m_ViewProjection=m_Projection*m_View;

    }

    void SceneCamera::Zoom(int zoom)
    {
        float &fov    =this->m_Camera3D->fov;
        float &zoom_speed    =this->m_Camera3D->zoom_speed;
        fov += zoom * zoom_speed;
        fov = glm::clamp(fov, 1.0f, 90.0f);
        //glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
        m_Projection=glm::perspective(glm::radians(fov),m_AspectRatio,m_PerspectiveNear,m_PerspectiveFar);
        m_ViewProjection=m_Projection*m_View;
    }

    void SceneCamera::Move(SceneCamera::Direction direction, float deltatime, bool snap)
    {
        glm::vec3 & position= this->m_Camera3D->trans.Translation;
        glm::vec3 &forward=this->m_Camera3D->forward;
        glm::vec3 &up=this->m_Camera3D->up;
        glm::vec3 &right=this->m_Camera3D->right;
        float &move_speed    =this->m_Camera3D->move_speed;
        switch (direction)
        {
            case Direction::W: position += forward * (move_speed * deltatime); break;
            case Direction::S: position -= forward * (move_speed * deltatime); break;
            case Direction::A: position -= right * (move_speed * deltatime); break;
            case Direction::D: position += right * (move_speed * deltatime); break;
            default:
                CSAS_CORE_WARN("[WARNING] Invalid direction!"); break;
        }
        if (snap) {
            position.y = 0.0f;  // snap to the ground
        }
        m_View=glm::lookAt(position,position+forward,up);
        m_ViewProjection=m_Projection*m_View;
//glm::lookAt(position, position + forward, up);

    }

    void SceneCamera::Update(Timestep ts)
    {
        if (Input::IsKeyPressed(Key::A))
            Move(Direction::A,ts.GetSeconds(), false);
        if (Input::IsKeyPressed(Key::D))
            Move(Direction::D,ts.GetSeconds(), false);
        if (Input::IsKeyPressed(Key::W))
            Move(Direction::W,ts.GetSeconds(), false);
        if (Input::IsKeyPressed(Key::S))
            Move(Direction::S,ts.GetSeconds(), false);


    }

    void SceneCamera::OnEvent(Event &e)
    {

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<MouseScrolledEvent>(
                [&](MouseScrolledEvent&e)->bool
                {

                    int zoom=e.GetYOffset();
                    if(zoom>0)
                    {
                        this->Zoom(-1);
                    }
                    else
                    {
                        this->Zoom(1);
                    }

                    return false;
                }
                );
        dispatcher.Dispatch<MouseMovedEvent>(
                [&](MouseMovedEvent&e)->bool
                {
                    float x=e.GetX();
                    float y=e.GetY();

                    delt_x=x-mouse_x;
                    delt_y=mouse_y-y;
                    mouse_x=x;
                    mouse_y=y;
                    if(SpinEnable&&Input::IsMouseButtonPressed(Mouse::ButtonLeft))
                    {
                        Spin(delt_x,delt_y);
                    }
                    return false;
                }
                );



    }

    void SceneCamera::SetCamera3D(CameraComponent *Camera3D)
    {
        m_Camera3D=Camera3D;
        auto&position=m_Camera3D->trans.Translation;
        auto&forward=m_Camera3D->forward;
        auto &up=m_Camera3D->up;
        auto&fov=m_Camera3D->fov;
        CSAS_CORE_WARN("position={0}",glm::to_string(position));
        CSAS_CORE_WARN("forward={0}",glm::to_string(forward));
        CSAS_CORE_WARN("up={0}",glm::to_string(up));
        CSAS_CORE_WARN("fov={0}",fov);

        //init Matrix
        init();


    }

    void SceneCamera::init()
    {
        auto&position=m_Camera3D->trans.Translation;
        auto&forward=m_Camera3D->forward;
        auto &up=m_Camera3D->up;
        auto&fov=m_Camera3D->fov;
        m_AspectRatio=1280.0f/720.0f;
        m_View=glm::lookAt(position,position+forward,up);
        m_Projection = glm::perspective(glm::radians(fov), m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
        m_ViewProjection=m_Projection*m_View;
        CSAS_CORE_WARN("m_ViewProjection={0}",glm::to_string(m_ViewProjection));
    }


}