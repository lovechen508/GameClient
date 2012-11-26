#ifndef _INCLUDE_YCLOGGER_H_
#define _INCLUDE_YCLOGGER_H_

#include "YCDef.h"

#include <fstream>

#include "YCAssert.h"
#include "YCRegistry.h"

/*
 * ֻ��Debug�������������Trace, Debug, Info��Ϣ
 * ��Debug,Release�¶��������WARNING,ERROR,FATAL��Ϣ
 *
 * ʹ��������
 *    LOG_DEBUG("��������" << filename << " success!");
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
	// ������initailize(const char* path, int level)
	//
	// Ŀ�ģ���ʼ����־�洢·������־����
	//       �����ļ�����ʽΪ YYYY-MM-DD.log
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
	// ������isEnable(int level)
	//
	// Ŀ�ģ�����Ƿ�Ӧ�ü�¼����־
	//
	bool isEnable(LOG_LEVEL level)
	{
		return (level >= myLogLevel);
	}

	//
	// ������Writer()
	//
	// Ŀ�ģ���ȡ��־�ļ��ļ����������־ͷ
	//       
	// ע�ͣ������ã���Ϊÿ�춼Ҫ�л�һ���µ�Log�ļ�
	//       ����ÿ�ε��ö�Ҫ���ʱ��������Ǿ�ͬʱ��
	//       fstream���º�
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