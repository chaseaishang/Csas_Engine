//
// Created by chaseaishang on 23-8-2.
//
#include "Csas_Engine/Csaspch.h"
#include "RenderPass.h"
#include "PassNode.h"
#include "Csas_Engine/Component/AllComponent.h"
#include "Csas_Engine/Renderer/RenderCommand.h"
#include "Csas_Engine/Renderer/UniformBuffer.h"
namespace CsasEngine {
    namespace GlobalCameraSpec
    {
        glm::mat4 ViewProjMatrix[2];
    }
    void ForwardPass::PrepareRenderer()
    {

        CameraUBO=UniformBuffer::Create(sizeof(GlobalCameraSpec::ViewProjMatrix),0);

        //for loop
        for(auto &[index,renderWrap]:renderMap)
        {
            auto &data=renderWrap.dataVec;
            for(const auto&node:renderWrap.passNodeVec)
            {
                node->OnPrepare(&data);
            }
        }

    }

    void ForwardPass::ExecuteRenderer()
    {
        CameraUBO->SetData(GlobalCameraSpec::ViewProjMatrix,sizeof(GlobalCameraSpec::ViewProjMatrix));
        this->render_Target->Bind();

        RenderCommand::Clear();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        for(auto &[index,renderWrap]:renderMap)
        {

            if(auto &data=renderWrap.dataVec;
                !data.spots.empty())
            {
                for(const auto&node:renderWrap.passNodeVec)
                {
                    node->OnExecute(&data);
                }
            }



        }
        this->render_Target->Unbind();
    }


    ForwardPass::~ForwardPass()
    {
        for(auto &[render,vec]:renderMap)
        {
            for(auto&pass:vec.passNodeVec)
            {
                delete pass;
            }
        }
    }

    void ForwardPass::SubmitRenderer(const RenderDataVec&data,RenderIndex index)
    {
        auto&datavec=renderMap[index].dataVec;

        datavec={
                .data_vec=std::move(data),
                .spots=m_spots
        };


        //datavec=std::move(data);
    }

    void ForwardPass::SetConstData(const Ref<Framebuffer>& render_Target,
                                   const CameraPtr& m_camera,
                                   const SpotLightPtrVec& m_spots)
    {

        this->m_spots=m_spots;
        this->render_Target=render_Target;
        this->m_camera=m_camera;
        GlobalCameraSpec::ViewProjMatrix[0]=m_camera->GetView();
        GlobalCameraSpec::ViewProjMatrix[1]=m_camera->GetProjection();


    }

    void ForwardPass::AddPass(ForwardPass::RenderIndex index, PassNodeType type)
    {
        auto&vec=renderMap[index].passNodeVec;
        switch (type)
        {
            case PassNodeType::BrdfPass:
            {

                vec.push_back(new BRDFPassNode());
                break;
            }

        }
    }

    glm::mat4 &ForwardPass::get_CameraView()
    {
        return GlobalCameraSpec::ViewProjMatrix[0];
    }


}