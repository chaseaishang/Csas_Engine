//
// Created by chaseaishang on 23-6-18.
//

#pragma once


#include "RenderCommand.h"

#include "Shader.h"


namespace CsasEngine {
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);


        static void EndScene();

        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        static void setRenderType(bool Render2D=true){m_Renderer2D=Render2D;};
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static bool m_Renderer2D;
        static Scope<SceneData> s_SceneData;
    };






}
