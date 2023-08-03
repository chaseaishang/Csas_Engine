//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/RenderCommand.h"
#include "Csas_Engine/Renderer/Framebuffer.h"
#include "Csas_Engine/Renderer/UniformBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/string_cast.hpp"
namespace CsasEngine
{
    namespace GlobalCameraSpec
    {
        glm::mat4 ViewProjMatrix[2];
    }
    void BRDFPassNode::OnPrepare(PassData*data)
    {
       CameraUBO=UniformBuffer::Create(sizeof(GlobalCameraSpec::ViewProjMatrix),0);

    }

    void BRDFPassNode::OnExecute(PassData*data)
    {
        // state switch
        auto ptr=static_cast<BRDFPassData*>(data);
        auto& fbo=ptr->fbo;
        fbo->Bind();
        auto&camera=ptr->camera;
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        RenderCommand::Clear();

        GlobalCameraSpec::ViewProjMatrix[0]=camera->GetView();
        GlobalCameraSpec::ViewProjMatrix[1]=camera->GetProjection();

        CameraUBO->SetData(glm::value_ptr(GlobalCameraSpec::ViewProjMatrix[0]),sizeof(GlobalCameraSpec::ViewProjMatrix));
        auto& Spotlights=ptr->spots;
        std::vector<SpotLightComponent> spotlights;
        for(auto&spot:Spotlights)
        {
            spotlights.push_back(*spot);
        }
        for(int i=0;i<ptr->meshs.size();i++)
        {
            auto&mesh=ptr->meshs[i];
            auto &material=ptr->material[i];

            auto &vao=mesh->m_VAO;
            auto transform=mesh->transform.GetTransform();
            mesh->Update();
            //material Update
            material->Update(transform,GlobalCameraSpec::ViewProjMatrix[0],spotlights);
            vao->Bind();
            RenderCommand::DrawIndexed(vao);
        }

        fbo->Unbind();
    }

    BRDFPassNode::BRDFPassNode(BRDFPassData&data)
    {

    }

    BRDFPassNode::~BRDFPassNode() {

    }
}