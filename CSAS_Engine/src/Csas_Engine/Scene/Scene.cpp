//
// Created by chaseaishang on 23-7-13.
//
#include "Csas_Engine/Csaspch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Csas_Engine/Renderer/Renderer2D.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "SceneCamera.h"
#include "glm/glm.hpp"
namespace CsasEngine {


    Scene::Scene() {

    }

    Scene::~Scene() {
    }

    Entity Scene::CreateEntity(const std::string& name)
    {

        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        return entity;
    };
    void Scene::On2DUpdate(Camera &main_camera, glm::mat4& cameraTransform)
    {
        Renderer2D::BeginScene(main_camera, cameraTransform);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
        }

        Renderer2D::EndScene();
    }

    void Scene::On3DUpdate(Camera &main_camera, glm::mat4 &cameraTransform)
    {
        Renderer3D::BeginScene(main_camera, cameraTransform);
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer3D::DrawCube(transform.GetTransform(), sprite.Color);
        }

        Renderer3D::EndScene();
    }

    void Scene::OnUpdate(Timestep ts) {
        // Update scripts
        {






            m_Registry.view<NativeScriptComponent>().each(
                    [=](auto entity, auto& nsc)
          {
              if (!nsc.Instance)
              {
                  nsc.Instance = nsc.InstantiateScript();
                  nsc.Instance->m_Entity = Entity{ entity, this };

                  nsc.Instance->OnCreate();
              }

              nsc.Instance->OnUpdate(ts);
          });

        }
        // Render 2D
        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.Primary)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }

        }
        if (mainCamera)
        {
            if(m_2DScene)
            {
                On2DUpdate(*mainCamera,cameraTransform);
            }
            else
            {
                On3DUpdate(*mainCamera,cameraTransform);
            }

        }


    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        // Resize our non-FixedAspectRatio cameras
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }

    }




}