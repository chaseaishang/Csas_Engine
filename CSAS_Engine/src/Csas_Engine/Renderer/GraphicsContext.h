//
// Created by chaseaishang on 23-6-18.
//
#pragma once

namespace CsasEngine {

    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual ~GraphicsContext() = default;

        static Scope<GraphicsContext> Create(void* window);
    };


}
