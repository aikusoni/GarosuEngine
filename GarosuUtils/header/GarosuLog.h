#pragma once
#ifndef __GAROSU_LOG_H__
#define __GAROSU_LOG_H__

namespace Garosu
{

	enum class LogLevel
	{
		CRITICAL,
		ERROR,
		DEBUG,
		WARNING,
		NOTICE,
		INFO
	};

	class Logger
	{
	public:
		Logger(const String& logPath, const LogLevel& logLevel = LogLevel::ERROR, bool printToConsole = false);
		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		virtual ~Logger(void);

		bool StartLogThread(void);

		inline void C(const String&);
		inline void E(const String&);
		inline void D(const String&);
		inline void W(const String&);
		inline void N(const String&);
		inline void I(const String&);

	private:
		class impl;
		uptr<impl> pImpl;
	};

	class Log : public Logger
	{
	public:
		static Log& instance(void)
		{
			// this code will be thread-safety on c++11
			static Log inst;
			return inst;
		}

	private:
		Log(void) : Logger("garosu.log") {}
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		virtual ~Log(void) {}
	};

#define LOGINSTANCE Log::instance()
#define LOGC(X) LOGINSTANCE.C(X)
#define LOGE(X) LOGINSTANCE.E(X)
#define LOGD(X) LOGINSTANCE.D(X)
#define LOGW(X) LOGINSTANCE.W(X)
#define LOGN(X) LOGINSTANCE.N(X)
#define LOGI(X) LOGINSTANCE.I(X)

}

#endif