//
// Created by chaseaishang on 23-7-13.
//
#include "Csas_Engine/Csaspch.h"

#include "Scene.h"
#include "Csas_Engine/Component/Entity.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/Renderer2D.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Csas_Engine/Renderer/RenderPipeline/RenderPipeline.h"
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
namespace CsasEngine {


    Scene::Scene() {

    }

    Scene::~Scene()
    {
        m_Registry.clear();
    }

    Entity Scene::CreateEntity(const std::string& name)
    {

        Entity entity = { m_Registry.create(), this };
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

        std::vector<SpotLightComponent*>SpotlightsPtr;
        std::vector<DirectionLightComponent>Direction_lights;
        {
            auto view = m_Registry.view<MeshComponent,SpotLightComponent>();
            for(auto entity: view)
            {
                auto [mesh,spot]=view.get<MeshComponent,SpotLightComponent>(entity);

                Renderer3D::Submit(mesh,spot);

                SpotlightsPtr.push_back(&spot);
            }
        }
        {//light

            m_Registry.view<DirectionLightComponent>().each(
                    [&](auto entity, auto &direct)
                    {
                        Direction_lights.push_back(direct);

                    }
            );

        }
        Renderer3D::BeginScene(main_camera,SpotlightsPtr);



        {
            auto view = m_Registry.view<Material_Skybox, MeshComponent>();
            for(auto entity: view)
            {
                auto [material,mesh]=view.get<Material_Skybox,MeshComponent>(entity);
                Renderer3D::Submit(mesh,material);
            }
        }
        {
            auto view = m_Registry.view<Material_BaseBRDF, MeshComponent>();
            for(auto entity: view)
            {
                auto [material,mesh]=view.get<Material_BaseBRDF,MeshComponent>(entity);
                Renderer3D::Submit(mesh,material);
            }
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
        Camera *MainCamera;
        glm::mat4 cameraTransform;
        {
            auto view = m_Registry.view<CameraComponent>();
            for (auto entity : view)
            {
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Need & reference
            // otherwise the camera.Camera will be randomly initialize
                auto&  camera = view.get<CameraComponent>(entity);

                if (camera.Primary)
                {
                    MainCamera=&(camera.Camera);
                    break;
                }
            }

        }
        if (MainCamera)
        {
            if(m_2DScene)
            {
                On2DUpdate(*MainCamera,cameraTransform);
            }
            else
            {
                On3DUpdate(*MainCamera,cameraTransform);
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