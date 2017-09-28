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

	class Log final
	{
	public:
		Log(const LogLevel& logLevel, const std::string& logPath, bool printToConsole = false);
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		virtual ~Log(void);

		bool StartLogThread(void);

		void C(const std::string&);
		void E(const std::string&);
		void W(const std::string&);
		void N(const std::string&);
		void I(const std::string&);

	private:
		class impl;
		std::unique_ptr<impl> pImpl;
	};

}

#endif