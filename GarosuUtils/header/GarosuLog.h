#pragma once
#ifndef __GAROSU_LOG_H__
#define __GAROSU_LOG_H__

namespace Garosu
{

	enum class LogLevel
	{
		CRITICAL,
		ERROR,
		WARNING,
		NOTICE,
		INFO
	};

	class Log
	{
	public:
		Log(const std::string& logPath, const LogLevel& logLevel = LogLevel::ERROR, bool printToConsole = false);
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		virtual ~Log(void);

		bool StartLogThread(void);

		inline void C(const std::string&);
		inline void E(const std::string&);
		inline void W(const std::string&);
		inline void N(const std::string&);
		inline void I(const std::string&);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif