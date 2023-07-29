//
// Created by chaseaishang on 23-7-25.
//

#pragma once
#include "glm/glm.hpp"
namespace CsasEngine {


    struct LightComponent
    {
    public:
        glm::vec3 position={5.0f,5.0f,2.0f};
    public:
        LightComponent(glm::vec3 position):position(position){};
        LightComponent()=default;
        virtual ~LightComponent()=default;
    };
    struct SpotLightComponent:public LightComponent
    {
    public:
        glm::vec4 color={1,1,1,1};// @TODO temp
    public:

        glm::vec3 La={ 0.4f, 0.4f, 0.4f};
        glm::vec3 Ld{1.0f, 1.0f, 1.0f};
        glm::vec3 Ls{1.0f, 1.0f, 1.0f};
    public:
        SpotLightComponent( glm::vec3 position={5.0f,5.0f,2.0f})
            :LightComponent(position){};
        ~SpotLightComponent()=default;
    };


}
