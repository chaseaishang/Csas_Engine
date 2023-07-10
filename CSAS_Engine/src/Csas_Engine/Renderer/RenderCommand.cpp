//
// Created by chaseaishang on 23-6-19.
//

#include "Csas_Engine/Csaspch.h"
#include "RenderCommand.h"

#include "Csas_Engine/Platform/OpenGL/OpenGLRendererAPI.h"

namespace CsasEngine {

    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}

