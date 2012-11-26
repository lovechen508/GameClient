#ifndef _INCLUDE_YCNETWORK_H_
#define _INCLUDE_YCNETWORK_H_

#define HOST_NAME 100

#include "YCMSGDefine.h"
#include "YCDecodeHelper.h"

#include "YCBasic\YCDef.h"
#include "YCInput\YCIInputListener.h"

class YCLink;
class YCInput;
class YCConfig;
class YCIStreamEncryption;
class YCPackageHandlerImpl;
class YCINetworkCallback;

/*
 * YCNetwork : ����ӿڳ�����
 *
 * ˵����
 *
 *    ͬ����Ϣ��ͨ��get�����Զ�����(��֧��)
 *
 *    �첽��Ϣ��ͨ���򴰿ڷ���WM_NETWORK_MSG��Ϣ
 *             ͳһͨ��YCInput�ɷ�
 *             UINT message  ��WM_NETWORK_MSG 
 *             WPARAM wParam ��socket
 *             LPARAM lParam ��error
 */
class YCAPI YCNetwork
{
public:

	YCNetwork();

	virtual ~YCNetwork();

	//
	// ������initialize(HWND hWnd, YCInput* input, YCConfig& config)
	//
	// Ŀ�ģ���ʼ����������
	//
	bool initialize(HWND hWnd, YCInput* input, YCConfig& config);

	//
	// ������connect(const char* host, int port)
	//
	// Ŀ�ģ���������
	//
	bool connect(const char* host, int port);

	//
	// ������bind(T* host, P& p)
	//
	// Ŀ�ģ����Ͱ�
	//
	template<typename T, typename P>
	bool bind(T* host, unsigned short msgId, P&)
	{
		if (host == NULL)
		{
			return false;
		}

		if (msgId >= MAX_PROTOCOL)
		{
			return false;
		}

		if (myCallbacks[msgId] != NULL)
		{
			return false;
		}

		myCallbacks[msgId] = new YCDecodeHelper<T, P>(host, myDecoders[msgId]);

		return true;
	}
	
	//
	// ������unbind(unsigned int msgId)
	//
	// Ŀ�ģ����Ӧ�ò���Ϣ����
	//
	void unbind(unsigned int msgId);

	//
	// ������handlePackage(unsigned short msgId, char* buf, unsigned int len)
	//
	// Ŀ�ģ�YCLink�ص����յ��������
	//
	void handlePackage(unsigned short msgId, char* buf, unsigned int len);

	//
	// ������bindEncryption(YCIStreamEncryption* encryption)
	//
	// Ŀ�ģ��󶨼ӽ����㷨,���ؾ��㷨
	//
	YCIStreamEncryption* bindEncryption(YCIStreamEncryption* encryption);

	//
	// ������get(YCIPackageRequest* request)
	//
	// Ŀ�ģ���������ύ������������
	//
	//YCIPackageResponse* get(YCIPackageRequest* request);

	//
	// ������post(unsigned short msgId, YCIPackageRequest* request)
	//
	// Ŀ�ģ���������ύ�����첽��Ϣ
	//
	void post(unsigned short msgId, YCIPackageRequest* request);

	//
	// ������close()
	//
	// Ŀ�ģ��ر�����
	//
	void close();

	//
	// ������registry(unsigned int msgId, ENCODE encode, DECODE decode)
	//
	// Ŀ�ģ�ע���������������
	//
	void registry(unsigned int msgId, ENCODE encode, DECODE decode);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ���������
	//
	void finalize();

private:

	HWND myWnd;	

	YCInput *myInput;

	YCLink* myConnection;     //��ʵ����

	YCIStreamEncryption* myEncrypt;

	ENCODE myEncoders[MAX_PROTOCOL];
	DECODE myDecoders[MAX_PROTOCOL];

	YCINetworkCallback *myCallbacks[MAX_PROTOCOL];
};

#endif