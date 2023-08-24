//
// Created by chaseaishang on 23-7-15.
//我们希望在渲染中排序，目前我们仅仅查看mesh自身的顺序，并以此来建立批次并打包，在未来我们希望。。。
//We Now Just see the Index in meshComponent and build RenderBatch with it. In the future we hope that
//现在我们假定顺序一样的mesh其材质也应该一样
//So now we assume if two mesh that it's Index is same ,the material should be also
//
//
//
//
//
#pragma once
#include "Camera.h"
#include "Csas_Engine/Component/Mesh.h"
#include "Csas_Engine/Component/ParticleMesh.h"
#include "Csas_Engine/Component/Particle.h"
#include "UniformBuffer.h"
#include "Csas_Engine/Component/Material.h"
#include "Csas_Engine/Component/light.h"
namespace CsasEngine
{

    class Renderer3D {
    public:
        static void Init();

        static void Shutdown();

        static void BeginScene(Camera &camera,
                               std::vector<SpotLightComponent*>SpotlightsPtr,
                               std::vector<MeshComponent<Vertex> *> SpotMeshPtr,
                               std::vector<DirectionLightComponent*>DirectLight,
                               std::vector<MeshComponent<Vertex> *> DirectMeshPtr
                               );
        static void Submit(MeshComponent<Vertex>&mesh,        Material_BaseBRDF&material);
        static void Submit(MeshComponent<Vertex>&mesh,        Material_Skybox&material);
        static void Submit(MeshComponent<Vertex>&mesh,        SpotLightComponent&material);
        static void Submit(MeshComponent<ParticleVertex>&mesh,Particles&material);

        //Material_Skybox
        static void EndScene();
        static void DrawQuad();


        // Primitives

        // Stats
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t CubeCount = 0;
            uint32_t SphereCount = 0;
            uint32_t GetTotalVertexCount() const { return CubeCount * 8; }

            uint32_t GetTotalIndexCount() const { return CubeCount * 36; }
            float z=0;

        };

        static void ResetStats();

        static Statistics GetStats();
        class Utils
        {
        public:
            static void PreComputeIBL(Ref<CubeTexture>&envTexture);
        };
    private:
         static Ref<VertexArray> dummy_vao;


    };


}

