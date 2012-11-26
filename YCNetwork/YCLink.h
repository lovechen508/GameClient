#ifndef _INCLUDE_YCLINK_H_
#define _INCLUDE_YCLINK_H_

#include <string>
#include <winSock2.h>

#include "YCBasic\YCDef.h"
#include "YCInput\YCIInputListener.h"

//
// 链路层解包，封包
//
class YCLinkPackager;

class YCNetwork;

#define WM_NETWORK_MSG WM_USER+100

/*
 * YCLink : TCP/IP 连接抽象
 *
 *          不在DLL接口中暴露
 */
class YCLink : public YCIInputListener
{
public:
	
	YCLink(YCNetwork* network, HWND hWnd);
	
	virtual ~YCLink(void);

	//
	// 函数：connect(const char* host, int port)
	//
	// 目的：向服务器发起连接，阻塞函数
	//
	bool connect(const char* host, int port);

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// 函数：get(YCIPackageRequest* request)
	//
	// 目的：向服务器提交请求，阻塞函数
	//
	//YCIPackageResponse* get(YCIPackageRequest* request);

	//
	// 函数：post(const char* buf, int len)
	//
	// 目的：向服务器提交请求，异步消息
	//
	void post(const char* buf, int len);

	//
	// 函数：close()
	//
	// 目的：主动关闭连接，阻塞函数
	//
	bool close();

private:

	HWND myWnd;

	std::string myHost;

	int myPort;

	SOCKET mySocket;

	YCNetwork* myNetwork;

	YCLinkPackager* myPackager;
};

#endif

