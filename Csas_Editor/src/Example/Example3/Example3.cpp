//
// Created by chaseaishang on 23-7-26.
//

#include "Example3.h"
#include "../utils/math.h"
#include "ImGui/include/imgui.h"
namespace CsasEngine {
    void Example3::OnAttach()
    {
        CSAS_INFO("Switch to Example3");
        CSAS_PROFILE_FUNCTION();
//          1300 fps now 8/20 debug
//          Well As we have  refactoring the transform  .So it needn't to multi each frame.Only in dirty.
        Application::Get().GetWindow().SetVSync(false);

        m_ActiveScene = CreateRef<Scene>();
        m_Scene = m_ActiveScene;
        // Entity

        auto renderPipeline=RenderPipeline::getInstance();
        renderPipeline->BeginPass();
        RenderIndex FirstIndex=renderPipeline->SubmitPass(PassNodeType::Skybox);
        RenderIndex secondIndex=renderPipeline->SubmitPass(PassNodeType::BrdfPass);
        RenderIndex thirdIndex=renderPipeline->SubmitPass(PassNodeType::LightPass);
        RenderIndex fourthIndex=renderPipeline->SubmitPass(PassNodeType::ParticlePass);
        //renderPipeline->SubmitPass(PassNodeType::BlurPass);
        renderPipeline->EndPass();
        //m_particle
        uint sphereGroup=m_ActiveScene->AddGroup("Brdf Sphere");
        auto lightGroup=m_ActiveScene->AddGroup("Lights");
        auto SystemGroup=m_ActiveScene->AddGroup("Systems");
        skybox=m_ActiveScene->CreateEntity("Skybox");///bluesky
        skybox.AddComponent<Material_Skybox>("./assets/textures/HDR/newport_loft.hdr");
        skybox.AddComponent<MeshComponent_Vertex>(Primitive::Cube,FirstIndex);
        glm::vec3 lightPositions[4];
        lightPositions[0]={-10.0f,  10.0f, 10.0f};
        lightPositions[1]={ 10.0f,  10.0f, 10.0f};
        lightPositions[2]={-10.0f, -10.0f, 10.0f};
        lightPositions[3]={10.0f, -10.0f, 10.0f};
        for(int i=0;i<4;i++)
        {

            std::string name="Spotlight_"+std::to_string(i+1);

            SpotLights[i]=m_ActiveScene->CreateEntity(lightGroup,name);

            auto&Trans=SpotLights[i].AddComponent<MeshComponent_Vertex>(Primitive::Cube,thirdIndex).transform;
            Trans.SetPosition(lightPositions[i]);
            auto&scale=Trans.Scale;
            scale={0.1,0.1,0.1};
            Trans.SetDirty();
            auto&spot=SpotLights[i].AddComponent<SpotLightComponent>();
            float ksi=0.2;
            auto rgb=Utils::math::HSL2RGB(ksi, 0.7f + ksi * 0.3f, 0.4f + ksi * 0.2f);

            spot.color={rgb,1};
            spot.intensity=300;

        }
        DirectLight=m_ActiveScene->CreateEntity(lightGroup,"DirectLight");
        auto&direct=DirectLight.AddComponent<MeshComponent_Vertex>(Primitive::UnRender).transform;
        direct.Translate({0,0,3},TransformComponent::Space::World);
        direct.Rotate({1,0,0},-49,TransformComponent::Space::World);
        DirectLight.AddComponent<DirectionLightComponent>(
                glm::vec4(0.0f, 0.43f, 1.0f,1.0),
                10
                );

        m_plane=m_ActiveScene->CreateEntity("plane");///bluesky
        m_plane.AddComponent<Material_BaseBRDF>();
        auto&trans=m_plane.AddComponent<MeshComponent_Vertex>(Primitive::Plane,secondIndex).transform;
        trans.Scale={5,5,5};
        auto &plane_material=m_plane.GetComponent<Material_BaseBRDF>();
        plane_material.roughness=0.5f;
        plane_material.metallic=0.5;

        auto rgb=Utils::math::HSL2RGB(0.2f,1.0f,0.6f);
        plane_material.albedo={rgb};
        m_plane.GetComponent<MeshComponent_Vertex>().transform.SetPosition({ 0,
                                                                      -0.6,
                                                                      0});


        // BRDF
        for(int i=0;i<9;i++)
        {
            std::string name="BRDF_Sphere_"+std::to_string(i+1);
            m_BRDF_Sphere[i]=m_ActiveScene->CreateEntity(sphereGroup,name);
            RenderIndex index=secondIndex;
            auto&trans=m_BRDF_Sphere[i].AddComponent<MeshComponent_Vertex>(Primitive::Sphere,index);
            //trans.transform.Rotate(glm::vec3(1,0,0),45);
            m_BRDF_Sphere[i].AddComponent<Material_BaseBRDF>();
        }
        float spacing = 1.2;

        for(int i=0;i<3;i++)
        {
            float metallic=(float)i / (float)7;
            for(int j=0;j<3;j++)
            {
                int index=i*3+j;
                auto &material=m_BRDF_Sphere[index].GetComponent<Material_BaseBRDF>();
                auto &metalness=material.metallic;
                metalness=metallic;
                auto &roughness=material.roughness;
                roughness=glm::clamp((float)j / (float)7, 0.05f, 1.0f);
                auto &trans =m_BRDF_Sphere[index].GetComponent<MeshComponent_Vertex>().transform;
                trans.SetPosition({(j - (3 / 2)) * spacing,
                                   0.0f,
                                   -i * spacing*2});


            }
        }
        m_particle=m_ActiveScene->CreateEntityForSys("Particle");
        m_particle.AddComponent<Particles>(glm::vec3{0.05,0.05,0.05f},10);
        auto&particles=m_particle.GetComponent<Particles>();

        auto&Particle_trans=m_particle.AddComponent<MeshComponent_ParticleVertex>(50,500,
                                                              Primitive::Particle,
                                                              fourthIndex).transform;
        Particle_trans.SetPosition(glm::vec3{0,-0.5,0.9});

        {
            auto&sys=m_ActiveScene->CreateParticleSystem();
            sys.Mount(m_particle);
        }


        //Camera
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");


        auto &cc = m_CameraEntity.AddComponent<CameraComponent>(glm::vec3{0, 0, 3});
        cc.Primary = true;
        cc.Camera.SetProjectionType(SceneCamera::ProjectionType::Perspective);
        float mousex = Input::GetMouseX();
        float mousey = Input::GetMouseY();
        cc.Camera.SetMousePosition(mousex, mousey);
        cc.Camera.SetCamera3D(&cc);


    }

    void CsasEngine::Example3::OnDetach() {

    }

    void CsasEngine::Example3::Update(CsasEngine::Timestep ts) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.Update(ts);



    }

    void CsasEngine::Example3::OnImGuiRender()
    {
        ImGui::Begin("Debug");
        if(ImGui::Button("RotateX"))
        {
            auto angle=(30);

            auto&trans=DirectLight.GetComponent<MeshComponent_Vertex>().transform;
            trans.Rotate({1,0,0},-angle,TransformComponent::Space::World);
        }
        if(ImGui::Button("RotateY"))
        {
            auto angle=(30);

            auto&trans=DirectLight.GetComponent<MeshComponent_Vertex>().transform;
            trans.Rotate({0,1,0},-angle,TransformComponent::Space::World);
        }

        ImGui::End();

    }

    void CsasEngine::Example3::OnEvent(CsasEngine::Event &e) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.OnEvent(e);
    }

}