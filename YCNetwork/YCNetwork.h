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
 * YCNetwork : 网络接口抽象类
 *
 * 说明：
 *
 *    同步消息：通过get函数自动返回(不支持)
 *
 *    异步消息：通过向窗口发送WM_NETWORK_MSG消息
 *             统一通过YCInput派发
 *             UINT message  ：WM_NETWORK_MSG 
 *             WPARAM wParam ：socket
 *             LPARAM lParam ：error
 */
class YCAPI YCNetwork
{
public:

	YCNetwork();

	virtual ~YCNetwork();

	//
	// 函数：initialize(HWND hWnd, YCInput* input, YCConfig& config)
	//
	// 目的：初始化网络连接
	//
	bool initialize(HWND hWnd, YCInput* input, YCConfig& config);

	//
	// 函数：connect(const char* host, int port)
	//
	// 目的：发起连接
	//
	bool connect(const char* host, int port);

	//
	// 函数：bind(T* host, P& p)
	//
	// 目的：泛型绑定
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
	// 函数：unbind(unsigned int msgId)
	//
	// 目的：解除应用层消息处理
	//
	void unbind(unsigned int msgId);

	//
	// 函数：handlePackage(unsigned short msgId, char* buf, unsigned int len)
	//
	// 目的：YCLink回调接收到的网络包
	//
	void handlePackage(unsigned short msgId, char* buf, unsigned int len);

	//
	// 函数：bindEncryption(YCIStreamEncryption* encryption)
	//
	// 目的：绑定加解密算法,返回旧算法
	//
	YCIStreamEncryption* bindEncryption(YCIStreamEncryption* encryption);

	//
	// 函数：get(YCIPackageRequest* request)
	//
	// 目的：向服务器提交请求，阻塞函数
	//
	//YCIPackageResponse* get(YCIPackageRequest* request);

	//
	// 函数：post(unsigned short msgId, YCIPackageRequest* request)
	//
	// 目的：向服务器提交请求，异步消息
	//
	void post(unsigned short msgId, YCIPackageRequest* request);

	//
	// 函数：close()
	//
	// 目的：关闭连接
	//
	void close();

	//
	// 函数：registry(unsigned int msgId, ENCODE encode, DECODE decode)
	//
	// 目的：注册解包，封包函数对
	//
	void registry(unsigned int msgId, ENCODE encode, DECODE decode);

	//
	// 函数：finalize()
	//
	// 目的：释放网络连接
	//
	void finalize();

private:

	HWND myWnd;	

	YCInput *myInput;

	YCLink* myConnection;     //真实连接

	YCIStreamEncryption* myEncrypt;

	ENCODE myEncoders[MAX_PROTOCOL];
	DECODE myDecoders[MAX_PROTOCOL];

	YCINetworkCallback *myCallbacks[MAX_PROTOCOL];
};

#endif