//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
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
/////////////////////////////////
////////////////////////////////BRDFPassNode
///////////////////////////////
    void BRDFPassNode::OnPrepare(PassData*data)
    {


    }

    void BRDFPassNode::OnExecute(PassData*data)
    {
        // state switch
        auto ptr=static_cast<BRDFPassData*>(data);

        for(auto &[mesh,material]:ptr->data_vec)
        {
            auto &vao=mesh->m_VAO;
            auto transform=mesh->transform.GetTransform();
            mesh->Update();
            vao->Bind();
            static_cast<Material_BaseBRDF*>(material)->Update(transform,ForwardPass::get_CameraView(),
                                                              *ptr->irradiance_map,
                                                              *ptr->prefiltered_map,
                                                              *ptr->BRDF_LUT
                                                              );

            RenderCommand::DrawIndexed(vao);
        }



    }

    BRDFPassNode::BRDFPassNode(BRDFPassData&data)
    {

    }

    BRDFPassNode::~BRDFPassNode() {

    }
/////////////////////////////////
////////////////////////////////SkyboxPassNode
///////////////////////////////
    SkyboxPassNode::~SkyboxPassNode() {

    }

    void SkyboxPassNode::OnPrepare(PassData *data) {

    }

    void SkyboxPassNode::OnExecute(PassData *data)
    {
        auto ptr=static_cast<SkyboxPassData*>(data);
        auto&mesh=ptr->data.meshPtr;
        auto*material=static_cast<Material_Skybox*>( ptr->data.materialPtr);

        auto &vao=mesh->m_VAO;
        auto transform=mesh->transform.GetTransform();
        mesh->Update();
        vao->Bind();
        material->Update(transform);
        RenderCommand::DrawIndexed(vao);
    }

    /////////////////////////////////
    ////////////////////////////////BlurPassNode
    ///////////////////////////////
    void BlurPassNode::OnPrepare(PassData *data)
    {

    }

    void BlurPassNode::OnExecute(PassData *data)
    {
        auto ptr=static_cast<BlurPassData*>(data);
        auto*texture=ptr->source_tex;

        auto&material=ptr->blur_material;
        material->Update(texture);
        Renderer3D::DrawQuad();

    }

    void LightPassNode::OnPrepare(PassData *data) {

    }

    void LightPassNode::OnExecute(PassData *data)
    {
        // state switch
        auto ptr=static_cast<LightPassData*>(data);
        for(auto &[mesh,material]:ptr->data_vec)
        {
            auto &vao=mesh->m_VAO;
            auto transform=mesh->transform.GetTransform();
            mesh->Update();
            vao->Bind();
            static_cast<SpotLightComponent*>(material)->Update(transform);

            RenderCommand::DrawIndexed(vao);
        }

    }
}