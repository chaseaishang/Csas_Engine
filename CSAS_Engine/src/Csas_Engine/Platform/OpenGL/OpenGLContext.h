//
// Created by chaseaishang on 23-6-18.
//

#include "Csas_Engine/Renderer/GraphicsContext.h"
struct GLFWwindow;

namespace CsasEngine {

    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle;
    };

}

