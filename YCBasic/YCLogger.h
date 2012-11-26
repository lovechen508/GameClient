#ifndef _INCLUDE_YCLOGGER_H_
#define _INCLUDE_YCLOGGER_H_

#include "YCDef.h"

#include <fstream>

#include "YCAssert.h"
#include "YCRegistry.h"

/*
 * 只在Debug条件下允许输出Trace, Debug, Info信息
 * 在Debug,Release下都可以输出WARNING,ERROR,FATAL信息
 *
 * 使用样例：
 *    LOG_DEBUG("加载纹理：" << filename << " success!");
 */

#define LOG(level, data) do {\
							 YCLogger* logger = (YCLogger*)YCRegistry::get("logger");\
							 if (logger && logger->isEnable(level)) \
							 { \
                                 logger->prepare(level) << data << std::endl; \
							 } \
						 } while(0)

#ifdef _DEBUG
  #define LOG_TRACE(data) LOG(YCLogger::LOG_TRACE, data)
  #define LOG_DEBUG(data) LOG(YCLogger::LOG_DEBUG, data)
  #define LOG_INFO(data)  LOG(YCLogger::LOG_INFO,  data)
#else
  #define LOG_TRACE(data)
  #define LOG_DEBUG(data)
  #define LOG_INFO(data)
#endif

#define LOG_WARNING(data) LOG(YCLogger::LOG_WARNING, data)
#define LOG_ERROR(data)   LOG(YCLogger::LOG_ERROR,   data)
#define LOG_FATAL(data)   LOG(YCLogger::LOG_FATAL,   data)

class YCLogger
{
public:

	enum LOG_LEVEL{
		LOG_TRACE		= 1,
		LOG_DEBUG		= 2,
		LOG_INFO		= 3,
		LOG_WARNING		= 4,
		LOG_ERROR       = 5,
		LOG_FATAL		= 6
	};

	YCLogger() : myLogLevel(LOG_WARNING), myCurrentDay(0) 
	{
	}

	~YCLogger() 
	{
		myFstream.close();
	}

	//
	// 函数：initailize(const char* path, int level)
	//
	// 目的：初始化日志存储路径，日志级别
	//       日期文件名格式为 YYYY-MM-DD.log
	//
	bool initailize(const char* path, LOG_LEVEL level)
	{
		SMART_ASSERT(path != NULL);

		if (level >= LOG_TRACE && level <= LOG_FATAL)
		{
			myLogLevel = level;
		}
		myLogPath = path;
		
		SYSTEMTIME sys;
		::GetLocalTime(&sys);
		myCurrentDay = MAKELONG(sys.wYear, MAKEWORD(sys.wMonth, sys.wDay));
		char filename[512] = {0};
		sprintf_s(filename, "%s\\%4d-%02d-%02d.log", 
			    path, sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
		myFstream.open(filename, std::ios::out | std::ios::app);
		return myFstream.good();
	}

	//
	// 函数：isEnable(int level)
	//
	// 目的：检查是否应该记录该日志
	//
	bool isEnable(LOG_LEVEL level)
	{
		return (level >= myLogLevel);
	}

	//
	// 函数：Writer()
	//
	// 目的：获取日志文件文件流并输出日志头
	//       
	// 注释：副作用，因为每天都要切换一个新的Log文件
	//       所以每次调用都要检查时间戳，于是就同时把
	//       fstream更新好
	//
	std::ofstream& prepare(LOG_LEVEL level)
	{
		if (myFstream.good())
		{
			SYSTEMTIME sys;
			::GetLocalTime(&sys);
			long nowDay = MAKELONG(sys.wYear, MAKEWORD(sys.wMonth, sys.wDay));
			if (nowDay != myCurrentDay)
			{
				myFstream.close();
				char filename[512] = {0};
				sprintf_s(filename, "%s\\%4d-%02d-%02d.log", 
						myLogPath.c_str(), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
				myFstream.open(filename, std::ios::out | std::ios::app);
				if (!myFstream.good())
				{
					return myFstream;
				}
			}

			if      (level == LOG_TRACE)    myFstream << "[T] "; //myFstream << "[TRACE]   ";
			else if (level == LOG_DEBUG)    myFstream << "[D] "; //myFstream << "[DEBUG]   ";
			else if (level == LOG_INFO)     myFstream << "[I] "; //myFstream << "[INFO]    ";
			else if (level == LOG_WARNING)  myFstream << "[W] "; //myFstream << "[WARNING] ";
			else if (level == LOG_ERROR)	myFstream << "[E] "; //myFstream << "[ERROR]   ";
			else							myFstream << "[F] "; //myFstream << "[FATAL]   ";

			char date[32] = {0};
			//sprintf_s(date, "%4d-%02d-%02d %02d:%02d:%02d.%03d ", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
			sprintf_s(date, "%02d:%02d:%02d.%03d ", sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
			myFstream << date;
		}
		return myFstream;
	}

private:

	std::string myLogPath;

	long myCurrentDay;

	std::ofstream myFstream;

	LOG_LEVEL myLogLevel;
};

#endif