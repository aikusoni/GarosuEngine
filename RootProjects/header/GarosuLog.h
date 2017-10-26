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

		void Logging(LogLevel, const String&);

		// Stream Style Log
		template <Garosu::LogLevel VLogLevel>
		inline auto S(void) { return LogStream<VLogLevel>(*this); }

		// Function Style Log (maybe quick)
		template <Garosu::LogLevel VLogLevel>
		inline void Q(const String& str) {
			Logging(VLogLevel, str);
		}

		// Formatting Style Log
		template <Garosu::LogLevel VLogLevel, typename ... Args>
		inline void F(const String& format, Args&& ... args) {
			StringStream ss;
			FMT(ss, format.c_str(), args...);
			Logging(VLogLevel, ss.str());
		}

	private:
		Log(void);
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		~Log(void);

	private:
		// template for stream style log
		template <Garosu::LogLevel VLogLevel>
		class LogStream
		{
		private:
			std::stringstream ss;
			Log& mLogger;

		public:
			LogStream(Log& logger) : mLogger(logger) { }
			LogStream(LogStream& logStream) : mLogger(logStream.mLogger) { }
			LogStream& operator=(LogStream& logStream) { mLogger = logstream.mLogger; }

			virtual ~LogStream(void) {
				mLogger.Logging(VLogLevel, ss.str());
			}

			template<typename T>
			LogStream& operator<<(const T& toLog)
			{
				ss << toLog;
				return *this;
			}
		};

		// template for formatting style log
		inline void FMT(StringStream& ss, const char* format) { ss << format; }

		template <typename T, typename ... Args>
		inline void FMT(StringStream& ss, const char* format, T&& value, Args&& ... args) {
			for (; *format != '\0'; ++format) {
				if (*format == '%') {
					if (*(format + 1) == '%') ++format; // "%%" -> '%'						
					else
					{
						ss << value;
						FMT(ss, format + 1, args...); // recursive call
						return;
					}
				}
				ss << *format;
			}
		}

	private:
		class impl;
		uptr<impl> pImpl;
	};

}

#define LOGINSTANCE Garosu::Log::Instance()

#define SETLOGLEVEL(X) LOGINSTANCE.SetLogLevel(X)

#define SETLOGLEVEL_CRITICAL() SETLOGLEVEL(Garosu::LogLevel::CRITICAL)
#define SETLOGLEVEL_ERROR() SETLOGLEVEL(Garosu::LogLevel::ERROR)
#define SETLOGLEVEL_DEBUG() SETLOGLEVEL(Garosu::LogLevel::DEBUG)
#define SETLOGLEVEL_WARNING() SETLOGLEVEL(Garosu::LogLevel::WARNING)
#define SETLOGLEVEL_NOTICE() SETLOGLEVEL(Garosu::LogLevel::NOTICE)
#define SETLOGLEVEL_INFO() SETLOGLEVEL(Garosu::LogLevel::INFO)

#define LOGSTART()	LOGINSTANCE.Start()
#define LOGSTOP()	LOGINSTANCE.Stop()

// stream style log macro
// usage : LOGC << "blah blah " << 123 << ", " << 3.14f << 'c';
#define LOGC LOGINSTANCE.S<Garosu::LogLevel::CRITICAL>()
#define LOGE LOGINSTANCE.S<Garosu::LogLevel::ERROR>()
#define LOGD LOGINSTANCE.S<Garosu::LogLevel::DEBUG>()
#define LOGW LOGINSTANCE.S<Garosu::LogLevel::WARNING>()
#define LOGN LOGINSTANCE.S<Garosu::LogLevel::NOTICE>()
#define LOGI LOGINSTANCE.S<Garosu::LogLevel::INFO>()

// function style log macro (it may quick than the others)
// usage : LOGFC("function style log");
#define LOGQC(X) LOGINSTANCE.Q<Garosu::LogLevel::CRITICAL>(X)
#define LOGQE(X) LOGINSTANCE.Q<Garosu::LogLevel::ERROR>(X)
#define LOGQD(X) LOGINSTANCE.Q<Garosu::LogLevel::DEBUG>(X)
#define LOGQW(X) LOGINSTANCE.Q<Garosu::LogLevel::WARNING>(X)
#define LOGQN(X) LOGINSTANCE.Q<Garosu::LogLevel::NOTICE>(X)
#define LOGQI(X) LOGINSTANCE.Q<Garosu::LogLevel::INFO>(X)

// formatting style log macro
// usage : LOGFC("i-value : %, f-value : %", 123, 3.14f);
#define LOGFC(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::CRITICAL>(FORMAT, __VA_ARGS__)
#define LOGFE(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::ERROR>(FORMAT, __VA_ARGS__)
#define LOGFD(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::DEBUG>(FORMAT, __VA_ARGS__)
#define LOGFW(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::WARNING>(FORMAT, __VA_ARGS__)
#define LOGFN(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::NOTICE>(FORMAT, __VA_ARGS__)
#define LOGFI(FORMAT, ...) LOGINSTANCE.F<Garosu::LogLevel::INFO>(FORMAT, __VA_ARGS__)

#endif