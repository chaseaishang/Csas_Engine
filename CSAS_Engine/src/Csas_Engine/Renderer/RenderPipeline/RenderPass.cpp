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


    ForwardPass::ForwardPass(ForwardPass::MeshVector &meshs, ForwardPass::MaterialVector &material,
                             Ref<Framebuffer> render_Target,
                             CameraPtr m_camera,
                             SpotLightPtrVec m_spots
    )
    :meshs(meshs),material(material)
    {
        //std::vector<Material_BaseBRDF*> material;
        //            std::vector<MeshComponent*>meshs;
        //            std::vector<SpotLightComponent*>spots;
        //            Ref<Framebuffer>fbo;
        //            CameraPtr camera;
        m_data={
                        .material=material,
                        .meshs=meshs,
                        .spots=m_spots,
                        .fbo=render_Target,
                        .camera=m_camera,

                    };

        brdfNode=BRDFPassNode(m_data);

    }


    ForwardPass::~ForwardPass()
    {

    }


}