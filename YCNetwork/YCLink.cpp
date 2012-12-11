#include "YCLink.h"

#include "YCNetwork.h"
#include "YCIPHelper.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#include <WinSock2.h>
#pragma comment(lib,"WS2_32")

//
//链路层数据包头部长度
//
//    |<-------------------- Message ----------------------------->|<------------------ Message ----------------
//    |                                                            |
//    V msgId  | length |<-------------- length ------------------>V                                                     
//     ----------------------------------------------------------------------------------------------------------
//    | 2Bytes | 2Bytes |                payload                   |
//     ----------------------------------------------------------------------------------------------------------
//
struct YCLinkLayerPkg
{
	unsigned short msgId;
	unsigned short length;
};

//
// 单次接收缓冲区长度
//
const int RECV_BUFFER_SIZE = 1024;

///////////////////////////////////////////////////////////////
// Link Package Encoder/Decoder
///////////////////////////////////////////////////////////////
class YCLinkPackager
{
public:

	YCLinkPackager() {}

	~YCLinkPackager() {}

	//
	// 函数：getDataSizeInBuffer()
	//       getDataSizeOutBuffer()
	//
	// 目的：
	//       得到进出缓冲区的数据长度
	//
	int getDataSizeInBuffer()   { return myInSize;  }
	int getDataSizeOutBuffer()  { return myOutSize; }

	//
	// 函数：recv(SOCKET)
	//
	// 目的：FD_RECV事件，追加数据到接收缓冲
	//
	bool recv(SOCKET socket)
	{
		int total = 0;
		while (true)
		{
			char tmp[RECV_BUFFER_SIZE];
			int bytes = ::recv(socket, tmp, RECV_BUFFER_SIZE, 0);
			if (bytes == SOCKET_ERROR)
			{
				LOG_ERROR("YCLink::recv 错误：" << WSAGetLastError());
				return false;
			}
			else if (bytes == 0)
			{
				LOG_ERROR("YCLink::recv连接被对方关闭");
				return false;
			}
			else
			{
				if (bytes+myInSize > BUFFER_SIZE)
				{
					throw YCException(2002, "YCLink::recv接收缓冲区满");
				}
				memcpy(myInBuffer+myInSize, tmp, bytes);
				myInSize += bytes;
			}
		}
		return true;
	}

	//
	// 函数：handleInPackage(YCNetwork* network)
	//
	// 目的：解码数据包，回调YCNetwork
	//
	void handleInPackage(YCNetwork* network)
	{
		//network->handlePackage(unsigned int msgId, const char* buf, int len);
		SMART_ASSERT(network != NULL);
		if (network == NULL)
		{
			throw YCException(2002, "YCLink::handleInPackage 处理指针为空");
		}
		
		while (true)
		{
			if (myInSize < sizeof(YCLinkLayerPkg))
			{
				break;  //包头尚未完毕
			}
			
			YCLinkLayerPkg* pkg = (YCLinkLayerPkg*)(myInBuffer);
			if (myInSize < (pkg->length+sizeof(YCLinkLayerPkg)))
			{
				break;  //  包不完整
			}
			
			network->handlePackage(pkg->msgId, myInBuffer+sizeof(YCLinkLayerPkg), pkg->length);
			
			myInSize -= sizeof(YCLinkLayerPkg) + pkg->length;
			memmove(myInBuffer, myInBuffer+sizeof(YCLinkLayerPkg)+pkg->length, myInSize);
		}
	}

