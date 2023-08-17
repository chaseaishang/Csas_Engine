//
// Created by chaseaishang on 23-7-25.
//
//#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Texture.h"
#include "Material.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Mesh.h"
#include "light.h"
namespace CsasEngine {
    static ShaderLibrary shaderLibrary;
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
//            light.position = glm::vec3(CameraView * glm::vec4(light.position, 0));
//            m_Shader->SetFloat3("Light.Position", light.position);



        }
        {
            if(int size=Direction_lights.size();size>0)
            {
                m_Shader->SetBoolean("DirLightsEnable", true);
                auto direct_light=Direction_lights[0];
//                direct_light.position = glm::vec3(CameraView * glm::vec4(direct_light.position, 0));
//                m_Shader->SetFloat3("DirLights.direction", direct_light.position);
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
//            light.position = glm::vec3(CameraView * glm::vec4(light.position, 0));
//            m_Shader->SetFloat3("Light.position", light.position);

        }

    }


    Material_BaseBRDF::Material_BaseBRDF()
    {
        type=MaterialType::BaseBRDF;
//        if(!shaderLibrary.Exists("BaseBRDF"))
//        {
//            shaderLibrary.Load("BaseBRDF","./assets/shaders/environment_BRDF.glsl");
//        }
        m_Shader = Shader::Create("./assets/shaders/environment_BRDF.glsl");
        m_Shader->Bind();
        m_Shader->SetFloat("ao",ao);
        m_Shader->SetFloat3("albedo",albedo);

//        uniform float metallic;
//        uniform float roughness;




    }

    void Material_BaseBRDF::Update(glm::mat4 &model,glm::mat4 &CameraView,
                                   const CubeTexture&irradiance_map,
                                   const CubeTexture&prefiltered_map,
                                   const Texture2D&BRDF_LUT
                                   )
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
//        uniform float metallic;
//        uniform float roughness;
        m_Shader->SetFloat3("albedo",albedo);
        m_Shader->SetFloat("metallic",metallic);
        m_Shader->SetFloat("roughness",roughness);
        auto with1=irradiance_map.GetWidth();
        auto with2=prefiltered_map.GetWidth();
        auto width3=BRDF_LUT.GetWidth();
        irradiance_map.Bind(17);
        prefiltered_map.Bind(18);
        BRDF_LUT.Bind(19);

    }

    Material_Skybox::Material_Skybox(const std::string filename)
    {
        type=MaterialType::Skybox;

        this->cube_map=CubeTexture::Create(filename);
        if(IBL)
        {
            Renderer3D::Utils::PreComputeIBL(cube_map);

        }
        m_Shader=Shader::Create("./assets/shaders/skybox.glsl");
        
    }

    void Material_Skybox::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
        cube_map->Bind(0);


    }

    Material_Blur::Material_Blur()
    {
        type=MaterialType::Blur;
        m_Shader=Shader::Create("./assets/shaders/blur.glsl");

    }

    void Material_Blur::Update(Texture2D*texture)
    {
        m_Shader->Bind();
        texture->Bind(0);
    }




}