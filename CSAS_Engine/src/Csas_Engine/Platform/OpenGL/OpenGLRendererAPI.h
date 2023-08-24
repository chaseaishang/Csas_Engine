//
// Created by chaseaishang on 23-6-19.
//

#pragma once

#include "Csas_Engine/Renderer/RendererAPI.h"

namespace CsasEngine {

    class OpenGLRendererAPI : public RendererAPI
    {
    public:
        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void FaceCulling(bool Enable,bool back=true)override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;
        void DrawArrays(const Ref<VertexArray>& vertexArray)override;
        void DrawPoints(const Ref<VertexArray>& vertexArray,uint32_t count)override;
        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount=0)override;
        void SetPointSize(float size)override;
        void DepthMask(bool Enable)override;
        void DepthTest(bool Enable)override;
        void SetBlend(bool Enable,BlendFun fun=BlendFun::ONE_MINUS_SRC_ALPHA)override;

    };




}

