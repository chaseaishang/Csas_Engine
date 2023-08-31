//
// Created by chaseaishang on 23-8-23.
//
#include "Csas_Engine/Csaspch.h"
#include "ParticlePassNode.h"
#include "PassNode.h"
#include "Csas_Engine/Renderer/Shader.h"
#include "Csas_Engine/Renderer/Renderer3D.h"
#include "Csas_Engine/Renderer/RenderCommand.h"

void CsasEngine::ParticlePassNode::OnPrepare(CsasEngine::PassData *data) {

}

void CsasEngine::ParticlePassNode::OnExecute(CsasEngine::PassData *data)
{
// state switch
    auto ptr=static_cast<ParticlePassData*>(data);
    for(auto &[mesh,material]:ptr->data)
    {
        auto meshVertex=static_cast<MeshComponent_ParticleVertex*>(mesh);
        auto &vao=meshVertex->m_VAO;
        auto transform=mesh->transform.GetTransform();
        meshVertex->Update();
        vao->Bind();
        auto material_particle=static_cast<Particles*>(material);
        material_particle->Update(transform);
        uint number=material_particle->live_count;
        RenderCommand::DrawPoints(vao,number);
        number=material_particle->gpu_Live_count;
        material_particle->UpdateGpu_particle(transform);
        RenderCommand::DrawPoints(vao,number);

        //static_cast<Particles*>(material)->Update(transform);


    }

}
