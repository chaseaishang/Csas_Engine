//
// Created by chaseaishang on 23-6-18.
//
#include "Csas_Engine/Csaspch.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "OpenGLContext.h"






namespace CsasEngine {

    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
            : m_WindowHandle(windowHandle)
    {
        CSAS_CORE_ASSERT(windowHandle, "Window handle is null!")
    }

    void OpenGLContext::Init()
    {
        CSAS_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CSAS_CORE_ASSERT(status, "Failed to initialize Glad!");

        CSAS_CORE_INFO("OpenGL Info:");
        CSAS_CORE_INFO("  Vendor: {0}", (char*)glGetString(GL_VENDOR));
        CSAS_CORE_INFO("  Renderer: {0}", (char*)glGetString(GL_RENDERER));
        CSAS_CORE_INFO("  Version: {0}", (char*)glGetString(GL_VERSION));

#ifdef CSAS_ENABLE_ASSERTS
        int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		CSAS_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "Hazel requires at least OpenGL version 4.5!");
#endif
    }

    void OpenGLContext::SwapBuffers()
    {
        CSAS_PROFILE_FUNCTION();

        glfwSwapBuffers(m_WindowHandle);
    }

}