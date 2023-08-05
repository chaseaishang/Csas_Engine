//
// Created by chaseaishang on 23-7-26.
//

#include "Example3.h"

#include "ImGui/include/imgui.h"
namespace CsasEngine {
    void Example3::OnAttach()
    {
        CSAS_INFO("Switch to Example3");
        CSAS_PROFILE_FUNCTION();
        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

        m_ActiveScene = CreateRef<Scene>();
        m_Scene = m_ActiveScene;
        // Entity

        auto renderPipeline=RenderPipeline::getInstance();
        RenderIndex FirstIndex=renderPipeline->BeginPass();
        renderPipeline->SubmitPass(PassNodeType::BrdfPass);
        renderPipeline->EndPass();


        glm::vec3 lightPositions[4];
        lightPositions[0]={-10.0f,  10.0f, 10.0f};
        lightPositions[1]={ 10.0f,  10.0f, 10.0f};
        lightPositions[2]={-10.0f, -10.0f, 10.0f};
        lightPositions[3]={10.0f, -10.0f, 10.0f};
        for(int i=0;i<4;i++)
        {
            std::string name="Spotlight_"+std::to_string(i+1);
            SpotLights[i]=m_ActiveScene->CreateEntity(name);
            auto&spot=SpotLights[i].AddComponent<SpotLightComponent>(lightPositions[i]);
            spot.color={300,300,300,1};
        }


        // BRDF
        for(int i=0;i<9;i++)
        {
            std::string name="BRDF_Sphere_"+std::to_string(i+1);
            m_BRDF_Sphere[i]=m_ActiveScene->CreateEntity(name);
            RenderIndex index=FirstIndex;
            m_BRDF_Sphere[i].AddComponent<MeshComponent>(Primitive::Sphere,index);
            m_BRDF_Sphere[i].AddComponent<Material_BaseBRDF>();
        }
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                int index=i*3+j;
                auto &material=m_BRDF_Sphere[index].GetComponent<Material_BaseBRDF>();
                auto &metalness=material.metallic;
                metalness=(float)i / (float)3;
                auto &roughness=material.roughness;
                roughness=glm::clamp((float)j / (float)3, 0.05f, 1.0f);
                auto &position =m_BRDF_Sphere[index].GetComponent<MeshComponent>().transform.Translation;
                position={i,j,0};


            }
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

    void CsasEngine::Example3::OnImGuiRender() {


    }

    void CsasEngine::Example3::OnEvent(CsasEngine::Event &e) {
        auto &camera = m_CameraEntity.GetComponent<CameraComponent>().Camera;
        camera.OnEvent(e);
    }

}