	//
	// 函数：send(SOCKET socket)
	//
	// 目的：FD_WRITE, 发送数据
	//
	void send(SOCKET socket)
	{
		const char* position = myOutBuffer;
		if (myOutSize > 0)
		{
			int result = ::send(socket, position, myOutSize, 0);
			if (result == SOCKET_ERROR)
			{
				throw YCException(2002, "YCLink::send发送数据失败");
			}

			if (result > 0)
			{
				if (result < myOutSize)
				{
					memmove(myOutBuffer, position+result, myOutSize-result);
				}
				myOutSize -= result;
			}
			else
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					throw YCException(2002, "YCLink::send发送数据失败");
				}
			}
		}
	}

	//
	// 函数：addOutPackage(const char* pkg, int len)
	//
	// 目的：如果缓冲区中有数据，必须缓存按先后顺序发送
	//
	void addOutPackage(const char* pkg, int len)
	{
		if (len + myOutSize > BUFFER_SIZE)
		{
			throw YCException(2002, "YCLink::addOutPackage发送缓冲区满");
		}

		memcpy(myOutBuffer+myOutSize, pkg, len);
		myOutSize += len;
	}

private:

	//
	// 接收缓冲
	//
	char myInBuffer[BUFFER_SIZE];

	//
	// 接收缓冲区数据长度
	//
	int myInSize;

	//
	// 发送缓冲
	//
	char myOutBuffer[BUFFER_SIZE];

	//
	// 发送缓冲区数据长度
	//
	int myOutSize;
};

///////////////////////////////////////////////////////////////
// Window Socket Initializer
///////////////////////////////////////////////////////////////
struct YCWindowSocketInitializer
{
	YCWindowSocketInitializer(BYTE minorVer = 2,BYTE majorVER = 2)
	{
		//初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer,majorVER);
		if(::WSAStartup(sockVersion,&wsaData)!=0)
		{
			SMART_ASSERT(0);
		}
	}

	~YCWindowSocketInitializer()
	{
		::WSACleanup();
	}
};

YCLink::YCLink(YCNetwork* network, HWND hWnd)
	: myNetwork(network)
	, myWnd(hWnd)
	, mySocket(-1)
{
	static YCWindowSocketInitializer __windowSocketInitializer;
}


YCLink::~YCLink(void)
{
}

