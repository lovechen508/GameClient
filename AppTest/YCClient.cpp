// AppTest.cpp : 定义应用程序的入口点。
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

// 全局变量:
HWND hWnd;										// 窗口句柄
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

YCInput   theInput;
YCGraphic theGraphic;
YCGame    theGame;
YCWMTimer theTimer;

int nWidth = 1024;         // 窗口宽度
int nHeight = 768;         // 窗口高度

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


//
//   名称：SetClientSize(HWND, int, int)

//   功能：设置客户区大小。
//   参数：hWnd: 当前程序窗口句柄

//         width: 设置的宽度

//         height: 设置的高度

//   返回: void
//
void SetClientSize(int width, int height)
{
	RECT rectProgram, rectClient;	
	GetWindowRect(hWnd, &rectProgram);   //获得程序窗口位于屏幕坐标
	GetClientRect(hWnd, &rectClient);      //获得客户区坐标

	//非客户区宽,高
	int nWidth = rectProgram.right - rectProgram.left -(rectClient.right - rectClient.left); 
	int nHeiht = rectProgram.bottom - rectProgram.top -(rectClient.bottom - rectClient.top); 
	nWidth += width;
	nHeiht += height;
	rectProgram.right =  nWidth;
	rectProgram.bottom =  nHeiht;
	int showToScreenx = GetSystemMetrics(SM_CXSCREEN)/2-nWidth/2;    //居中处理
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

	// TODO: 在此放置代码。
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_APPTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APPTEST));
	
	//获取当前工作目录
	char path[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(path, MAX_PATH))
	{
		return FALSE;
	}

	//读取配置文件
	std::string configFile(path);
	configFile += "\\..\\conf\\configure.xml";
	YCConfig  theConfig;
	if (!theConfig.load(configFile.c_str()))
	{
		return FALSE;
	}

	nWidth = theConfig.width();
	nHeight = theConfig.height();

	//初始化游戏日志
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

	LOG_TRACE("游戏日志初始化成功！");

	if (theTimer.start())
	{
		LOG_TRACE("游戏定时器初始化成功！");
	}
	YCRegistry::set("timer", &theTimer);

	// 调整游戏窗口
	SetClientSize(nWidth, nHeight);

	// 初始化游戏输入
	if (!theInput.initialize(hWnd, 0))
	{
		return FALSE;
	}
	YCRegistry::set("input", &theInput);
	LOG_TRACE("游戏输入控制器初始化成功！");
	
	// 初始化游戏系统
	if (theGame.initialize(hWnd, theInput, theConfig))
	{
		LOG_TRACE("游戏核心系统初始化成功！");

		if (theGraphic.initialize(hWnd, theInput, theConfig))
		{
			LOG_TRACE("游戏图形子系统初始化成功！");

			// 绑定图形系统
			theGame.bindGraphic(&theGraphic);

			// 创建游戏场景
			theGame.launchStage(theConfig);

			// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		LOG_WARNING("WndProc 处理消息异常：" << message << "[" << wParam << ", " << lParam << "] : " << e.what());
	}

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// 分析菜单选择:
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
		// TODO: 在此添加任意绘图代码...
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

