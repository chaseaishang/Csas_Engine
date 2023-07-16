#include <CsasEngine.h>
#include <Csas_Engine/Core/EntryPoint.h>
#include "EditorLayer.h"

namespace CsasEngine {

    class CsasEditor : public Application
    {
    public:
        CsasEditor()
                : Application("Hazel Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~CsasEditor()
        {
        }
    };

    Application* CreateApplication()
    {
        return new CsasEditor();
    }

}

