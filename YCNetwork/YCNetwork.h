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
	// 函数：template<typename T> void post(T* request)
	//
	// 目的：向服务器提交请求，异步消息
	//
    template<typename T>
	void post(T* request)
    {
        if (myConnection == NULL)
        {
            throw YCException(2002, "YCNetwork::post网络连接尚未建立");
        }

        if (request == NULL)
        {
            throw YCException(2002, "YCNetwork::post提交NULL网络消息");
        }

        int msgId = YCType2Int<T>::id;
        if (msgId == -1 || myEncoders[msgId] == NULL)
        {
            throw YCException(2002, "YCNetwork::post未注册封包函数", msgId);
        }

        char buf[BUFFER_SIZE];
        unsigned int len = BUFFER_SIZE;
        YCDataHolder holder(buf, len);
        if (myEncoders[msgId](request, &holder))
        {
            throw YCException(2002, "YCNetwork::post封包函数处理失败", msgId);
        }

        do_post(buf, len);
    }

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

    //
    // 函数：post 函数实现
    //
    void do_post(char* buf, unsigned int len);

private:

	HWND myWnd;	

	YCInput *myInput;

	YCLink* myConnection;     //真实连接

	YCIStreamEncryption* myEncrypt;

	ENCODE myEncoders[MAX_PROTOCOL];
	DECODE myDecoders[MAX_PROTOCOL];

	//
	// 实际上是YCDecodeHelper, 持有生命周期
	//
	YCINetworkCallback *myCallbacks[MAX_PROTOCOL];
};

#endif