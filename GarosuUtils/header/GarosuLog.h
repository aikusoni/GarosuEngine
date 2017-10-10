#pragma once
#ifndef __GAROSU_LOG_H__
#define __GAROSU_LOG_H__

namespace Garosu
{

	enum class LogLevel
	{
		NONE,
		CRITICAL,
		ERROR,
		DEBUG,
		WARNING,
		NOTICE,
		INFO
	};

	class Log
	{
	public:
		static Log& Instance(void)
		{
			// this code is thread-safety on c++11
			static Log inst;
			return inst;
		}

		void SetLogLevel(const LogLevel&);
		void Start(void);
		void Stop(void);
		
		inline void C(const String& str) { Logging(LogLevel::CRITICAL, str); }
		inline void E(const String& str) { Logging(LogLevel::ERROR, str); }
		inline void D(const String& str) { Logging(LogLevel::DEBUG, str); }
		inline void W(const String& str) { Logging(LogLevel::WARNING, str); }
		inline void N(const String& str) { Logging(LogLevel::NOTICE, str); }
		inline void I(const String& str) { Logging(LogLevel::INFO, str); }

	private:
		Log(void);
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		~Log(void);

		void Logging(LogLevel, const String&);

	private:
		class LogThread;
		uptr<LogThread> pLT;
	};

}

#define LOGINSTANCE Garosu::Log::Instance()

#define SETLOGLEVEL(X) LOGINSTANCE.SetLogLevel(X)

#define SETLOGLEVELC() SETLOGLEVEL(LogLevel::CRITICAL)
#define SETLOGLEVELE() SETLOGLEVEL(LogLevel::ERROR)
#define SETLOGLEVELD() SETLOGLEVEL(LogLevel::DEBUG)
#define SETLOGLEVELW() SETLOGLEVEL(LogLevel::WARNING)
#define SETLOGLEVELN() SETLOGLEVEL(LogLevel::NOTICE)
#define SETLOGLEVELI() SETLOGLEVEL(LogLevel::INFO)

#define LOGSTART()	LOGINSTANCE.Start()
#define LOGSTOP()	LOGINSTANCE.Stop()

#define LOGC(X) LOGINSTANCE.C(X)
#define LOGE(X) LOGINSTANCE.E(X)
#define LOGD(X) LOGINSTANCE.D(X)
#define LOGW(X) LOGINSTANCE.W(X)
#define LOGN(X) LOGINSTANCE.N(X)
#define LOGI(X) LOGINSTANCE.I(X)

#endif