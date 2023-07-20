//
// Created by chaseaishang on 23-7-14.
//
/**
 * @details we Don't use rolls we are fps style camera
 * the up vector are always (0,1,0)
 *
 *
 * */

#pragma once

#include "Csas_Engine/Renderer/Camera.h"
#include <Csas_Engine/Core/Timestep.h>
#include "Csas_Engine/Events/include/Event.h"
namespace CsasEngine {
    struct CameraComponent;
    class SceneCamera : public Camera
    {
    public:

        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
        enum class Direction { W, A, S, D };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;
        void SetCamera3D(CameraComponent*Camera3D);     //TODO Remove
        void SetMousePosition(float x,float y){mouse_x=x;mouse_y=y;}//TODO Remove
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetPerspective(float verticalFOV, float nearClip, float farClip);
        void Update(Timestep ts); //TODO Using CameraSystem To Replace
        void OnEvent(Event&e);//TODO Using CameraSystem To Replace
        void SetViewportSize(uint32_t width, uint32_t height);

        float GetPerspectiveVerticalFOV() const { return m_PerspectiveFOV; }
        void SetPerspectiveVerticalFOV(float verticalFov) { m_PerspectiveFOV = verticalFov; RecalculateProjection(); }
        float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(float nearClip) { m_PerspectiveNear = nearClip; RecalculateProjection(); }
        float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(float farClip) { m_PerspectiveFar = farClip; RecalculateProjection(); }

        float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(float size) { m_OrthographicSize = size; RecalculateProjection(); }
        float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(float nearClip) { m_OrthographicNear = nearClip; RecalculateProjection(); }
        float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(float farClip) { m_OrthographicFar = farClip; RecalculateProjection(); }

        ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }
    private:
        void RecalculateProjection();

        void Spin(int delta_x, int delta_y);//TODO Using CameraSystem To Replace
        void Zoom(int zoom);//TODO Using CameraSystem To Replace
        void Move(Direction direction, float deltatime, bool snap);//TODO Using CameraSystem To Replace
    public:
        //for camera  Temp
        bool SpinEnable=false;
    private:
        ProjectionType m_ProjectionType = ProjectionType::Perspective;
//for 3D
        float m_AspectRatio = 0.0f;
        float m_PerspectiveFOV = glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
        //for 2D
        float m_OrthographicSize = 10.0f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;


        //for camera  Temp
        float delt_x,delt_y;
        float mouse_x,mouse_y;
        CameraComponent*m_Camera3D;

    };


}



