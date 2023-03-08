#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace TurboGE {
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	class Instrumentor
	{
	private:
		std::ofstream m_OutputStream;
		int m_ProfileCount;
	public:
		Instrumentor()
			:  m_ProfileCount(0)
		{
		}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json")
		{
			m_OutputStream.open(filepath);
			WriteHeader();
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& result)
		{
			if (m_ProfileCount++ > 0)
				m_OutputStream << ",";

			std::string name = result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << result.ThreadID << ",";
			m_OutputStream << "\"ts\":" << result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });


			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped;
	};
}

#define TGE_PROFILE 1
#ifdef TGE_PROFILE

#define TGE_BEGIN_PROFILING(name, filepath) TurboGE::Instrumentor::Get().BeginSession(name, filepath)
#define TGE_END_PROFILING() TurboGE::Instrumentor::Get().EndSession()
#define TGE_PROFILE_SCOPE(name) TurboGE::InstrumentationTimer timer##__LINE__(name);
#define TGE_PROFILE_FUNCTION() TGE_PROFILE_SCOPE(__FUNCSIG__)
#elif
#define TGE_BEGIN_PROFILING(name, filepath)
#define TGE_END_PROFILING()
#define TGE_PROFILE_SCOPE(name)
#define TGE_PROFILE_FUNCTION()
#endif