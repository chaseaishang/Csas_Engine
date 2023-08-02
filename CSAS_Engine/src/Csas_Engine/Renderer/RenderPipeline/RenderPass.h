//
// Created by chaseaishang on 23-8-2.
//

#pragma once

#include "PassNode.h"

namespace CsasEngine
{

    struct Material_BaseBRDF;
    struct MeshComponent;

    class RenderPass
    {
    public:
        RenderPass()=default;
        virtual ~RenderPass()=default;
        virtual void PrepareRenderer()=0;
        virtual void ExecuteRenderer()=0;
//        virtual void SubmitRenderer()=0;
//        virtual void FinishRenderer()=0;
    };
   //dispatch
    class ForwardPass : public RenderPass
    {
    public:
        using MeshVector=std::vector<MeshComponent>;
        using MaterialVector=std::vector<Material_BaseBRDF>;
        using brdfNodeVector=std::vector<BRDFPassNode>;
        using brdfNodeDataVector=std::vector<BRDFPassNode::BRDFPassData>;
        using DataStruct=BRDFPassNode::BRDFPassData;
        ForwardPass(MeshVector&meshs,MaterialVector&material);
        ~ForwardPass()override;
        void PrepareRenderer()override;
        void ExecuteRenderer()override;
        //light info? ok


    private:
        brdfNodeVector brdfNodes;
        MeshVector&meshs;
        MaterialVector&material;
        brdfNodeDataVector datas;
        //
        //mesh data vector
        //material
    };

}