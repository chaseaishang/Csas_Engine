//
// Created by chaseaishang on 23-6-18.
//

#pragma once

#include <string>

namespace CsasEngine {

    class Shader
    {
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;
    private:
        uint32_t m_RendererID;
    };

}
