//
// Created by chaseaishang on 23-6-14.
//

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
namespace CsasEngine
{
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
void Log::Init()
{
    spdlog::set_pattern("%^[%T]%$ %n: %v%s");
    s_CoreLogger = spdlog::stdout_color_mt("CsasEngine");
    s_CoreLogger->set_level(spdlog::level::trace);
    s_ClientLogger = spdlog::stdout_color_mt("App");
    s_ClientLogger->set_level(spdlog::level::trace);
}
}
//ENABLE_SPDLOG_STD(const unsigned char*)