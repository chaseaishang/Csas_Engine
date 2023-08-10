//
// Created by chaseaishang on 23-7-25.
//
//#include "Csas_Engine/Csaspch.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Texture.h"
#include "Material.h"
#include "Mesh.h"

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

    void Material_BaseBRDF::Update(glm::mat4 &model,glm::mat4 &CameraView,std::vector<SpotLightComponent>&spots)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
//        uniform float metallic;
//        uniform float roughness;
        m_Shader->SetFloat("metallic",metallic);
        m_Shader->SetFloat("roughness",roughness);
        if(int size=spots.size();size>=0)
        {
            m_Shader->SetInt("light_nums",size);
            for(int i=0;i<size;i++)
            {
                std::string light_position="lightPositions[" + std::to_string(i)+']';
                std::string light_color   ="lightColors["    + std::to_string(i)+']';
                glm::vec3 light_pos=spots[i].position;
                auto light_col=glm::vec3(spots[i].color);
                glm::vec3 position=glm::vec3(CameraView*glm::vec4(light_pos,0.0));
                m_Shader->SetFloat3(light_position,position);
                m_Shader->SetFloat3(light_color,light_col);
            }
        }
    }

    Material_Skybox::Material_Skybox(const std::string filename)
    {
        type=MaterialType::Skybox;
        //this->cube_map=CubeTexture::Create(filename);
        TextureSpecification spec;
                        spec.RGB= false;//hdr

                        //hdr_texture= sky30  satara_night_2k
        this->cube_map=CubeTexture::Create("./assets/textures/HDR/satara_night_2k.hdr",spec);
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