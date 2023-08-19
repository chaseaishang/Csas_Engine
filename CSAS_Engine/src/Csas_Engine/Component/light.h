//
// Created by chaseaishang on 23-7-25.
//

#pragma once
#include "Material.h"
#include "glm/glm.hpp"
namespace CsasEngine {


    struct LightComponent:public Material
    {

    public:
        LightComponent()=default;
        virtual ~LightComponent()=default;
    };
    struct SpotLightComponent:public LightComponent
    {
    public:
        glm::vec4 color={1,1,1,1};
        float intensity=1.0f;
    public:
        SpotLightComponent( glm::vec4 color={5.0f,5.0f,2.0f,1.0f},float intensity=1.0f);
        ~SpotLightComponent()=default;
        void Update(glm::mat4 & model);

    };

    //Position use as direction
    struct DirectionLightComponent:public LightComponent
    {
    public:
        glm::vec4 color={1,1,1,1};
        float intensity=1.0f;

    public:
        DirectionLightComponent( glm::vec4 color={5.0f,5.0f,2.0f,1.0f},float intensity=1.0f);
        ~DirectionLightComponent()=default;
    };


}
