//
// Created by chaseaishang on 23-6-14.
//

#pragma once
#include "Csas_Engine/Core/Application.h"
#ifdef CSAS_PLATFORM_LINUX

extern CsasEngine::Application* CsasEngine::CreateApplication();

int main(int argc, char** argv)
{


    CsasEngine::Log::Init();

    CSAS_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
    auto app = CsasEngine::CreateApplication();
    CSAS_PROFILE_END_SESSION();

    CSAS_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
    app->Run();
    CSAS_PROFILE_END_SESSION();

    CSAS_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Shutdown.json");
    delete app;

    CSAS_PROFILE_END_SESSION();

}

#endif // CSAS_PLATFORM_LINUX
