#pragma once


//#include"spdlog/sinks/stdout_color_sinks.h"
#include"Core.h"
#include"spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#define TURBO_CORE_ERR(...) TurboGE::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define TURBO_CORE_FTL(...) TurboGE::Logger::GetCoreLogger()->fatal(__VA_ARGS__)
#define TURBO_CORE_WRN(...) TurboGE::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define TURBO_CORE_TRC(...) TurboGE::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define TURBO_CORE_INF(...) TurboGE::Logger::GetCoreLogger()->info(__VA_ARGS__)

#define TURBO_CLIENT_ERR(...) TurboGE::Logger::GetClientLogger()->error(__VA_ARGS__)
#define TURBO_CLIENT_FTL(...) TurboGE::Logger::GetClientLogger()->fatal(__VA_ARGS__)
#define TURBO_CLIENT_WRN(...) TurboGE::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define TURBO_CLIENT_TRC(...) TurboGE::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define TURBO_CLIENT_INF(...) TurboGE::Logger::GetClientLogger()->info(__VA_ARGS__)

namespace TurboGE
{
	class TGE_API Logger
	{
		static std::shared_ptr<spdlog::logger> m_coreLogger;
		static std::shared_ptr<spdlog::logger> m_clientLogger;
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_clientLogger; }

	};
}

