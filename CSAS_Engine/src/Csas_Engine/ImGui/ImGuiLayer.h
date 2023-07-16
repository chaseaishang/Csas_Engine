//
// Created by chaseaishang on 23-6-17.
//
#pragma once

#include "Csas_Engine/Core/Layer.h"

#include <Csas_Engine/Events/include/MouseEvent.h>
#include <Csas_Engine/Events/include/KeyEvent.h>
#include <Csas_Engine/Events/include/ApplicationEvent.h>

namespace CsasEngine {

    /*
     * @todo fix ImGui窗口出现重影, 目测窗口或帧之类的没清理干净的，暂时放在这儿，后面再看看
     * @todo ImGui窗体无法关系，关闭事件未接入？
     */
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnEvent(Event& e) override;
        void Begin();
        void End();
        void BlockEvents(bool block) { m_BlockEvents = block; }

    private:
        bool m_BlockEvents = true;
        float m_Time = 0.0f;
    };

}
