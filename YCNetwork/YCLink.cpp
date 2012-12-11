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
//��·�����ݰ�ͷ������
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
// ���ν��ջ���������
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
	// ������getDataSizeInBuffer()
	//       getDataSizeOutBuffer()
	//
	// Ŀ�ģ�
	//       �õ����������������ݳ���
	//
	int getDataSizeInBuffer()   { return myInSize;  }
	int getDataSizeOutBuffer()  { return myOutSize; }

	//
	// ������recv(SOCKET)
	//
	// Ŀ�ģ�FD_RECV�¼���׷�����ݵ����ջ���
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
				LOG_ERROR("YCLink::recv ����" << WSAGetLastError());
				return false;
			}
			else if (bytes == 0)
			{
				LOG_ERROR("YCLink::recv���ӱ��Է��ر�");
				return false;
			}
			else
			{
				if (bytes+myInSize > BUFFER_SIZE)
				{
					throw YCException(2002, "YCLink::recv���ջ�������");
				}
				memcpy(myInBuffer+myInSize, tmp, bytes);
				myInSize += bytes;
			}
		}
		return true;
	}

	//
	// ������handleInPackage(YCNetwork* network)
	//
	// Ŀ�ģ��������ݰ����ص�YCNetwork
	//
	void handleInPackage(YCNetwork* network)
	{
		//network->handlePackage(unsigned int msgId, const char* buf, int len);
		SMART_ASSERT(network != NULL);
		if (network == NULL)
		{
			throw YCException(2002, "YCLink::handleInPackage ����ָ��Ϊ��");
		}
		
		while (true)
		{
			if (myInSize < sizeof(YCLinkLayerPkg))
			{
				break;  //��ͷ��δ���
			}
			
			YCLinkLayerPkg* pkg = (YCLinkLayerPkg*)(myInBuffer);
			if (myInSize < (pkg->length+sizeof(YCLinkLayerPkg)))
			{
				break;  //  ��������
			}
			
			network->handlePackage(pkg->msgId, myInBuffer+sizeof(YCLinkLayerPkg), pkg->length);
			
			myInSize -= sizeof(YCLinkLayerPkg) + pkg->length;
			memmove(myInBuffer, myInBuffer+sizeof(YCLinkLayerPkg)+pkg->length, myInSize);
		}
	}

	//
	// ������send(SOCKET socket)
	//
	// Ŀ�ģ�FD_WRITE, ��������
	//
	void send(SOCKET socket)
	{
		const char* position = myOutBuffer;
		if (myOutSize > 0)
		{
			int result = ::send(socket, position, myOutSize, 0);
			if (result == SOCKET_ERROR)
			{
				throw YCException(2002, "YCLink::send��������ʧ��");
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
					throw YCException(2002, "YCLink::send��������ʧ��");
				}
			}
		}
	}

	//
	// ������addOutPackage(const char* pkg, int len)
	//
	// Ŀ�ģ�����������������ݣ����뻺�水�Ⱥ�˳����
	//
	void addOutPackage(const char* pkg, int len)
	{
		if (len + myOutSize > BUFFER_SIZE)
		{
			throw YCException(2002, "YCLink::addOutPackage���ͻ�������");
		}

		memcpy(myOutBuffer+myOutSize, pkg, len);
		myOutSize += len;
	}

