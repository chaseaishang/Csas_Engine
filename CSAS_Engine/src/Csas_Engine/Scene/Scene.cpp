//
// Created by chaseaishang on 23-7-13.
//
#include "Csas_Engine/Csaspch.h"
#include <algorithm>
#include "Scene.h"
#include "Csas_Engine/Component/Entity.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/Renderer2D.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Csas_Engine/Renderer/RenderPipeline/RenderPipeline.h"
#include "SceneCamera.h"
#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"
#include "Node.h"
namespace CsasEngine {


    Scene::Scene()
    {
        roots.emplace_back("root");

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
        roots[0].AddEntity(entity);
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
        std::vector<MeshComponent<Vertex> *> SpotMeshPtr;

        std::vector<DirectionLightComponent*>Direction_lightsPtr;
        std::vector<MeshComponent<Vertex> *> DirectMeshPtr;
        {
            auto view = m_Registry.view<MeshComponent<Vertex>,SpotLightComponent>();
            for(auto entity: view)
            {
                auto [mesh,spot]=view.get<MeshComponent<Vertex>,SpotLightComponent>(entity);

                Renderer3D::Submit(mesh,spot);

                SpotlightsPtr.push_back(&spot);
                SpotMeshPtr.push_back(&mesh);
            }
        }
        {//light

            m_Registry.view<MeshComponent<Vertex>,DirectionLightComponent>().each(
                    [&](auto &mesh, auto &direct)
                    {

                        Direction_lightsPtr.push_back(&direct);
                        DirectMeshPtr.push_back(&mesh);

                    }
            );

        }
        Renderer3D::BeginScene(main_camera,
                               SpotlightsPtr,SpotMeshPtr,
                               Direction_lightsPtr,
                               DirectMeshPtr
                               );



        {
            auto view = m_Registry.view<Material_Skybox, MeshComponent<Vertex>>();
            for(auto entity: view)
            {
                auto [material,mesh]=view.get<Material_Skybox,MeshComponent<Vertex>>(entity);
                Renderer3D::Submit(mesh,material);
            }
        }
        {
            auto view = m_Registry.view<Material_BaseBRDF, MeshComponent<Vertex>>();
            for(auto entity: view)
            {
                auto [material,mesh]=view.get<Material_BaseBRDF,MeshComponent<Vertex>>(entity);
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

    uint Scene::AddGroup(std::string name)
    {
        static int group_index=1;//0 is for root
         roots.emplace_back(name);
        return group_index++;
    }

    Entity Scene::CreateEntity(uint group_index, const std::string &name)
    {

        CSAS_CORE_ASSERT(group_index<roots.size(),"Error index!");
        Entity entity = { m_Registry.create(), this };
        auto& tag = entity.AddComponent<TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;
        roots[group_index].AddEntity(entity);
        return entity;
    }

    const std::vector<Node> &Scene::getRoots() const {
        return roots;
    }


}