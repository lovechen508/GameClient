#include "YCShell.h"

#include <map>
#include <string>
#include <iostream>

#include "YCDef.h"
#include "YCAssert.h"
#include "YCException.h"
#include "YCIShellCommand.h"
#include "YCIShellInterceptor.h"

#pragma region YCShellImpl

////////////////////////////////////////////////////
// YCShellImpl ：Shell内部实现类
////////////////////////////////////////////////////

class YCShellImpl
{
	typedef std::map<std::string, YCIShellCommand*> CMDMAP_T;

public:

	YCShellImpl() : myPrompt("#"), myQuitFlag(false), myInterceptor(NULL)
	{
	}

	~YCShellImpl()
	{
		SAFE_DELETE(myInterceptor);
		
		for (CMDMAP_T::iterator it = myCommands.begin(); 
			it != myCommands.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
	}

	//
	// 注释：循环运行知道用户关闭CMD窗口
	//
	void run()
	{
		while (!myQuitFlag)
		{
			try
			{
				std::string cmd;
				std::cout << myPrompt << " ";
				std::cin >> cmd;

				handle(cmd.c_str(), cmd.length());
			}
			catch (YCException& e)
			{
				std::cerr << "error:" << e.what() << std::endl;
			}
		}
	}

	//
	// 注释：注册命令拦截器，可以用于登录，登出检查等
	//
	void registInterceptor(YCIShellInterceptor* interceptor)
	{
		SMART_ASSERT(interceptor != NULL);
		if (interceptor == NULL)
		{
			throw YCException(1002, "YCShell::registInterceptor注册空拦截器");
		}

		if (myInterceptor != NULL)
		{
			throw YCException(1002, "YCShell::registInterceptor拦截器注册重复");
		}

		myInterceptor = interceptor;
	}

	void registCommander(YCIShellCommand* commander)
	{
		SMART_ASSERT(commander != NULL);
		if (commander == NULL)
		{
			throw YCException(1002, "YCShell::registInterceptor注册空处理器");
		}

		const char* name = commander->name();
		if (myCommands.end() != myCommands.find(name))
		{
			throw YCException(1002, "YCShell::registInterceptor注册处理器重复", name);
		}

		myCommands[commander->name()] = commander;
	}

	//
	// 注释：处理命令
	//
	void handle(const char* cmd, int len)
	{
		SMART_ASSERT(cmd != NULL);
		SMART_ASSERT(len > 0);

		if (cmd == NULL || len <= 0)
		{
			throw YCException(1002, "YCShell:handle无效命令");
		}

		// 拦截
		if (!intercepted(cmd, len))
		{
			const std::string& cmdName = command(cmd,len);
			CMDMAP_T::iterator at = myCommands.find(cmdName);
			if (at == myCommands.end())
			{
				throw YCException(1002, "YCShell::handle未知命令", cmd);
			}
			at->second->handle(cmd, len);
		}		
	}

	//
	// 注释：获取提示符
	//
	const char* getPrompt() const
	{
		return myPrompt.c_str();
	}

	//
	// 注释：设置提示符
	//
	void setPrompt(const char* prompt)
	{
		SMART_ASSERT(prompt != NULL);
		if (prompt == NULL)
		{
			throw YCException(1002, "YCShell::setPrompt设置空提示符");
		}
		myPrompt = prompt;
	}

	//
	// 注释：退出Shell
	//
	void quit()
	{
		myQuitFlag = true;
	}

private:

	//
	// 注释：取得命令名
	//
	std::string command(const char* cmd, int len)
	{
		std::string strCmd(cmd, len);
		unsigned int pos = strCmd.find_first_of(" ");
		return (pos == std::string::npos) ? strCmd : strCmd.substr(0, pos);
	}

	//
	// 注释：尝试拦截，
	//       
	//      被拦截不后续处理，返回true
	//      无拦截需后续处理，返回false
	//
	//
	bool intercepted(const char* cmd, int len)
	{
		return (myInterceptor == NULL ? false : myInterceptor->handle(cmd, len));
	}

private:

	//
	// 退出标志，每次命令处理结束都要检查
	//
	bool myQuitFlag;

	//
	// 属性：命令提示符
	//
	std::string myPrompt;

	//
	// 属性：拦截器列表
	//
	YCIShellInterceptor* myInterceptor;

	//
	// 属性：
	//
	CMDMAP_T myCommands;

};

#pragma endregion YCShellImpl

YCShell::YCShell(const char* prompt)
	: myImpl(new YCShellImpl())
{
	myImpl->setPrompt(prompt);
}

YCShell::~YCShell(void)
{
	SAFE_DELETE(myImpl);
}

//
// 注释：注册命令拦截器，可以用于登录，登出检查等
//
YCShell& YCShell::registInterceptor(YCIShellInterceptor* interceptor)
{
	myImpl->registInterceptor(interceptor);
	interceptor->bind(this);
	return *this;
}

//
// 注释：注册命令处理器
//
YCShell& YCShell::registCommander(YCIShellCommand* commander)
{
	myImpl->registCommander(commander);
	commander->bind(this);
	return *this;
}

//
// 注释：循环运行知道用户关闭CMD窗口
//
void YCShell::run()
{
	myImpl->run();
}

//
// 注释：处理命令
//
void YCShell::handle(const char* cmd, int len)
{
	myImpl->handle(cmd, len);
}

//
// 注释：获取提示符
//
const char* YCShell::getPrompt() const
{
	return myImpl->getPrompt();
}

//
// 注释：设置提示符
//
void YCShell::setPrompt(const char* prompt)
{
	myImpl->setPrompt(prompt);
}

//
// 注释：退出Shell
//
void YCShell::quit()
{
	myImpl->quit();
}