private:

	//
	// ���ջ���
	//
	char myInBuffer[BUFFER_SIZE];

	//
	// ���ջ��������ݳ���
	//
	int myInSize;

	//
	// ���ͻ���
	//
	char myOutBuffer[BUFFER_SIZE];

	//
	// ���ͻ��������ݳ���
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
		//��ʼ��WS2_32.dll
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
// ������connect()
//
// Ŀ�ģ���������������ӣ���������
//
bool YCLink::connect(const char* host, int port)
{
	myHost = host;
	myPort = port;

	SMART_ASSERT(mySocket == INVALID_SOCKET);

	// �����ͻ����׽���  
	mySocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (mySocket == INVALID_SOCKET)
	{
		return false;
	}

	// �󶨱����˿�  
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
// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ�������Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCLink::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NETWORK_MSG)
	{
		SOCKET socket = (SOCKET)wParam; // ���������¼����׽���  
		long event = WSAGETSELECTEVENT(lParam); // �¼�  
		int error = WSAGETSELECTERROR(lParam); // ������

		try 
		{
			switch (event)
			{
			case FD_CONNECT: // �ͻ������ӵ��������Ĳ������ؽ��
				{
					if (socket != mySocket)
					{
						throw YCException(2002, "YCLink::onMessageδ֪����˿�������Ϣ��");
					}

					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessage����ʧ��");
					}

					error = WSAAsyncSelect(socket, myWnd, message, FD_READ | FD_WRITE | FD_CLOSE);
					if (error == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage����ʧ��");
					}

					// �ɹ����ӵ������������ͻ����׽�����Ϊ������ģʽ�������������Ȥ���¼�Ϊ����д���ر�
				}
				break;
			case FD_READ: // �յ����������Ҫ��ȡ
				{
					// ���һ��Ӧ�ó�����Ҫ��һ��FD_READ�¼������е��ö��recv()����ô�������յ����FD_READ��Ϣ��
					// ��Ϊÿ��δ���껺������recv()���ã��������´���һ��FD_READ��Ϣ��
					// ������������������Ҫ�ڶ�ȡ�����ǰ�رյ�FD_READ��Ϣ֪ͨ����ȡ������ٽ��лָ���
					// �ر�FD_READ��Ϣ�ķ����ܼ򵥣�ֻ��Ҫ����WSAAsyncSelectʱ����lEvent��FD_READ�ֶβ������ü��ɡ�

					// �ر�FD_READ�¼�֪ͨ  
					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessages���ݶ�ȡʧ��");
					}

					if (WSAAsyncSelect(socket, myWnd, message, FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage FD_READ �ر�FD_READ�¼�֪ͨʧ��");
					}

					// ʹ���׽��ֶ�ȡ�����
					if (!myPackager->recv(socket))
					{
						throw YCException(2002, "YCLink::onMessage��ȡ���ݰ�ʧ��");
					}

					// �ٴδ�FD_READ�¼�֪ͨ  
					if (WSAAsyncSelect(socket, myWnd, message, FD_WRITE | FD_CLOSE | FD_READ) == SOCKET_ERROR)
					{
						throw YCException(2002, "YCLink::onMessage FD_READ ��FD_READ�¼�֪ͨʧ��");
					}

					myPackager->handleInPackage(myNetwork);
				}
				break;
			case FD_WRITE:
				{
					if (0 != error)
					{
						throw YCException(2002, "YCLink::onMessages����д��ʧ��");
					}

					// 1. �׽��ָս�������ʱ������׼���������������������ݡ�
					// 2. һ��ʧ�ܵ�send()���ú󻺳����ٴο���ʱ�����ϵͳ�������Ѿ�����������ô��ʱ����send()�������ݣ�������SOCKET_ERROR��
					//   ʹ��WSAGetLastError()��õ�������WSAEWOULDBLOCK��������������������µ�������������������ÿռ�󣬻���Ӧ�ó�����FD_WRITE��Ϣ��ʾ������Լ������������ˡ�
					// ����˵�յ�FD_WRITE��Ϣ���������ص�ͬ������ʹ��send()��Ψһʱ����һ����˵�������Ҫ������Ϣ��ֱ�ӵ���send()���ͼ��ɡ�
					// ����ôε��÷���ֵΪSOCKET_ERROR��WSAGetLastError()�õ�������WSAEWOULDBLOCK������ζ�Ż�����������ʱ�޷����ͣ��˿�������Ҫ���������ݱ����������ȵ�ϵͳ����FD_WRITE��Ϣ�������·��͡�

					myPackager->send(socket);
				}
				break;
			case FD_CLOSE: // �׽��ֵ����ӷ�(���Ǳ���socket)�ر���Ϣ
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
			LOG_ERROR("YCLink::onMessage��Ϣ����ʧ�ܣ�" << e.what());
			close();
			throw;
		}
		return true;
	}
	return false;
}

//
// ������get(YCIPackageRequest* request)
//
// Ŀ�ģ���������ύ������������
//
//YCIPackageResponse* YCLink::get(YCIPackageRequest* request)
//{
//	throw YCException(2002, "YCLink::getͬ������֧�֣�");
//	return NULL;
//}

//
// ������post(const char* buf, int len)
//
// Ŀ�ģ���������ύ�����첽��Ϣ
//
void YCLink::post(const char* buf, int len)
{
	if (buf == NULL && len <= 0)
	{
		throw YCException(2002, "YCLink::post�ύ��Ч����");
	}

	int dataOutBuffer = myPackager->getDataSizeOutBuffer();
	if (dataOutBuffer == 0)
	{
		//���������ݣ�����ֱ�ӷ���
		int result = ::send(mySocket, buf, len, 0);
		if (result == SOCKET_ERROR)
		{
			throw YCException(2002, "YCLink::post��������ʧ��");
		}

		if (result > 0)
		{
			// ����û�з�����
			if (result < len)
			{
				if ((len-result) > BUFFER_SIZE)
				{
					throw YCException(2002, "YCLink::post���ͻ�������");
				}

				myPackager->addOutPackage(buf+result, len-result);
			}
		}
		else
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				throw YCException(2002, "YCLink::post��������ʧ��");
			}
			myPackager->addOutPackage(buf, len);
		}
	}
	else
	{
		//���������ݣ�����������
		myPackager->addOutPackage(buf, len);
	}
}

//
// ������close()
//
// Ŀ�ģ������ر����ӣ���������
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
