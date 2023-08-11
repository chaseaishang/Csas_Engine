//
// Created by chaseaishang on 23-6-19.
//

#pragma once

#include "Csas_Engine/Renderer/RendererAPI.h"

namespace CsasEngine {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        virtual void Init() override;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void FaceCulling(bool Enable,bool back=true)override;
        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
        void DrawArrays(const Ref<VertexArray>& vertexArray)override;
        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount=0)override;
        void DepthMask(bool Enable)override;

    };




}

