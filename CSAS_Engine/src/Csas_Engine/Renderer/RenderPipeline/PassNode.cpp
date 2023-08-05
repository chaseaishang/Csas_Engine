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
#include "RenderPass.h"
namespace CsasEngine
{

    void BRDFPassNode::OnPrepare(PassData*data)
    {


    }

    void BRDFPassNode::OnExecute(PassData*data)
    {
        // state switch
        auto ptr=static_cast<BRDFPassData*>(data);





        auto& Spotlights=ptr->spots;
        std::vector<SpotLightComponent> spotlights;
        for(auto&spot:Spotlights)
        {
            spotlights.push_back(*spot);
        }
        for(auto &[mesh,material]:ptr->data_vec)
        {
            auto &vao=mesh->m_VAO;
            auto transform=mesh->transform.GetTransform();
            mesh->Update();
            vao->Bind();
            static_cast<Material_BaseBRDF*>(material)->Update(transform,ForwardPass::get_CameraView(),spotlights);

            RenderCommand::DrawIndexed(vao);
        }



    }

    BRDFPassNode::BRDFPassNode(BRDFPassData&data)
    {

    }

    BRDFPassNode::~BRDFPassNode() {

    }
}