//
// 函数：connect()
//
// 目的：向服务器发起连接，阻塞函数
//
bool YCLink::connect(const char* host, int port)
{
	myHost = host;
	myPort = port;

	SMART_ASSERT(mySocket == INVALID_SOCKET);

	// 创建客户端套接字  
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (mySocket == INVALID_SOCKET)
	{
		return false;
	}

	// 绑定本机端口  
	sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if (bind(mySocket, (const sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
	{
		return false;  
	}

	sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = YCIPHelper::getIPAddress(host);
	dest.sin_port = htons(port);

	if (WSAAsyncSelect(mySocket, myWnd, WM_NETWORK_MSG, FD_CONNECT) == SOCKET_ERROR)
	{
		return false;
	}

	if (::connect(mySocket, (const sockaddr*)&dest, sizeof(dest)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}

	return true;
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCLink::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NETWORK_MSG)
	{
		SOCKET socket = (SOCKET)wParam; // 发生网络事件的套接字  
		long event = WSAGETSELECTEVENT(lParam); // 事件  
		int error = WSAGETSELECTERROR(lParam); // 错误码

		try 
		{
			switch (event)
			{
			case FD_CONNECT: // 客户端连接到服务器的操作返回结果
				{
					if (socket != mySocket)
					{
						throw YCException(2002, "YCLink::onMessage未知网络端口连接消息！");
					}

					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessage连接失败");
					}

					error = WSAAsyncSelect(socket, myWnd, message, FD_READ | FD_WRITE | FD_CLOSE);
					if (error == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage连接失败");
					}

					// 成功连接到服务器，将客户端套接字置为非阻塞模式，并标明其感兴趣的事件为读、写及关闭
				}
				break;
			case FD_READ: // 收到网络包，需要读取
				{
					// 如果一个应用程序需要在一个FD_READ事件处理中调用多次recv()，那么它将会收到多个FD_READ消息，
					// 因为每次未读完缓冲区的recv()调用，都会重新触发一个FD_READ消息。
					// 针对这种情况，我们需要在读取网络包前关闭掉FD_READ消息通知，读取完这后再进行恢复，
					// 关闭FD_READ消息的方法很简单，只需要调用WSAAsyncSelect时参数lEvent中FD_READ字段不予设置即可。

					// 关闭FD_READ事件通知  
					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessages数据读取失败");
					}

					if (WSAAsyncSelect(socket, myWnd, message, FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage FD_READ 关闭FD_READ事件通知失败");
					}

					// 使用套接字读取网络包
					if (!myPackager->recv(socket))
					{
						throw YCException(2002, "YCLink::onMessage读取数据包失败");
					}

					// 再次打开FD_READ事件通知  
					if (WSAAsyncSelect(socket, myWnd, message, FD_WRITE | FD_CLOSE | FD_READ) == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage FD_READ 打开FD_READ事件通知失败");
					}

					myPackager->handleInPackage(myNetwork);
				}
				break;
			case FD_WRITE:
				{
					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessages数据写入失败");
					}

					// 1. 套接字刚建立连接时，表明准备就绪可以立即发送数据。
					// 2. 一次失败的send()调用后缓冲区再次可用时。如果系统缓冲区已经被填满，那么此时调用send()发送数据，将返回SOCKET_ERROR，
					//   使用WSAGetLastError()会得到错误码WSAEWOULDBLOCK表明被阻塞。这种情况下当缓冲区重新整理出可用空间后，会向应用程序发送FD_WRITE消息，示意其可以继续发送数据了。
					// 所以说收到FD_WRITE消息并不单纯地等同于这是使用send()的唯一时机。一般来说，如果需要发送消息，直接调用send()发送即可。
					// 如果该次调用返回值为SOCKET_ERROR且WSAGetLastError()得到错误码WSAEWOULDBLOCK，这意味着缓冲区已满暂时无法发送，此刻我们需要将待发数据保存起来，等到系统发出FD_WRITE消息后尝试重新发送。

					myPackager->send(socket);
				}
				break;
			case FD_CLOSE: // 套接字的连接方(而非本地socket)关闭消息
				{
					close();
				}
				break;
			default:
				break;
			}
		}
		catch (YCException& e)
		{
			LOG_ERROR("YCLink::onMessage消息处理失败：" << e.what());
			close();
			throw;
		}
		return true;
	}
	return false;
}

//
// 函数：get(YCIPackageRequest* request)
//
// 目的：向服务器提交请求，阻塞函数
//
//YCIPackageResponse* YCLink::get(YCIPackageRequest* request)
//{
//	throw YCException(2002, "YCLink::get同步请求不支持！");
//	return NULL;
//}

//
// 函数：post(const char* buf, int len)
//
// 目的：向服务器提交请求，异步消息
//
void YCLink::post(const char* buf, int len)
{
	if (buf == NULL && len <= 0)
	{
		throw YCException(2002, "YCLink::post提交无效数据");
	}

	int dataOutBuffer = myPackager->getDataSizeOutBuffer();
	if (dataOutBuffer == 0)
	{
		//缓冲无数据，尝试直接发送
		int result = ::send(mySocket, buf, len, 0);
		if (result == SOCKET_ERROR)
		{
			throw YCException(2002, "YCLink::post发送数据失败");
		}

		if (result > 0)
		{
			// 数据没有发送完
			if (result < len)
			{
				if ((len-result) > BUFFER_SIZE)
				{
					throw YCException(2002, "YCLink::post发送缓冲区满");
				}

				myPackager->addOutPackage(buf+result, len-result);
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				throw YCException(2002, "YCLink::post发送数据失败");
			}
			myPackager->addOutPackage(buf, len);
		}
	}
	else
	{
		//缓冲有数据，必须进入队列
		myPackager->addOutPackage(buf, len);
	}
}

//
// 函数：close()
//
// 目的：主动关闭连接，阻塞函数
//
bool YCLink::close()
{
	if (mySocket != INVALID_SOCKET)
	{
		if (closesocket(mySocket) == SOCKET_ERROR)
		{
			return false;
		}
		mySocket = INVALID_SOCKET;
	}
	return true;
}
