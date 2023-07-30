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
        BasePBR
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
        struct SpotLightInfo
        {
            uint8_t nums;
            glm::vec3 *Las;
            glm::vec3 *Lds;
            glm::vec3 *Lss;
        };

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
}



