#include"tgepch.h"
#include "Logger.h"

namespace TurboGE
{
	std::shared_ptr<spdlog::logger> Logger::m_coreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_clientLogger;

	void TurboGE::Logger::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_coreLogger = spdlog::stdout_color_mt("TURBOGE");
		m_coreLogger->set_level(spdlog::level::trace);
		m_clientLogger = spdlog::stdout_color_mt("APP");
		m_clientLogger->set_level(spdlog::level::trace);
	}
}