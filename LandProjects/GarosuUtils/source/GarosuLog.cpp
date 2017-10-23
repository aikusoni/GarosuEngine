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
			//os << "CRITICAL";
			os << "C";
			break;
		case LogLevel::ERROR:
			//os << "ERROR";
			os << "E";
			break;
		case LogLevel::DEBUG:
			//os << "DEBUG";
			os << "D";
			break;
		case LogLevel::WARNING:
			//os << "WARNING";
			os << "W";
			break;
		case LogLevel::NOTICE:
			//os << "NOTICE";
			os << "N";
			break;
		case LogLevel::INFO:
			//os << "INFO";
			os << "I";
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

	/*
	*
	*
	*
	*/
	class LogLoop : public LoopThread
	{
	public:
		LogLoop(LogQueue& logQueue);
		virtual ~LogLoop(void);
		
		virtual bool OnBegin(void);
		virtual bool OnLoop(void);
		virtual bool OnEnd(void);

	private:
		LogQueue& mLogQueue;
		std::fstream fs;
		std::function<void(LogData*)> output;
	};

	LogLoop::LogLoop(LogQueue& logQueue)
		: mLogQueue(logQueue)
	{

	}

	LogLoop::~LogLoop(void)
	{

	}

	bool LogLoop::OnBegin(void)
	{
		// open file
		fs.open(Settings::GetLogPath().c_str(), std::fstream::out);
		if (!fs.is_open()) return false;

		output = [&](LogData* logData) { fs << *logData << std::endl; };

		return true;
	}

	bool LogLoop::OnLoop(void)
	{
		shptr<LogData> logData = mLogQueue.Pop();
		if (logData != nullptr) output(logData.get());

		return true;
	}

	bool LogLoop::OnEnd(void)
	{
		// close file and flush logs
		shptr<LogData> logData = nullptr;
		while ((logData = mLogQueue.Pop()) != nullptr)
			output(logData.get());

		if (fs.is_open()) fs.close();

		return true;
	}

	class Log::impl
	{
	public:
		impl(void);
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;

		virtual ~impl(void);

		void SetLogLevel(const LogLevel&);
		void HandoverLog(const LogLevel&, const String&);

		std::atomic<LogLevel> mLogLevel;
		LogQueue mLogQueue;
		LogLoop mLoggingLoop;

		Locker mStartingLock;
		bool mStarted = false;
	};

	Log::impl::impl(void)
		: mLogLevel(LogLevel::NONE)
		, mLoggingLoop(mLogQueue)
	{

	}

	Log::impl::~impl(void)
	{

	}

	void Log::impl::SetLogLevel(const LogLevel& logLevel)
	{
		mLogLevel = logLevel;
	}

	void Log::impl::HandoverLog(const LogLevel& logLevel, const String& logString)
	{
		if (logLevel > mLogLevel) return;

		auto logData = mk_shptr<LogData>();
		logData->logTime = std::chrono::system_clock::now();
		logData->logLevel = logLevel;
		logData->logString = logString;

		mLogQueue.Push(logData);
	}

	Log::Log(void)
		: pImpl(mk_uptr<impl>())
	{

	}

	Log::~Log(void)
	{
	}

	void Log::SetLogLevel(const LogLevel& logLevel)
	{
		pImpl->SetLogLevel(logLevel);
	}

	void Log::Start(void)
	{
		pImpl->mStartingLock.Lock();
		if (pImpl->mStarted) {
			pImpl->mStartingLock.Unlock();
			return;
		}
		pImpl->mStarted = true;

		pImpl->mLoggingLoop.Start();
		pImpl->mStartingLock.Unlock();
	}

	void Log::Stop(void)
	{
		pImpl->mStartingLock.Lock();
		pImpl->mStarted = false;

		pImpl->mLoggingLoop.Stop();
		pImpl->mLoggingLoop.Join();
		pImpl->mStartingLock.Unlock();
	}

	void Log::Logging(LogLevel mLogLevel, const String& str)
	{
		pImpl->HandoverLog(mLogLevel, str);
	}

}