//
// Created by chaseaishang on 23-7-25.
//
//#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Material.h"
#include "Mesh.h"

namespace CsasEngine {
    std::string Material::GetShader_name()
    {
        return m_Shader->GetName();
    }
    Material_BasePrimitive::Material_BasePrimitive()
    {
        type = MaterialType::BasePrimitive;
        m_Shader = Shader::Create("./assets/shaders/BasePrimitive.glsl");



    }

    void Material_BasePrimitive::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////Material_BasePBR//////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////


    Material_BasePBR::Material_BasePBR()
    {
        type = MaterialType::BasePBR;
        m_Shader = Shader::Create("./assets/shaders/ADS_Light.glsl");
        materialInfo.Ka={0.9f, 0.5f, 0.3f};
        materialInfo.Kd={0.9f, 0.5f, 0.3f};
        materialInfo.Ks={0.8f, 0.8f, 0.8f};
        materialInfo.Shininess=100.0f;
    }

    void Material_BasePBR::Update(glm::mat4 & CameraView,glm::mat4 &model,
        std::vector<SpotLightComponent>&spots,std::vector<DirectionLightComponent>&Direction_lights)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);

        {
            m_Shader->SetFloat3("Material.Ka", materialInfo.Ka);
            m_Shader->SetFloat3("Material.Kd", materialInfo.Kd);
            m_Shader->SetFloat3("Material.Ks", materialInfo.Ks);
            m_Shader->SetFloat("Material.Shininess", materialInfo.Shininess);
        }
        {
            auto &light = spots[0];// temp
            light.position = glm::vec3(CameraView * glm::vec4(light.position, 0));
            m_Shader->SetFloat3("Light.Position", light.position);
            m_Shader->SetFloat3("Light.La", light.La);
            m_Shader->SetFloat3("Light.Ld", light.Ld);
            m_Shader->SetFloat3("Light.Ls", light.Ls);


        }
        {
            if(int size=Direction_lights.size();size>0)
            {
                m_Shader->SetBoolean("DirLightsEnable", true);
                auto direct_light=Direction_lights[0];
                direct_light.position = glm::vec3(CameraView * glm::vec4(direct_light.position, 0));
                m_Shader->SetFloat3("DirLights.direction", direct_light.position);
                m_Shader->SetFloat3("DirLights.ambient", direct_light.La);
                m_Shader->SetFloat3("DirLights.diffuse", direct_light.Ld);
                m_Shader->SetFloat3("DirLights.specular", direct_light.Ls);
            }
            else
            {
                m_Shader->SetBoolean("DirLightsEnable",false);
            }


        }

    }
    /////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////Material_Cartoon//////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////
    Material_Cartoon::Material_Cartoon()
    {
        type = MaterialType::Cartoon;
        m_Shader = Shader::Create("./assets/shaders/Cartoon.glsl");
        materialInfo.Ka={0.9f, 0.5f, 0.3f};
        materialInfo.Kd={0.9f, 0.5f, 0.3f};

    }
    void Material_Cartoon::Update(glm::mat4 &CameraView, glm::mat4 &model, std::vector<SpotLightComponent> &spots)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
        {
            m_Shader->SetFloat3("Material.Ka", materialInfo.Ka);
            m_Shader->SetFloat3("Material.Kd", materialInfo.Kd);
        }
        {
            auto &light = spots[0];// temp
            light.position = glm::vec3(CameraView * glm::vec4(light.position, 0));
            m_Shader->SetFloat3("Light.position", light.position);
            m_Shader->SetFloat3("Light.intensity", light.La);
        }

    }


    Material_BaseBRDF::Material_BaseBRDF()
    {
        type=MaterialType::BaseBRDF;
        m_Shader = Shader::Create("./assets/shaders/environment_BRDF.glsl");
        m_Shader->Bind();
        m_Shader->SetFloat("ao",ao);
        m_Shader->SetFloat3("albedo",albedo);

//        uniform float metallic;
//        uniform float roughness;


        lightPositions[0]={-10.0f,  10.0f, 10.0f};
        lightPositions[1]={ 10.0f,  10.0f, 10.0f};
        lightPositions[2]={-10.0f, -10.0f, 10.0f};
        lightPositions[3]={10.0f, -10.0f, 10.0f};
        lightColors[0]={300.0f, 300.0f, 300.0f};
        lightColors[1]={300.0f, 300.0f, 300.0f};
        lightColors[2]={300.0f, 300.0f, 300.0f};
        lightColors[3]={300.0f, 300.0f, 300.0f};
    }

    void Material_BaseBRDF::Update(glm::mat4 &model,glm::mat4 &CameraView)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
//        uniform float metallic;
//        uniform float roughness;
        m_Shader->SetFloat("metallic",metallic);
        m_Shader->SetFloat("roughness",roughness);

        for(int i=0;i<4;i++)
        {
            std::string light_position="lightPositions[" + std::to_string(i)+']';
            std::string light_color   ="lightColors["    + std::to_string(i)+']';
            glm::vec3 position=glm::vec3(CameraView*glm::vec4(lightPositions[i],0.0));
            m_Shader->SetFloat3(light_position,position);
            m_Shader->SetFloat3(light_color,lightColors[i]);
        }



    }
}