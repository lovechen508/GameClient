#ifndef _INCLUDE_YCNETWORK_H_
#define _INCLUDE_YCNETWORK_H_

#define HOST_NAME 100

#include "YCType2Int.h"
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
 *                                                                                    
 *                                           | | | | | | | | |                         
 *                                           YCINetworkListener                       
 *                                                    ^                               | 1. decrypt(buf, len, buf, len)
 *                                    YCNetwork::handleInPackage(buf, len)     ---->  | 
 *                                                    ^                               | 2. decode(Package* pkg, YCDataHolder* holder)
 *                                             YCLink:onMessage                       
 *                                                    ^                               
 *                                            YCInput::Listener                       
 *                                                    ^                               
 *                                    AsyncSocket ready -> WM_NETWORK_MSG   
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
	bool bind(T* host, P&)
	{
		if (host == NULL)
		{
			return false;
		}

		if (msgId >= MAX_PROTOCOL)
		{
			return false;
		}

        int msgId = YCType2Int<P>::id;
		if (msgId == -1 || myCallbacks[msgId] != NULL)
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
	// ������template<typename T> void post(T* request)
	//
	// Ŀ�ģ���������ύ�����첽��Ϣ
	//
    template<typename T>
	void post(T* request)
    {
        if (myConnection == NULL)
        {
            throw YCException(2002, "YCNetwork::post����������δ����");
        }

        if (request == NULL)
        {
            throw YCException(2002, "YCNetwork::post�ύNULL������Ϣ");
        }

        int msgId = YCType2Int<T>::id;
        if (msgId == -1 || myEncoders[msgId] == NULL)
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

        do_post(buf, len);
    }

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

    //
    // ������post ����ʵ��
    //
    void do_post(char* buf, unsigned int len);

private:

	HWND myWnd;	

	YCInput *myInput;

	YCLink* myConnection;     //��ʵ����

	YCIStreamEncryption* myEncrypt;

	ENCODE myEncoders[MAX_PROTOCOL];
	DECODE myDecoders[MAX_PROTOCOL];

	//
	// ʵ������YCDecodeHelper, ������������
	//
	YCINetworkCallback *myCallbacks[MAX_PROTOCOL];
};

#endif