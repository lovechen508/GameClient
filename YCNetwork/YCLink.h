#ifndef _INCLUDE_YCLINK_H_
#define _INCLUDE_YCLINK_H_

#include <string>
#include <winSock2.h>

#include "YCBasic\YCDef.h"
#include "YCInput\YCIInputListener.h"

//
// ��·���������
//
class YCLinkPackager;

class YCNetwork;

#define WM_NETWORK_MSG WM_USER+100

/*
 * YCLink : TCP/IP ���ӳ���
 *
 *          ����DLL�ӿ��б�¶
 */
class YCLink : public YCIInputListener
{
public:
	
	YCLink(YCNetwork* network, HWND hWnd);
	
	virtual ~YCLink(void);

	//
	// ������connect(const char* host, int port)
	//
	// Ŀ�ģ���������������ӣ���������
	//
	bool connect(const char* host, int port);

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// ������get(YCIPackageRequest* request)
	//
	// Ŀ�ģ���������ύ������������
	//
	//YCIPackageResponse* get(YCIPackageRequest* request);

	//
	// ������post(const char* buf, int len)
	//
	// Ŀ�ģ���������ύ�����첽��Ϣ
	//
	void post(const char* buf, int len);

	//
	// ������close()
	//
	// Ŀ�ģ������ر����ӣ���������
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

