#include <GarosuTypedef.h>
#include <GarosuSettings.h>

#include "GarosuLog.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <concurrent_priority_queue.h>

#include <chrono>

#include "GarosuThread.h"
#include "GarosuStringUtils.h"
#include "GarosuLockUtils.h"

namespace Garosu
{

	inline std::ostream& operator<<(std::ostream& os, LogLevel logLevel)
	{
		switch (logLevel)
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

		return os;
	}

	class LogData
	{
	public:
		std::chrono::system_clock::time_point logTime;
		LogLevel logLevel;
		String logString;

		struct Comparator
		{
			bool operator()(shptr<LogData> a, shptr<LogData> b) const {
				return a->logTime > b->logTime;
			}
		};

		inline friend std::ostream& operator<<(std::ostream& os, LogData& logData)
		{
			os << "(" << logData.logTime << ")[" << logData.logLevel << "] " << logData.logString;
			return os;
		}
	};

	class LogQueue
	{
	public:
		LogQueue(void);
		LogQueue(const LogQueue&) = delete;
		LogQueue& operator=(const LogQueue&) = delete;

		~LogQueue(void);

		inline void Push(shptr<LogData> logData);
		inline shptr<LogData> Pop(void);

	private:
		Concurrency::concurrent_priority_queue<shptr<LogData>, LogData::Comparator> mQueue;
	};

	LogQueue::LogQueue(void)
	{

	}

	LogQueue::~LogQueue(void)
	{

	}

	void LogQueue::Push(shptr<LogData> logData)
	{
		mQueue.push(logData);
	}

	shptr<LogData> LogQueue::Pop(void)
	{
		shptr<LogData> logData;
		if (mQueue.try_pop(logData))
			return logData;
		else
			return nullptr;
	}

	class LogWorker : public BaseWorker
	{
	public:
		LogWorker(LogQueue& logQueue) : doLog(false), isLogging(false), mLogQueue(logQueue) {}
		LogWorker(const LogWorker&) = delete;
		LogWorker& operator=(const LogWorker&) = delete;

		~LogWorker(void) {}

		virtual void DoWork(void)
		{
			locker.Lock();
			if (isLogging)
			{
				locker.Unlock();
				return;
			}
			isLogging = true;
			locker.Unlock();

			std::fstream logFile;
			logFile.open(Settings::GetLogPath().c_str(), std::fstream::out);

			if (!logFile.is_open()) return;

			// log output loop
			u32 tryCnt = 0u;
			doLog = true;
			while (doLog)
			{
				auto logData = mLogQueue.Pop();
				if (logData == NULL) {
					tryCnt++;
					if (tryCnt > 1000)
					{
						tryCnt = 0;
						ThreadUtils::SleepFor(10 * 1000 * 1000); // 10 ms
					}
					continue;
				}

				logFile << logData << std::endl; // TODO
			}

			if (logFile.is_open()) logFile.close();

			locker.Lock();
			isLogging = false;
			locker.Unlock();
		}

		Locker locker;
		bool isLogging;

		std::atomic<bool> doLog;
		LogQueue& mLogQueue;
	};	

	class Log::LogThread : public BaseThread
	{
	public:
		LogThread(void);
		LogThread(const LogThread&) = delete;
		LogThread& operator=(const LogThread&) = delete;

		~LogThread(void);

		void Stop(void);

		void SetLogLevel(const LogLevel&);
		void HandoverLog(const LogLevel&, const String&);

	private:
		std::atomic<LogLevel> mLogLevel;
		LogQueue mLogQueue;
		LogWorker mLogWorker;
	};

	Log::LogThread::LogThread(void)
		: mLogLevel(LogLevel::NONE)
		, mLogWorker(mLogQueue)
		, BaseThread(&mLogWorker)
	{

	}

	Log::LogThread::~LogThread(void)
	{

	}

	void Log::LogThread::Stop(void)
	{
		mLogWorker.doLog = false;
	}

	void Log::LogThread::SetLogLevel(const LogLevel& logLevel)
	{
		mLogLevel = logLevel;
	}

	void Log::LogThread::HandoverLog(const LogLevel& logLevel, const String& logString)
	{
		if (!mLogWorker.doLog) return;
		if (logLevel < mLogLevel) return;

		auto logData = mk_shptr<LogData>();
		logData->logTime = std::chrono::system_clock::now();
		logData->logLevel = logLevel;
		logData->logString = logString;

		mLogQueue.Push(logData);
	}

	Log::Log(void)
		: pLT(mk_uptr<LogThread>())
	{

	}

	Log::~Log(void)
	{
		pLT->Join();
	}

	void Log::SetLogLevel(const LogLevel& logLevel)
	{
		pLT->SetLogLevel(logLevel);
	}

	void Log::Start(void)
	{
		pLT->Start();
	}

	void Log::Stop(void)
	{
		pLT->Stop();
	}

	void Log::C(const String& str)
	{
		pLT->HandoverLog(LogLevel::CRITICAL, str);
	}

	void Log::E(const String& str)
	{
		pLT->HandoverLog(LogLevel::ERROR, str);
	}

	void Log::D(const String& str)
	{
		pLT->HandoverLog(LogLevel::DEBUG, str);
	}

	void Log::W(const String& str)
	{
		pLT->HandoverLog(LogLevel::WARNING, str);
	}

	void Log::N(const String& str)
	{
		pLT->HandoverLog(LogLevel::NOTICE, str);
	}

	void Log::I(const String& str)
	{
		pLT->HandoverLog(LogLevel::INFO, str);
	}

}