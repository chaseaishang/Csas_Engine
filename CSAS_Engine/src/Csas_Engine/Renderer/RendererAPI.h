//
// Created by chaseaishang on 23-6-19.
//

#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"



namespace CsasEngine {

    class RendererAPI
    {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };
    public:
        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;
        virtual void DepthMask(bool Enable)=0;
        virtual void DepthTest(bool Enable)=0;
        virtual void FaceCulling(bool Enable,bool back=true)=0;

        virtual void DrawArrays(const Ref<VertexArray>& vertexArray)=0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
        virtual ~RendererAPI() = default;
        inline static API GetAPI() { return s_API; }
        static Scope<RendererAPI> Create();

    private:
        static API s_API;
    };




}

