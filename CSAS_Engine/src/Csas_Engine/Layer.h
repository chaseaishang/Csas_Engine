#pragma once
#include "Csas_Engine/Csaspch.h"
#include "Core.h"
#include "Csas_Engine/Events/include/Event.h"

namespace CsasEngine {

class  Layer
{
public:
    Layer(const std::string& name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate() {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event& event) {}

    inline const std::string& GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};
}