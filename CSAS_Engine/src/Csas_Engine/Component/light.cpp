//
// Created by chaseaishang on 23-7-25.
//

#include "light.h"
#include "Mesh.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
namespace CsasEngine {

    SpotLightComponent::SpotLightComponent(glm::vec4 color, float intensity)
            :color(color),intensity(intensity)
    {
        type=MaterialType::Light;
        m_Shader=Shader::Create("./assets/shaders/light.glsl");
    }
    void SpotLightComponent::Update(glm::mat4 &model)
    {
        m_Shader->Bind();
        m_Shader->SetMat4("model",model);
        m_Shader->SetFloat4("light_color",color);
        m_Shader->SetFloat("light_intensity",intensity);


    }
}