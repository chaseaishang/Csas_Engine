#include <CsasEngine.h>
#include <Csas_Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public CsasEngine::Application
{
public:
    Sandbox()
    {
        // PushLayer(new ExampleLayer());
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {
    }
};

CsasEngine::Application* CsasEngine::CreateApplication()
{
    return new Sandbox();
}
