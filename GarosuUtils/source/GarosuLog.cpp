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

		virtual ~LogQueue(void);

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
		LogWorker(LogQueue& logQueue);
		LogWorker(const LogWorker&) = delete;
		LogWorker& operator=(const LogWorker&) = delete;

		virtual ~LogWorker(void);

		virtual void DoWork(void);

		Locker mLocker;
		Signal mSignal;
		bool isLogging;

		std::atomic<bool> doLoop;
		LogQueue& mLogQueue;
	};

	LogWorker::LogWorker(LogQueue& logQueue) : doLoop(false), isLogging(false), mLogQueue(logQueue) {}
	LogWorker::~LogWorker(void) {}

	void LogWorker::DoWork(void)
	{
		mLocker.Lock();
		// Check the thread started.
		if (isLogging)
		{
			mLocker.Unlock();
			return;
		}
		isLogging = true;
		mLocker.Unlock();

		std::fstream logFile;
		logFile.open(Settings::GetLogPath().c_str(), std::fstream::out);

		if (!logFile.is_open()) return;

		u32 tryCnt = 0u;
		shptr<LogData> logData;

		auto outputLog = [&]() {
			logFile << *logData << std::endl;
		};

		doLoop = true;
		while (doLoop)
		{
			logData = mLogQueue.Pop();
			if (logData == nullptr) {
				tryCnt++;
				if (tryCnt > 1000)
				{
					tryCnt = 0;
					mSignal.wait();
				}
				continue;
			}

			outputLog();
		}

		// flush remaining logs
		while ((logData = mLogQueue.Pop()) != nullptr)
			outputLog();

		if (logFile.is_open()) logFile.close();

		mLocker.Lock();
		isLogging = false;
		mLocker.Unlock();
	}

	class Log::LogThread : public BaseThread
	{
	public:
		LogThread(void);
		LogThread(const LogThread&) = delete;
		LogThread& operator=(const LogThread&) = delete;

		virtual ~LogThread(void);

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
		mLogWorker.doLoop = false;
		mLogWorker.mSignal.notify();
	}

	void Log::LogThread::SetLogLevel(const LogLevel& logLevel)
	{
		mLogLevel = logLevel;
	}

	void Log::LogThread::HandoverLog(const LogLevel& logLevel, const String& logString)
	{
		if (logLevel > mLogLevel) return;

		auto logData = mk_shptr<LogData>();
		logData->logTime = std::chrono::system_clock::now();
		logData->logLevel = logLevel;
		logData->logString = logString;

		mLogQueue.Push(logData);

		mLogWorker.mSignal.notify();
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

	void Log::Logging(LogLevel mLogLevel, const String& str)
	{
		pLT->HandoverLog(LogLevel::CRITICAL, str);
	}

}