//
// Created by chaseaishang on 23-7-25.
//

#pragma once
#include "glm/glm.hpp"
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
    protected:
        MaterialType type;
        Ref<Shader>m_Shader;
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
        struct LightInfo {
            glm::vec3 Position;
            glm::vec3 La;
            glm::vec3 Ld;
            glm::vec3 Ls;
        };
        struct MaterialInfo {
            glm::vec3 Ka;
            glm::vec3 Kd;
            glm::vec3 Ks;
            float Shininess;
        };
    public:
        Material_BasePBR();
        void Update(glm::mat4 & model);
        ~Material_BasePBR() override =default ;
        LightInfo light;
        MaterialInfo materialInfo;

    };
}



