//
// Created by chaseaishang on 23-7-18.
//

#pragma once

#include "Transform.h"
#include "Csas_Engine/Scene/SceneCamera.h"
namespace CsasEngine
{

    struct CameraComponent
    {
//TODO May be the euler function should in TransformComponent
    friend class SceneCamera;
    public:
        float fov=45;           // vertical field of view (fov_y)
        float near_clip=0.001f;     // near clipping distance
        float far_clip=100.0f;      // far clipping distance
        float move_speed=2.5f;    // keypress translates the camera
        float zoom_speed=2.0f;    // scrollwheel zooms in/out the FoV
        float sensitivity=0.05f;   // mouse movement rotates the camera
        TransformComponent trans;
        SceneCamera Camera;
        bool Primary = true; // TODO: think about moving to Scene
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    private:
        //glm::vec3 position;  // camera position in world space in TransformComponent
        glm::vec3 forward= glm::vec3(0.0f, 0.0f, -1.0f);   // forward direction in world space
        glm::vec3 right  = glm::vec3(1.0f, 0.0f, 0.0f );     // right direction in world space
        glm::vec3 up     = glm::vec3(0.0f, 1.0f, 0.0f );        // up direction in world space
        float euler_x= 0.0f;       // euler angles around the x-axis pitch
        float euler_y= -90.0f;       // euler angles around the y-axis yaw
//glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f),
//        float euler_x = 0.0f, float euler_y = -90.0f);

    };






}