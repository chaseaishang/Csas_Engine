#include "Csas_Engine/Csaspch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Csas_Engine/Platform/OpenGL/OpenGLShader.h"
#include "Renderer3D.h"

namespace CsasEngine {
    Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();
    bool Renderer::m_Renderer2D= false;
    void Renderer::Init()
    {
        CSAS_PROFILE_FUNCTION();

        RenderCommand::Init();
        if(m_Renderer2D)
        {
            Renderer2D::Init();
        }
        else
        {
            Renderer3D::Init();
        }

    }

    void Renderer::Shutdown()
    {
        if(m_Renderer2D)
        {
            Renderer2D::Shutdown();
        }
        else
        {
            Renderer3D::Shutdown();
        }

    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }


}