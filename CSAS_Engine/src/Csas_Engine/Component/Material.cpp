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


    Material_BasePBR::Material_BasePBR()
    {
        type = MaterialType::BasePBR;
        //ADS_Light_two_side
        //ADS_Light
        m_Shader = Shader::Create("./assets/shaders/ADS_Light.glsl");
        materialInfo.Ka={0.9f, 0.5f, 0.3f};
        materialInfo.Kd={0.9f, 0.5f, 0.3f};
        materialInfo.Ks={0.8f, 0.8f, 0.8f};
        materialInfo.Shininess=100.0f;
    }

    void Material_BasePBR::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
        m_Shader->SetFloat3("Light.Position",light.Position);
        m_Shader->SetFloat3("Light.La",light.La);
        m_Shader->SetFloat3("Light.Ld",light.Ld);
        m_Shader->SetFloat3("Light.Ls",light.Ls);

        m_Shader->SetFloat3("Material.Ka",materialInfo.Ka);
        m_Shader->SetFloat3("Material.Kd",materialInfo.Kd);
        m_Shader->SetFloat3("Material.Ks",materialInfo.Ks);
        m_Shader->SetFloat("Material.Shininess",materialInfo.Shininess);

    }



}