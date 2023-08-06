//
// Created by chaseaishang on 23-7-25.
//

#pragma once


#include "glm/glm.hpp"
#include "light.h"
namespace CsasEngine {
    class Shader;

    enum class MaterialType
    {
        BasePrimitive,
        BasePBR,
        Cartoon,
        BaseBRDF,
        Skybox
    };
    struct Material
    {
    public:
        Material()=default;
        virtual ~Material()=default;
        MaterialType GetType(){return type;}
        Ref<Shader> GetShader(){return m_Shader;}
        std::string GetShader_name();
    protected:
        MaterialType type;
        //use shader library
        Ref<Shader>m_Shader;
        std::string Shader_name;
    };
    struct Material_BasePrimitive:public Material
    {
    public:

        Material_BasePrimitive();
        void Update(glm::mat4 & model);
        ~Material_BasePrimitive() override =default ;


    };
    struct Material_BasePBR:public Material
    {

        struct MaterialInfo {
            glm::vec3 Ka;
            glm::vec3 Kd;
            glm::vec3 Ks;
            float Shininess;
        };

    public:
        Material_BasePBR();
        void Update(glm::mat4 & CameraView,glm::mat4 & model,std::vector<SpotLightComponent>&spots,std::vector<DirectionLightComponent>&Direction_lights);
        ~Material_BasePBR() override =default ;
        MaterialInfo materialInfo;

    };
    struct Material_Cartoon:public Material
    {

        struct MaterialInfo {
            glm::vec3 Ka;
            glm::vec3 Kd;
            int size;
            float scaleFactor;
        };

    public:
        Material_Cartoon();
        void Update(glm::mat4 & CameraView,glm::mat4 & model,std::vector<SpotLightComponent>&spots);
        ~Material_Cartoon() override =default ;
        MaterialInfo materialInfo;

    };
    class CubeTexture;
    struct Material_Skybox:public Material
    {
    public:
        Material_Skybox(const std::string filename);
        Ref<CubeTexture>cube_map;
        void Update(glm::mat4 & model);
    };
    struct Material_BaseBRDF:public Material
    {
    public:
        struct MaterialInfo
        {
            glm::vec3 albedo=glm::vec3(0.5f, 0.0f, 0.0f);
            float ao=1.0f;
            float metallic=0.5f;
            float roughness=0.5f;
        };
        Material_BaseBRDF();
        void Update(glm::mat4 & model,glm::mat4 &CameraView,std::vector<SpotLightComponent>&spots);
        ~Material_BaseBRDF() override =default ;
        glm::vec3 lightPositions[4];

        glm::vec3 albedo=glm::vec3(0.5f, 0.0f, 0.0f);
        float ao=1.0f;
        float metallic=0.5f;
        float roughness=0.5f;

    };
}



