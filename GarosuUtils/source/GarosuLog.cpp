#include <GarosuTypedef.h>

#include "GarosuLog.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <concurrent_priority_queue.h>

#include <chrono>

#include "GarosuThread.h"
#include "GarosuStringUtils.h"

namespace Garosu
{

	struct LogData
	{
		std::chrono::system_clock::time_point logTime;
		LogLevel logLevel;
		String logString;

		struct Comparator
		{
			bool operator()(const LogData* a, const LogData* b) const {
				return a->logTime > b->logTime;
			}
		};
	};

	class Logger::impl final : public BaseWorker
	{
	public:
		impl(void)
			: mThread(this)
		{

		}

		~impl(void)
		{
			bLoop = false; // to stop thread loop
			mThread.Join();
			mFileStream.close();
		}

		LogLevel mLogLevel;
		String mLogPath;
		std::fstream mFileStream;
		bool mPrintToConsole = false;

		bool Push(const LogLevel&, const String&);
		LogData* Pop(void);

		void StartLogging(void);

		virtual void DoWork(void);

	private:
		Concurrency::concurrent_priority_queue <LogData*, LogData::Comparator> logQueue;

		std::atomic<bool> bLoop = true;
		BaseThread mThread;
	};

	void Logger::C(const String& t)
	{
		pImpl->Push(LogLevel::CRITICAL, t);
	}

	void Logger::E(const String& t)
	{
		pImpl->Push(LogLevel::ERROR, t);
	}

	void Logger::D(const String& t)
	{
		pImpl->Push(LogLevel::DEBUG, t);
	}

	void Logger::W(const String& t)
	{
		pImpl->Push(LogLevel::WARNING, t);
	}

	void Logger::N(const String& t)
	{
		pImpl->Push(LogLevel::NOTICE, t);
	}

	void Logger::I(const String& t)
	{
		pImpl->Push(LogLevel::INFO, t);
	}

	Logger::Logger(const String& logPath, const LogLevel& logLevel, bool printToConsole)
		: pImpl(mk_uptr<impl>())
	{
		pImpl->mLogLevel = logLevel;
		pImpl->mLogPath = logPath;
		pImpl->mPrintToConsole = printToConsole;
	}

	Logger::~Logger(void)
	{

	}

	bool Logger::StartLogThread(void)
	{
		try {
			pImpl->mFileStream.open(pImpl->mLogPath, std::fstream::out);
		}
		catch (std::ostream::failure&)
		{
			return false;
		}

		pImpl->StartLogging();

		return true;
	}

	bool Logger::impl::Push(const LogLevel& logLevel, const String& t)
	{
		if (logLevel > mLogLevel)
			return false;

		LogData* logData = new LogData();
		logData->logLevel = logLevel;
		logData->logTime = std::chrono::system_clock::now();
		logData->logString = t;

		logQueue.push(logData);

		return true;
	}

	LogData* Logger::impl::Pop(void)
	{
		LogData* logData;
		if (logQueue.try_pop(logData))
			return logData;

		else
			return NULL;
	}

	void Logger::impl::StartLogging(void)
	{
		mThread.Start();
	}

	std::ostream& operator<<(std::ostream& os, LogData& logData)
	{
		os << "(" << logData.logTime << ")[";
		switch (logData.logLevel)
		{
		case LogLevel::CRITICAL:
			os << "CRITICAL";
			break;
		case LogLevel::ERROR:
			os << "ERROR";
			break;
		case LogLevel::DEBUG:
			os << "DEBUG";
			break;
		case LogLevel::WARNING:
			os << "WARNING";
			break;
		case LogLevel::NOTICE:
			os << "NOTICE";
			break;
		case LogLevel::INFO:
			os << "INFO";
			break;
		}
		os << "] " << logData.logString;

		return os;
	}

	void Logger::impl::DoWork(void)
	{
		while (bLoop || logQueue.size() > 0)
		{
			LogData* logData = Pop();
			if (logData == NULL)
			{
				ThreadUtils::SleepFor(10 * 1000 * 1000);
				continue;
			}

			try {
				mFileStream << *logData << std::endl;
				if (mPrintToConsole) std::cout << *logData << std::endl;
			}
			catch (std::ifstream::failure&)
			{
				std::cerr << "log failed" << std::endl;
			}
		}
	}

}