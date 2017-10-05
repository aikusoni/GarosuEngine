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
			static Log inst(LogLevel::DEBUG);
			return inst;
		}

		inline void C(const String&);
		inline void E(const String&);
		inline void D(const String&);
		inline void W(const String&);
		inline void N(const String&);
		inline void I(const String&);

	private:
		Log(LogLevel logLevel);
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		~Log(void);

	private:
		class LogThread;
		uptr<LogThread> pLT;
	};

}

#define LOGINSTANCE Garosu::Log::Instance()

#define LOGC(X) LOGINSTANCE.C(X)
#define LOGE(X) LOGINSTANCE.E(X)
#define LOGD(X) LOGINSTANCE.D(X)
#define LOGW(X) LOGINSTANCE.W(X)
#define LOGN(X) LOGINSTANCE.N(X)
#define LOGI(X) LOGINSTANCE.I(X)

#endif