//
// Created by chaseaishang on 23-7-15.
//

#pragma once

#include "Camera.h"

namespace CsasEngine {
    class Renderer3D {

    public:
        static void Init();

        static void Shutdown();
        static void Flush();
        static void BeginScene(const Camera &camera, const glm::mat4 &transform);

        static void EndScene();

        static void
        DrawCube(const glm::mat4 &transform,const glm::vec4 &tintColor = glm::vec4(1.0f));
        // Primitives

        // Stats
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertexCount() const { return QuadCount * 4; }

            uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
        };

        static void ResetStats();

        static Statistics GetStats();
    };


}

