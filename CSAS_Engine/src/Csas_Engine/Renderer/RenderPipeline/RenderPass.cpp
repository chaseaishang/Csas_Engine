//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "RenderPass.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/RenderCommand.h"
namespace CsasEngine {

    void ForwardPass::PrepareRenderer()
    {



        brdfNode.OnPrepare(&m_data);
    }

    void ForwardPass::ExecuteRenderer()
    {

        brdfNode.OnExecute(&m_data);
    }


    ForwardPass::ForwardPass(RenderDataVec data,Ref<Framebuffer> render_Target,
                             CameraPtr m_camera,
                             SpotLightPtrVec m_spots
    )
    {
        m_data={
                .material=std::vector<Material_BaseBRDF*>(),
                .meshs=std::vector<MeshComponent*>(),
                .spots=m_spots,
                .fbo=render_Target,
                .camera=m_camera,
        };
        auto&meshvec=m_data.meshs;
        auto&material=m_data.material;
        for(auto&single:data)
        {
            meshvec.push_back(single.meshPtr);
            material.push_back(static_cast<Material_BaseBRDF*>(single.materialPtr));
        }
        brdfNode=BRDFPassNode(m_data);

    }


    ForwardPass::~ForwardPass()
    {

    }


}