#ifndef _INCLUDE_YCISHELLINTERCEPTOR_H_
#define _INCLUDE_YCISHELLINTERCEPTOR_H_

#include "YCDef.h"
#include "YCIShellCommand.h"

/*
 * YCIShellInterceptor : Shell 命令拦截器
 */
class YCAPI YCIShellInterceptor : public YCIShellCommand
{
public:

	YCIShellInterceptor(YCIShellInterceptor* next)
		: myNext(next)
	{
	}

	virtual ~YCIShellInterceptor()
	{
		SAFE_DELETE(myNext);
	}

	//
	// 注释：处理Shell转发的命令
	//
	virtual bool handle(const char* cmd, int len)
	{
		if (!selfIntercept(cmd, len))
		{
			if (myNext != NULL)
			{
				return myNext->handle(cmd, len);
			}
			return false;
		}		

		return true;
	}

private:

	//
	// 自身拦截
	//
	virtual bool selfIntercept(const char* cmd, int len) = 0;

private:

	YCIShellInterceptor* myNext;
};

#endif