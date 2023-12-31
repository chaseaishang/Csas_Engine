//
// Created by chaseaishang on 23-6-19.
//

#include "Csas_Engine/Csaspch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace CsasEngine {

	void OpenGLMessageCallback(
        unsigned source,
        unsigned type,
        unsigned id,
        unsigned severity,
        int length,
        const char* message,
        const void* userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         CSAS_CORE_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:       CSAS_CORE_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW:          CSAS_CORE_WARN(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION: CSAS_CORE_TRACE(message); return;
    }

    CSAS_CORE_ASSERT(false, "Unknown severity level!");
}

    void OpenGLRendererAPI::Init()
    {
        CSAS_PROFILE_FUNCTION();

#ifdef CSAS_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        glViewport(x, y, width, height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount)
    {


        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);

    }
    void OpenGLRendererAPI::DepthTest(bool Enable)
    {


        if (Enable )
        {
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LEQUAL);
            glDepthRange(0.0f, 1.0f);

        }
        else
        {
            glDisable(GL_DEPTH_TEST);

        }

    }
    void OpenGLRendererAPI::DepthMask(bool Enable)
    {
        glDepthMask(Enable);

    }
   //just for debug
    void OpenGLRendererAPI::DrawArrays(const Ref <VertexArray> &vertexArray)
    {
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void OpenGLRendererAPI::FaceCulling(bool Enable,bool back)
    {
        if(Enable)
        {
            glEnable(GL_CULL_FACE);

            glFrontFace(GL_CCW);
            if(back)
            {
                glCullFace(GL_BACK);
            }
            else
            {
                glCullFace(GL_FRONT);
            }


        }
        else
        {
            glDisable(GL_CULL_FACE);
        }

    }

    void OpenGLRendererAPI::SetPointSize(float size)
    {
        glPointSize(size);
    }

    void OpenGLRendererAPI::DrawPoints(const Ref <VertexArray> &vertexArray, uint32_t count)
    {
        glDrawArrays(GL_POINTS, 0, count);
    }

    void OpenGLRendererAPI::SetBlend(bool Enable, BlendFun fun)
    {
        if(Enable)
        {
            glEnable(GL_BLEND);
            uint dfactor;
            switch (fun)
            {
                case BlendFun::ONE_MINUS_SRC_ALPHA:dfactor=GL_ONE_MINUS_SRC_ALPHA;break;

            }
            glBlendFunc(GL_SRC_ALPHA, dfactor);

        } else
        {
            glDisable(GL_BLEND);
        }


    }


}

