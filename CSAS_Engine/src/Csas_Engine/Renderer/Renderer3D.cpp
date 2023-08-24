//
// Created by chaseaishang on 23-7-15.
//
#include "Csas_Engine/Csaspch.h"
#include <tuple>//@TODO remove to pch
#include "Renderer3D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "RenderCommand.h"
#include "./RenderPipeline/RenderPipeline.h"
#include "RenderPipeline/RenderData/RenderData.h"
#include "Csas_Engine/Renderer/Texture.h"
#include <glm/gtc/type_ptr.hpp>
namespace CsasEngine {
    static bool Debug= false;

    static std::unordered_map<RenderIndex,RenderDataVec>Render_map;
    namespace Pbr_data
    {
        Ref<CubeTexture>irradiance_map= nullptr;
        Ref<CubeTexture>prefiltered_map= nullptr;
        Ref<Texture2D>BRDF_LUT= nullptr;
        //prefiltered_map BRDF_LUT
    }
    void Renderer3D::BeginScene(Camera &camera,
                                std::vector<SpotLightComponent *> SpotlightsPtr,
                                std::vector<MeshComponent *> SpotMeshPtr,
                                std::vector<DirectionLightComponent*>DirectSpot,
                                std::vector<MeshComponent *> DirectMeshPtr
                                )
    {
        auto m_pipeline=RenderPipeline::getInstance();
        Light_Data data={
                SpotlightsPtr,SpotMeshPtr,DirectSpot,DirectMeshPtr
        };

        m_pipeline->BeginPipeline(&camera,data,
                                  Pbr_data::irradiance_map.get(),
                                  Pbr_data::prefiltered_map.get(),
                                     Pbr_data::BRDF_LUT.get()
        );
    }
    void Renderer3D::Submit(MeshComponent_Vertex&mesh,Material_BaseBRDF&material)
    {

        uint8_t index=mesh.RenderIndex;
        auto&vec=Render_map[index];
        vec.push_back({&mesh,&material});

    }

    void Renderer3D::Submit(MeshComponent_Vertex &mesh, SpotLightComponent &material)
    {
        uint8_t index=mesh.RenderIndex;
        auto&vec=Render_map[index];
        vec.push_back({&mesh,&material});
    }
    void Renderer3D::Submit(MeshComponent_Vertex &mesh, Material_Skybox &material)
    {
        //material.cube_map=Pbr_data::irradiance_map;
        uint8_t index=mesh.RenderIndex;
        auto&vec=Render_map[index];
        vec.push_back({&mesh,&material});


    }
    void Renderer3D::Submit(MeshComponent_ParticleVertex&mesh,Particles&material)
    {
        uint8_t index=mesh.RenderIndex;
        auto&vec=Render_map[index];
        vec.push_back({&mesh,&material});
    }
    void Renderer3D::EndScene()
    {
        auto m_pipeline=RenderPipeline::getInstance();

        for(auto &[index,vec]:Render_map)
        {
            m_pipeline->Submit(vec,index);//void Submit(){};
        }

        m_pipeline->OnPrepare();
        m_pipeline->EndPipeline();
        Render_map.clear();
    }


    struct Renderer3DData
    {
         Renderer3D::Statistics Stats;
    };
    static Renderer3DData *s_Data;

    void Renderer3D::Init()
    {

        s_Data=new Renderer3DData;


    }






    Renderer3D::Statistics Renderer3D::GetStats()
    {
        return s_Data->Stats;;
    }

    void Renderer3D::ResetStats()
    {
        memset(&s_Data->Stats, 0, sizeof(Statistics));

    }

    void Renderer3D::Shutdown()
    {
        dummy_vao= nullptr;
        Pbr_data::irradiance_map= nullptr;
        Pbr_data::prefiltered_map= nullptr;
        Pbr_data::BRDF_LUT= nullptr;

        if(!Debug)
        {

            delete s_Data;
        }
        else
        {
            delete s_Data;
        }



    }


    Ref<VertexArray> Renderer3D::dummy_vao= nullptr;
    void Renderer3D::DrawQuad()
    {
        if(dummy_vao== nullptr)
        {
            dummy_vao=VertexArray::Create();
        }
        // bufferless rendering in OpenGL:
        // https://trass3r.github.io/coding/2019/09/11/bufferless-rendering.html
        // https://stackoverflow.com/a/59739538/10677643
        dummy_vao->Bind();
        RenderCommand::DrawArrays(dummy_vao);



    }




    void Renderer3D::Utils::PreComputeIBL(Ref <CubeTexture> &envTexture)
    {

        auto pbr_data=envTexture->PreComputeIBL();

        Pbr_data::irradiance_map =  std::get<0>(pbr_data);
        Pbr_data::prefiltered_map=  std::get<1>(pbr_data);
        Pbr_data::BRDF_LUT       =  std::get<2>(pbr_data);

    }
}