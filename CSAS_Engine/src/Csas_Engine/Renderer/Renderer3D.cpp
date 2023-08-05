//
// Created by chaseaishang on 23-7-15.
//
#include "Csas_Engine/Csaspch.h"
#include "Renderer3D.h"
#include "Shader.h"
#include "VertexArray.h"
#include "./RenderPipeline/RenderPipeline.h"
#include "RenderPipeline/RenderData/RenderData.h"
//Temp TODO remove
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "glm/gtx/string_cast.hpp"
namespace CsasEngine {
    static bool Debug= false;

    static std::unordered_map<RenderIndex,RenderDataVec>Render_map;

    void Renderer3D::BeginScene(Camera &camera, std::vector<SpotLightComponent *> SpotlightsPtr)
    {
        auto m_pipeline=RenderPipeline::getInstance();
        m_pipeline->BeginPipeline(&camera,SpotlightsPtr);
    }
    void Renderer3D::Submit(MeshComponent&mesh,Material_BaseBRDF&material)
    {
        uint8_t index=mesh.RenderIndex;
        auto&vec=Render_map[index];
        vec.push_back({&mesh,&material});
        //      index->vector
        //
        //
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
        if(!Debug)
        {

            delete s_Data;
        }
        else
        {
            delete s_Data;
        }



    }





    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera, Material_BasePrimitive &material)
    {

    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera,
                              Material_BasePBR &material,
                              std::vector<SpotLightComponent>&Spotlights,
                              std::vector<DirectionLightComponent>&Direction_lights)
    {

    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera, Material_Cartoon &material,
                              std::vector<SpotLightComponent> &Spotlights)
    {

    }

    void Renderer3D::DrawMesh(MeshComponent &mesh, const Camera &camera, Material_BaseBRDF &material,
                              std::vector<SpotLightComponent>&Spotlights)
    {

    }




}