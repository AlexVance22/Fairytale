#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"


namespace ftale
{

	class FTALE_API Log
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

#define FTALE_CORE_TRACE(...)	ftale::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FTALE_CORE_INFO(...)	ftale::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FTALE_CORE_WARN(...)	ftale::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FTALE_CORE_ERROR(...)	ftale::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FTALE_CORE_FATAL(...)	ftale::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define FTALE_TRACE(...)		ftale::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FTALE_INFO(...)			ftale::Log::GetClientLogger()->info(__VA_ARGS__)
#define FTALE_WARN(...)			ftale::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FTALE_ERROR(...)		ftale::Log::GetClientLogger()->error(__VA_ARGS__)
#define FTALE_FATAL(...)		ftale::Log::GetClientLogger()->fatal(__VA_ARGS__)