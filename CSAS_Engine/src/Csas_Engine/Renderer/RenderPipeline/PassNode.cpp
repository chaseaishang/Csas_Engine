//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
namespace CsasEngine {

    void BRDFPassNode::OnPrepare(PassData*data)
    {
        auto ptr=static_cast<BRDFPassData*>(data);
        auto material_ptr=ptr->material;
        auto spot_vec=*(ptr->ptr_spot_vec);
        auto&color=spot_vec[0].color;
    }

    void BRDFPassNode::OnExecute()
    {

        //        auto transform;
        //        mesh.Update();
        //        //material Update
        //        material.Update(transform,CameraSpec::ViewProjMatrix[0],Spotlights);
        //        vao->Bind();
        //        RenderCommand::DrawIndexed(vao);

    }

    BRDFPassNode::BRDFPassNode() {

    }

    BRDFPassNode::~BRDFPassNode() {

    }
}