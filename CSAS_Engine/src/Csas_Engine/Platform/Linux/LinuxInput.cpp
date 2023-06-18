//
// Created by chaseaishang on 23-6-17.
//
#include "Csas_Engine/Csaspch.h"
#include "LinuxInput.h"
#include "Csas_Engine/Application.h"
#include "GLFW/glfw3.h"
namespace CsasEngine
{
Input*Input::s_Instance=new LinuxInput();


bool LinuxInput::IsKeyPressedImpl(int keycode)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool LinuxInput::IsMouseButtonPressedImpl(int button)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> LinuxInput::GetMousePosImpl()
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return { (float)xPos, (float)yPos };
}

float LinuxInput::GetMouseXImpl()
{
    auto [x, y] = GetMousePosImpl();
    return x;
}

float LinuxInput::GetMouseYImpl()
{
    auto [x, y] = GetMousePosImpl();
    return y;
}

}