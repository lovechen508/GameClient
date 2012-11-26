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
// YCShellImpl ��Shell�ڲ�ʵ����
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
	// ע�ͣ�ѭ������֪���û��ر�CMD����
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
	// ע�ͣ�ע���������������������ڵ�¼���ǳ�����
	//
	void registInterceptor(YCIShellInterceptor* interceptor)
	{
		SMART_ASSERT(interceptor != NULL);
		if (interceptor == NULL)
		{
			throw YCException(1002, "YCShell::registInterceptorע���������");
		}

		if (myInterceptor != NULL)
		{
			throw YCException(1002, "YCShell::registInterceptor������ע���ظ�");
		}

		myInterceptor = interceptor;
	}

	void registCommander(YCIShellCommand* commander)
	{
		SMART_ASSERT(commander != NULL);
		if (commander == NULL)
		{
			throw YCException(1002, "YCShell::registInterceptorע��մ�����");
		}

		const char* name = commander->name();
		if (myCommands.end() != myCommands.find(name))
		{
			throw YCException(1002, "YCShell::registInterceptorע�ᴦ�����ظ�", name);
		}

		myCommands[commander->name()] = commander;
	}

	//
	// ע�ͣ���������
	//
	void handle(const char* cmd, int len)
	{
		SMART_ASSERT(cmd != NULL);
		SMART_ASSERT(len > 0);

		if (cmd == NULL || len <= 0)
		{
			throw YCException(1002, "YCShell:handle��Ч����");
		}

		// ����
		if (!intercepted(cmd, len))
		{
			const std::string& cmdName = command(cmd,len);
			CMDMAP_T::iterator at = myCommands.find(cmdName);
			if (at == myCommands.end())
			{
				throw YCException(1002, "YCShell::handleδ֪����", cmd);
			}
			at->second->handle(cmd, len);
		}		
	}

	//
	// ע�ͣ���ȡ��ʾ��
	//
	const char* getPrompt() const
	{
		return myPrompt.c_str();
	}

	//
	// ע�ͣ�������ʾ��
	//
	void setPrompt(const char* prompt)
	{
		SMART_ASSERT(prompt != NULL);
		if (prompt == NULL)
		{
			throw YCException(1002, "YCShell::setPrompt���ÿ���ʾ��");
		}
		myPrompt = prompt;
	}

	//
	// ע�ͣ��˳�Shell
	//
	void quit()
	{
		myQuitFlag = true;
	}

private:

	//
	// ע�ͣ�ȡ��������
	//
	std::string command(const char* cmd, int len)
	{
		std::string strCmd(cmd, len);
		unsigned int pos = strCmd.find_first_of(" ");
		return (pos == std::string::npos) ? strCmd : strCmd.substr(0, pos);
	}

	//
	// ע�ͣ��������أ�
	//       
	//      �����ز�������������true
	//      �������������������false
	//
	//
	bool intercepted(const char* cmd, int len)
	{
		return (myInterceptor == NULL ? false : myInterceptor->handle(cmd, len));
	}

private:

	//
	// �˳���־��ÿ������������Ҫ���
	//
	bool myQuitFlag;

	//
	// ���ԣ�������ʾ��
	//
	std::string myPrompt;

	//
	// ���ԣ��������б�
	//
	YCIShellInterceptor* myInterceptor;

	//
	// ���ԣ�
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
// ע�ͣ�ע���������������������ڵ�¼���ǳ�����
//
YCShell& YCShell::registInterceptor(YCIShellInterceptor* interceptor)
{
	myImpl->registInterceptor(interceptor);
	interceptor->bind(this);
	return *this;
}

//
// ע�ͣ�ע���������
//
YCShell& YCShell::registCommander(YCIShellCommand* commander)
{
	myImpl->registCommander(commander);
	commander->bind(this);
	return *this;
}

//
// ע�ͣ�ѭ������֪���û��ر�CMD����
//
void YCShell::run()
{
	myImpl->run();
}

//
// ע�ͣ���������
//
void YCShell::handle(const char* cmd, int len)
{
	myImpl->handle(cmd, len);
}

//
// ע�ͣ���ȡ��ʾ��
//
const char* YCShell::getPrompt() const
{
	return myImpl->getPrompt();
}

//
// ע�ͣ�������ʾ��
//
void YCShell::setPrompt(const char* prompt)
{
	myImpl->setPrompt(prompt);
}

//
// ע�ͣ��˳�Shell
//
void YCShell::quit()
{
	myImpl->quit();
}
