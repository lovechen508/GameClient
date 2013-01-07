// AppTest.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "YCClient.h"
#include "YCTest.h"

#include <string>

#include "YCBasic\YCWMTimer.h"
#include "YCBasic\YCLogger.h"
#include "YCGame\YCGame.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"

#include "YCGraphic\YCGraphic.h"
#include "YCInput\YCInput.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HWND hWnd;										// ���ھ��
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

YCInput   theInput;
YCGraphic theGraphic;
YCGame    theGame;
YCWMTimer theTimer;

int nWidth = 1024;         // ���ڿ��
int nHeight = 768;         // ���ڸ߶�

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


//
//   ���ƣ�SetClientSize(HWND, int, int)

//   ���ܣ����ÿͻ�����С��
//   ������hWnd: ��ǰ���򴰿ھ��

//         width: ���õĿ��

//         height: ���õĸ߶�

//   ����: void
//
void SetClientSize(int width, int height)
{
	RECT rectProgram, rectClient;	
	GetWindowRect(hWnd, &rectProgram);   //��ó��򴰿�λ����Ļ����
	GetClientRect(hWnd, &rectClient);      //��ÿͻ�������

	//�ǿͻ�����,��
	int nWidth = rectProgram.right - rectProgram.left -(rectClient.right - rectClient.left); 
	int nHeiht = rectProgram.bottom - rectProgram.top -(rectClient.bottom - rectClient.top); 
	nWidth += width;
	nHeiht += height;
	rectProgram.right =  nWidth;
	rectProgram.bottom =  nHeiht;
	int showToScreenx = GetSystemMetrics(SM_CXSCREEN)/2-nWidth/2;    //���д���
	int showToScreeny = GetSystemMetrics(SM_CYSCREEN)/2-nHeiht/2;
	MoveWindow(hWnd, showToScreenx, showToScreeny, rectProgram.right, rectProgram.bottom, TRUE);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
	YCTest::test();
#endif

	// TODO: �ڴ˷��ô��롣
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_APPTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APPTEST));
	
	//��ȡ��ǰ����Ŀ¼
	char path[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(path, MAX_PATH))
	{
		return FALSE;
	}

	//��ȡ�����ļ�
	std::string configFile(path);
	configFile += "\\..\\conf\\configure.xml";
	YCConfig  theConfig;
	if (!theConfig.load(configFile.c_str()))
	{
		return FALSE;
	}

	nWidth = theConfig.width();
	nHeight = theConfig.height();

	//��ʼ����Ϸ��־
	std::string logPath(path);
	logPath += "\\..\\log";
	YCLogger *logger = new YCLogger();
	if (!logger->initailize(logPath.c_str(), theConfig.logLevel()))
	{
		delete logger;
		return FALSE;
	}
	
	YCRegistry::initialize();
	YCRegistry::set("logger", logger);

	LOG_TRACE("��Ϸ��־��ʼ���ɹ���");

	if (theTimer.start())
	{
		LOG_TRACE("��Ϸ��ʱ����ʼ���ɹ���");
	}
	YCRegistry::set("timer", &theTimer);

	// ������Ϸ����
	SetClientSize(nWidth, nHeight);

	// ��ʼ����Ϸ����
	if (!theInput.initialize(hWnd, 0))
	{
		return FALSE;
	}
	YCRegistry::set("input", &theInput);
	LOG_TRACE("��Ϸ�����������ʼ���ɹ���");
	
	// ��ʼ����Ϸϵͳ
	if (theGame.initialize(hWnd, theInput, theConfig))
	{
		LOG_TRACE("��Ϸ����ϵͳ��ʼ���ɹ���");

		if (theGraphic.initialize(hWnd, theInput, theConfig))
		{
			LOG_TRACE("��Ϸͼ����ϵͳ��ʼ���ɹ���");

			// ��ͼ��ϵͳ
			theGame.bindGraphic(&theGraphic);

			// ������Ϸ����
			theGame.launchStage(theConfig);

			// ����Ϣѭ��:
			while (TRUE)
			{
				if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{				
					if (msg.message == WM_QUIT)
					{
						break;
					}

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					theGraphic.render();
				}
			}
			
			theTimer.stop();

			theGraphic.finalize();
		}

		theGame.finalize();
	}

	theInput.finalize();

	SAFE_DELETE(logger);
	YCRegistry::finalize();

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_APPTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	//if (IsWindow(hWnd))
	{
		hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_SYSMENU|WS_MINIMIZEBOX, //WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE|WS_CLIPSIBLINGS, //WS_OVERLAPPED|WS_DLGFRAME|WS_SYSMENU, 
			0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	}
	//else
	//{
	//	hWnd = CreateWindow(szWindowClass, szTitle, WS_POPUP|WS_CLIPCHILDREN, 
	//		0, 0, nWidth, nHeight, NULL, NULL, hInstance, NULL);
	//}

	if (!hWnd)
	{
		return FALSE;
	}

	theTimer.bind2HWND(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	try
	{
		if (theInput.onMessage(message, wParam, lParam))
		{
			return 0;
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("WndProc ������Ϣ�쳣��" << message << "[" << wParam << ", " << lParam << "] : " << e.what());
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SYSCOMMAND: //WM_SIZE:
		if (wParam == SC_MINIMIZE)
		{
			theGraphic.setVisible(false);
		}
		else if (wParam == SC_RESTORE)
		{
			theGraphic.setVisible(true);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		theTimer.trigger(::GetTickCount());
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

