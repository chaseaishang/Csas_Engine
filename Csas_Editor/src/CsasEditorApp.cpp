#include <CsasEngine.h>
#include <Csas_Engine/Core/EntryPoint.h>
#include "EditorLayer.h"
#include "Example/Example.h"
namespace CsasEngine {

    class CsasEditor : public Application
    {
    public:
        CsasEditor()
                : Application("Hazel Editor")
        {
            //register Examples
            ExampleMenu::RegisterAllExample();
            PushLayer(new EditorLayer());
        }

        ~CsasEditor()
        {
            ExampleMenu::UnRegisterAllExample();
        }
    };

    Application* CreateApplication()
    {
        return new CsasEditor();
    }

}

