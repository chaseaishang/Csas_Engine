//
// Created by chaseaishang on 23-7-15.
//

#pragma once

#include "Camera.h"
#include "Csas_Engine/Component/Mesh.h"
#include "UniformBuffer.h"
#include "Csas_Engine/Component/Material.h"
#include "Csas_Engine/Component/light.h"
namespace CsasEngine {

    class Renderer3D {

    public:
        static void Init();

        static void Shutdown();
        static void Flush();
        static void BeginScene(const Camera &camera, const glm::mat4 &transform);

        static void EndScene();



        static void DrawMesh(MeshComponent&mesh,const Camera &camera,Material_BasePrimitive&material);
        static void DrawMesh(MeshComponent&mesh,const Camera &camera,Material_BasePBR&material,
                             std::vector<SpotLightComponent>&Spotlights,
                             std::vector<DirectionLightComponent>&Direction_lights
                             );
        static void DrawMesh(MeshComponent&mesh,const Camera &camera,Material_Cartoon&material,
                             std::vector<SpotLightComponent>&Spotlights
        );
        static void DrawMesh(MeshComponent&mesh,const Camera &camera,Material_BaseBRDF&material);
        static void DrawTestUniforBLock();
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
    private:

    };


}

