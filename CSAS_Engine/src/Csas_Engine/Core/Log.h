//
// Created by chaseaishang on 23-6-14.
//

#pragma once
#include "Core.h"





#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <fmt/ostream.h>
namespace CsasEngine {


class  Log
{
private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
};
}



// core log macros
#define CSAS_CORE_CRITICAL(...) ::CsasEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CSAS_CORE_ERROR(...)    ::CsasEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CSAS_CORE_WARN(...)     ::CsasEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CSAS_CORE_INFO(...)     ::CsasEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CSAS_CORE_TRACE(...)    ::CsasEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)

// client log macros
#define CSAS_CRITICAL(...) ::CsasEngine::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CSAS_ERROR(...)    ::CsasEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CSAS_WARN(...)     ::CsasEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CSAS_INFO(...)     ::CsasEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define CSAS_TRACE(...)    ::CsasEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
//#define ENABLE_SPDLOG_STD(type) template <> struct fmt::formatter<type> : ostream_formatter{};

