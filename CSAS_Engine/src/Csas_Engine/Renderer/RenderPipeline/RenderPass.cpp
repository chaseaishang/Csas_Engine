//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "RenderPass.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
namespace CsasEngine {

    void ForwardPass::PrepareRenderer()
    {
        //gl clear color
        for(int i=0;i<meshs.size();i++)
        {
            brdfNodes[i].OnPrepare(&datas[i]);
        }

        //brdfNode->OnPrepare(&data);
    }

    void ForwardPass::ExecuteRenderer()
    {

        //brdfNode->OnExecute();
    }


    ForwardPass::ForwardPass(ForwardPass::MeshVector &meshs, ForwardPass::MaterialVector &material)
    :meshs(meshs),material(material)
    {
        for(int i=0;i<meshs.size();i++)
        {
            brdfNodes.push_back(BRDFPassNode());
            datas.push_back(
                    DataStruct
                    {
                        .material=&material[i],
                        .ptr_spot_vec=nullptr
                    }
                    );
        }
    }


    ForwardPass::~ForwardPass()
    {

    }


}