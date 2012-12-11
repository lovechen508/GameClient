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
// ������initialize(HWND hWnd, YCConfig* config)
//
// Ŀ�ģ���ʼ����������
//
bool YCNetwork::initialize(HWND hWnd, YCInput* input, YCConfig& config)
{
	myWnd = hWnd;
	myInput = input;

	memset(myEncoders, 0, sizeof(ENCODE) * MAX_PROTOCOL);
	memset(myDecoders, 0, sizeof(DECODE) * MAX_PROTOCOL);

	memset(myCallbacks, 0, sizeof(YCINetworkCallback*) * MAX_PROTOCOL);

	// ����config

    // ע�������������
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
// ������connect(const char* host, int port)
//
// Ŀ�ģ���������
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
		LOG_WARNING("YCNetwork::connect�쳣��" << e.what());	
		return false;
	}
	
	return true;
}

//
// ������unbind(unsigned int msgId)
//
// Ŀ�ģ����Ӧ�ò���Ϣ����
//
void YCNetwork::unbind(unsigned int msgId)
{
	if (msgId >= MAX_PROTOCOL)
	{
		throw YCException(2002, "YCNetwork::unbind��Ч��Ϣ��");
	}

	SAFE_DELETE(myCallbacks[msgId]);
}

//
// ������handlePackage(unsigned short msgId, char* buf, unsigned int len)
//
// Ŀ�ģ�YCLink�ص����յ��������
//
void YCNetwork::handlePackage(unsigned short msgId, char* buf, unsigned int len)
{
	SMART_ASSERT(buf != NULL);
	SMART_ASSERT(len > 0);

	if (buf == NULL || len <= 0)
	{
		throw YCException(2002, "YCNetwork::handlePackage��Ч�����");
	}
	
	try
	{
		if (myCallbacks[msgId] == NULL)
		{
			throw YCException(2002, "YCNetwork::handlePackage δ����������Ϣ��", msgId);
		}

		if (myEncrypt != NULL)
		{
			if (!myEncrypt->decode(buf, len, buf, len))
			{
				throw YCException(2002, "YCPackageHandlerImpl::decode�������ʧ�ܣ�");;
			}
		}

		myCallbacks[msgId]->handle(buf, len);
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCNetwork::handlePackage ����������Ϣ����" << e.what());
	}
}

//
// ������bindEncryption(YCIStreamEncryption* encryption)
//
// Ŀ�ģ��󶨼ӽ����㷨,���ؾ��㷨
//
YCIStreamEncryption* YCNetwork::bindEncryption(YCIStreamEncryption* encryption)
{
	YCIStreamEncryption* old = myEncrypt;
	myEncrypt = encryption;
	return old;
}

//
// ������registry(unsigned int msgId, ENCODE encode, DECODE decode)
//
// Ŀ�ģ�ע���������������
//
void YCNetwork::registry(unsigned int msgId, ENCODE encode, DECODE decode)
{
	SMART_ASSERT(msgId < MAX_PROTOCOL);
	SMART_ASSERT(encode != NULL);
	SMART_ASSERT(decode != NULL);

	if (encode == NULL)
	{
		throw YCException(2002, "YCNetwork::registryע����Ч�����");
	}

	if (decode == NULL)
	{
		throw YCException(2002, "YCNetwork::registryע����Ч�����");
	}

	if (msgId >= MAX_PROTOCOL)
	{
		throw YCException(2002, "YCNetwork::registryע����Ч��Ϣ��");
	}

	myEncoders[msgId] = encode;
	myDecoders[msgId] = decode;
}

//
// ������close()
//
// Ŀ�ģ��ر�����
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
		LOG_WARNING("YCNetwork::finalize�����ͷ�ʧ�ܣ�" << e.what());
	}

	SAFE_DELETE(myConnection);
}

//
// ������finalize()
//
// Ŀ�ģ��ͷ���������
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
// ������post ����ʵ��
//
void YCNetwork::do_post(char* buf, unsigned int len)
{

    if (myEncrypt != NULL)
    {
        if (!myEncrypt->encode(buf, len, buf, len))
        {
            throw YCException(2002, "YCPackageHandlerImpl::encode�������ʧ�ܣ�");;
        }
    }

    myConnection->post(buf, len);
}
