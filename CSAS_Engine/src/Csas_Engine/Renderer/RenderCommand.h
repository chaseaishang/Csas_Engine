//
// Created by chaseaishang on 23-6-19.
//

#pragma once

#include "RendererAPI.h"

namespace CsasEngine {

    class RenderCommand
    {
    public:
        inline static void Init()
        {
            s_RendererAPI->Init();
        }
        inline static void FaceCulling(bool Enable,bool back=true)
        {
            s_RendererAPI->FaceCulling(Enable,back);
        }
        inline static void DepthTest(bool Enable)
        {
            s_RendererAPI->DepthMask(Enable);
        }
        inline static void DepthMask(bool Enable)
        {
            s_RendererAPI->DepthMask(Enable);
        }
        inline static void SetBlend(bool Enable,BlendFun fun=BlendFun::ONE_MINUS_SRC_ALPHA)
        {
            s_RendererAPI->SetBlend(Enable,fun);
        }
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            s_RendererAPI->SetViewport(x, y, width, height);
        }

        inline static void SetClearColor(const glm::vec4& color)
        {
            s_RendererAPI->SetClearColor(color);
        }

        inline static void Clear()
        {
            s_RendererAPI->Clear();
        }
        inline static void DrawArrays(const Ref<VertexArray>& vertexArray)
        {
            s_RendererAPI->DrawArrays(vertexArray);
        }
        inline static void DrawPoints(const Ref<VertexArray>& vertexArray,uint32_t count=0)
        {
            s_RendererAPI->DrawPoints(vertexArray,count);
        }
        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0)
        {
            s_RendererAPI->DrawIndexed(vertexArray,count);
        }
        inline static void SetPointSize(float size=1.0f)
        {
            s_RendererAPI->SetPointSize(size);
        }
    private:
        static Scope<RendererAPI> s_RendererAPI;
    };

}

