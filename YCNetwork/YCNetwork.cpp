#include "YCNetwork.h"

#include <map>

#include "YCLink.h"

#include "YCInput\YCInput.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCIStreamEncryption.h"

#include "protocol\YCPkg_0001_Version.h"
#include "protocol\YCPkg_0003_Error.h"
#include "protocol\YCPkg_0005_Login.h"
#include "protocol\YCPkg_0007_Logout.h"
#include "protocol\YCPkg_0011_Role.h"
#include "protocol\YCPkg_0022_Ping.h"
#include "protocol\YCPkg_0080_Chat.h"
#include "protocol\YCPkg_1000_EnterScene.h"
#include "protocol\YCPkg_1002_LeaveScene.h"

YCNetwork::YCNetwork()
	: myConnection(NULL)
	, myInput(NULL)
{
}

YCNetwork::~YCNetwork()
{
}

//
// 函数：initialize(HWND hWnd, YCConfig* config)
//
// 目的：初始化网络连接
//
bool YCNetwork::initialize(HWND hWnd, YCInput* input, YCConfig& config)
{
	myWnd = hWnd;
	myInput = input;

	memset(myEncoders, 0, sizeof(ENCODE) * MAX_PROTOCOL);
	memset(myDecoders, 0, sizeof(DECODE) * MAX_PROTOCOL);

	memset(myCallbacks, 0, sizeof(YCINetworkCallback*) * MAX_PROTOCOL);

	// 处理config

    // 注册网络解封包函数
    YCPkg_0001_Version_init(this);
    YCPkg_0003_Error_init(this);
    YCPkg_0005_Login_init(this);
    YCPkg_0007_Logout_init(this);
    YCPkg_0011_Role_init(this);
    YCPkg_0022_Ping_init(this);
    YCPkg_0080_Chat_init(this);
    YCPkg_1000_EnterScene_init(this);
    YCPkg_1002_LeaveScene_init(this);

	return true;
}

//
// 函数：connect(const char* host, int port)
//
// 目的：发起连接
//
bool YCNetwork::connect(const char* host, int port)
{
	bool result = false;
	try 
	{
		if (myConnection != NULL)
		{
			close();
		}

		myConnection = new YCLink(this, myWnd);
		if (myConnection->connect(host, port))
		{
			myInput->subscribeListener(myConnection, WM_NETWORK_MSG);
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCNetwork::connect异常：" << e.what());	
		return false;
	}
	
	return true;
}

//
// 函数：unbind(unsigned int msgId)
//
// 目的：解除应用层消息处理
//
void YCNetwork::unbind(unsigned int msgId)
{
	if (msgId >= MAX_PROTOCOL)
	{
		throw YCException(2002, "YCNetwork::unbind无效消息号");
	}

	SAFE_DELETE(myCallbacks[msgId]);
}

//
// 函数：handlePackage(unsigned short msgId, char* buf, unsigned int len)
//
// 目的：YCLink回调接收到的网络包
//
void YCNetwork::handlePackage(unsigned short msgId, char* buf, unsigned int len)
{
	SMART_ASSERT(buf != NULL);
	SMART_ASSERT(len > 0);

	if (buf == NULL || len <= 0)
	{
		throw YCException(2002, "YCNetwork::handlePackage无效网络包");
	}
	
	try
	{
		if (myCallbacks[msgId] == NULL)
		{
			throw YCException(2002, "YCNetwork::handlePackage 未处理网络消息：", msgId);
		}

		if (myEncrypt != NULL)
		{
			if (!myEncrypt->decode(buf, len, buf, len))
			{
				throw YCException(2002, "YCPackageHandlerImpl::decode封包解密失败！");;
			}
		}

		myCallbacks[msgId]->handle(buf, len);
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCNetwork::handlePackage 处理网络消息错误：" << e.what());
	}
}

//
// 函数：bindEncryption(YCIStreamEncryption* encryption)
//
// 目的：绑定加解密算法,返回旧算法
//
YCIStreamEncryption* YCNetwork::bindEncryption(YCIStreamEncryption* encryption)
{
	YCIStreamEncryption* old = myEncrypt;
	myEncrypt = encryption;
	return old;
}

//
// 函数：registry(unsigned int msgId, ENCODE encode, DECODE decode)
//
// 目的：注册解包，封包函数对
//
void YCNetwork::registry(unsigned int msgId, ENCODE encode, DECODE decode)
{
	SMART_ASSERT(msgId < MAX_PROTOCOL);
	SMART_ASSERT(encode != NULL);
	SMART_ASSERT(decode != NULL);

	if (encode == NULL)
	{
		throw YCException(2002, "YCNetwork::registry注册无效封包器");
	}

	if (decode == NULL)
	{
		throw YCException(2002, "YCNetwork::registry注册无效解包器");
	}

	if (msgId >= MAX_PROTOCOL)
	{
		throw YCException(2002, "YCNetwork::registry注册无效消息号");
	}

	myEncoders[msgId] = encode;
	myDecoders[msgId] = decode;
}

//
// 函数：close()
//
// 目的：关闭连接
//
void YCNetwork::close()
{
	try 
	{
		if (myConnection != NULL)
		{
			myInput->unsubscribeListener(myConnection, WM_NETWORK_MSG);
			myConnection->close();		
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCNetwork::finalize网络释放失败：" << e.what());
	}

	SAFE_DELETE(myConnection);
}

//
// 函数：finalize()
//
// 目的：释放网络连接
//
void YCNetwork::finalize()
{	
	if (myConnection != NULL)
	{
		close();
	}

	for (int i = 0; i < MAX_PROTOCOL; ++i)
	{
		SAFE_DELETE(myCallbacks[i]);
	}
}

////////////////////////////////////////////////////////////////////////
// Private Part
////////////////////////////////////////////////////////////////////////
//
// 函数：post 函数实现
//
void YCNetwork::do_post(char* buf, unsigned int len)
{

    if (myEncrypt != NULL)
    {
        if (!myEncrypt->encode(buf, len, buf, len))
        {
            throw YCException(2002, "YCPackageHandlerImpl::encode封包加密失败！");;
        }
    }

    myConnection->post(buf, len);
}
