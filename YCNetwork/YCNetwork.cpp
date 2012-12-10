#include "YCNetwork.h"

#include <map>

#include "YCLink.h"
#include "YCIPackageRequest.h"
#include "YCIPackageResponse.h"

#include "YCInput\YCInput.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCIStreamEncryption.h"

#pragma region testCode

// ���Դ���
struct TEST_PKG : public YCIPackageResponse
{
	int id;
	char buf[10];
};

class TestHandler
{
public:

	void handle(const TEST_PKG& pkg)
	{

	}
};

//���Դ���
//TestHandler gTestHandler;
//myNetworker->bind(&gTestHandler, 1, TEST_PKG());

#pragma endregion testCode

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
// ������get(YCIPackageRequest* request)
//
// Ŀ�ģ���������ύ������������
//
//YCIPackageResponse* YCNetwork::get(YCIPackageRequest* request)
//{
//	return NULL;
//}

//
// ������post(unsigned short msgId, YCIPackageRequest* request)
//
// Ŀ�ģ���������ύ�����첽��Ϣ
//
void YCNetwork::post(unsigned short msgId, YCIPackageRequest* request)
{
	if (myConnection == NULL)
	{
		throw YCException(2002, "YCNetwork::post����������δ����");
	}

	if (request == NULL)
	{
		throw YCException(2002, "YCNetwork::post�ύNULL������Ϣ");
	}

	if (myEncoders[msgId] == NULL)
	{
		throw YCException(2002, "YCNetwork::postδע��������", msgId);
	}

	char buf[BUFFER_SIZE];
	unsigned int len = BUFFER_SIZE;
	YCDataHolder holder(buf, len);
	if (myEncoders[msgId](request, &holder))
	{
		throw YCException(2002, "YCNetwork::post�����������ʧ��", msgId);
	}

	if (myEncrypt != NULL)
	{
		if (!myEncrypt->encode(buf, len, buf, len))
		{
			throw YCException(2002, "YCPackageHandlerImpl::encode�������ʧ�ܣ�");;
		}
	}

	myConnection->post(buf, len);
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
