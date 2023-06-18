//
// Created by chaseaishang on 23-6-18.
//

#pragma once

namespace CsasEngine {

enum class RendererAPI
{
    None = 0, OpenGL = 1
};

class Renderer
{
public:
    inline static RendererAPI GetAPI() { return s_RendererAPI; }
private:
    static RendererAPI s_RendererAPI;
};


}
