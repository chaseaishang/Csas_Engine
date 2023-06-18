//
// Created by chaseaishang on 23-6-14.
//

#pragma once
#include "Csas_Engine/Application.h"
#ifdef CSAS_PLATFORM_LINUX

extern CsasEngine::Application* CsasEngine::CreateApplication();

int main(int argc, char** argv)
{
    CsasEngine::Log::Init();
    CSAS_CORE_WARN("Initialized Log");
    int a = 5;
    CSAS_INFO("hello var = {0}", a);
	auto app = CsasEngine::CreateApplication();
	app->Run();
	delete app;
}

#endif // CSAS_PLATFORM_LINUX
