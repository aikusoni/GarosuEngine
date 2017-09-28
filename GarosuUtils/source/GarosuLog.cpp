#include <memory>
#include <string>

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
		std::string logString;

		struct Comparator
		{
			bool operator()(const LogData* a, const LogData* b) const {
				return a->logTime > b->logTime;
			}
		};
	};

	class Log::impl final : public BaseWorker
	{
	public:
		impl(void)
			: mThread(*this)
		{

		}

		~impl(void)
		{
			bLoop = false;
			mThread.Join();
			mFileStream.close();
		}

		LogLevel mLogLevel;
		std::string mLogPath;
		std::fstream mFileStream;
		bool mPrintToConsole = false;

		bool Push(const LogLevel&, const std::string&);
		LogData* Pop(void);

		void StartLogging(void);

		virtual void DoWork(void);

	private:
		Concurrency::concurrent_priority_queue <LogData*, LogData::Comparator> logQueue;

		bool bLoop = true;
		BaseThread mThread;
	};

	Log::Log(const std::string& logPath, const LogLevel& logLevel, bool printToConsole)
		: pImpl(std::make_unique<impl>())
	{
		pImpl->mLogLevel = logLevel;
		pImpl->mLogPath = logPath;
		pImpl->mPrintToConsole = printToConsole;
	}

	Log::~Log(void)
	{

	}

	bool Log::StartLogThread(void)
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

	void Log::C(const std::string& t)
	{
		pImpl->Push(LogLevel::CRITICAL, t);
	}

	void Log::E(const std::string& t)
	{
		pImpl->Push(LogLevel::ERROR, t);
	}

	void Log::W(const std::string& t)
	{
		pImpl->Push(LogLevel::WARNING, t);
	}

	void Log::N(const std::string& t)
	{
		pImpl->Push(LogLevel::NOTICE, t);
	}

	void Log::I(const std::string& t)
	{
		pImpl->Push(LogLevel::INFO, t);
	}

	bool Log::impl::Push(const LogLevel& logLevel, const std::string& t)
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

	LogData* Log::impl::Pop(void)
	{
		LogData* logData;
		if (logQueue.try_pop(logData))
			return logData;

		else
			return NULL;
	}

	void Log::impl::StartLogging(void)
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

	void Log::impl::DoWork(void)
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