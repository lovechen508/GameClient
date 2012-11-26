#include "YCInput.h"

#include <list>
#include <map>

#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

#pragma region YCInputImpl

/*
 * YCInputImpl : 游戏输入内部实现类
 */
class YCInputImpl
	: public YCIInputListener
{

	typedef std::list<YCIInputListener*> LISTENER_LIST;
	typedef std::map<UINT, LISTENER_LIST> MSG_LISTENER_MAP;

public:

	YCInputImpl()
	{
	}

	~YCInputImpl()
	{
	}

	//
	// 函数：initialize(HWND hwnd, unsigned int monitorMode)
	//
	// 目的：初始化获取鼠标和键盘
	//
	bool initialize(HWND hwnd, unsigned int monitorMode)
	{
		myHWnd = hwnd;
		myMonitorMode = monitorMode;

		myLMouseButtonDown = false;
		myMMouseButtonDown = false;
		myRMouseButtonDown = false;
		myLMouseButtonDClick = false;
		myRMouseButtonDClick = false;

		return true;
	}

	//
	// 函数：finalize()
	//
	// 目的：释放鼠标和键盘
	//
	void finalize()
	{

	}

	////////////////////////////////////////
	// Keyboard 函数组
	////////////////////////////////////////
	//
	// 函数：getKeyboardState()
	//
	// 目的：释放键盘按键状态
	//
	bool getKeyboardState()
	{
		ZeroMemory(myKeyboardBuffer, 256);
		return (::GetKeyboardState(myKeyboardBuffer) ? true : false);
	}

	//
	// 函数：getMouseState(unsigned int message)
	//
	// 目的：取得鼠标状态
	//
	void getMouseState(unsigned int message)
	{
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(myHWnd, &point);
		int newX = point.x;
		int newY = point.y;

		switch (message)
		{
		case WM_LBUTTONDOWN:
			myLMouseButtonDown = true;
			break;
		case WM_LBUTTONUP:
			myLMouseButtonDown = false;
			myLMouseButtonDClick = false;
			break;
		case WM_LBUTTONDBLCLK:
			myLMouseButtonDClick = true;
			break;
		case WM_RBUTTONDOWN:
			myRMouseButtonDown = true;
			break;
		case WM_RBUTTONUP:
			myRMouseButtonDown = false;
			myRMouseButtonDClick = false;
			break;
		case WM_RBUTTONDBLCLK:
			myRMouseButtonDClick = true;
			break;
		case WM_MBUTTONDOWN:
			myMMouseButtonDown = true;
			break;
		case WM_MBUTTONUP:
			myMMouseButtonDown = false;
			break;
		case WM_MOUSEMOVE:
			{
				myMouseDeltaX = newX - myMousePositionX;
				myMouseDeltaY = newY - myMousePositionY;			

				myMousePositionX = newX;
				myMousePositionY = newY;
			}
			break;
		}
	}
	
	bool isKeyDown(BYTE key)    
	{ 
		//return ((GetAsyncKeyState(key) & 0x8000) ? true : false);			
		return ((myKeyboardBuffer[key] & 0x80) ? true : false);
	}

	////////////////////////////////////////
	// Mouse 函数组
	////////////////////////////////////////
	
	void getMousePosition(unsigned int& x, unsigned int& y)
	{
		/*POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(myHWnd, &point);
		x = point.x;
		y = point.y;*/
		
		x = myMousePositionX;
		y = myMousePositionY;
	}

	void getMouseMoveDelta(int& deltaX, int& deltaY)
	{
		deltaX = myMouseDeltaX;
		deltaY = myMouseDeltaY;
	}

	bool isLMouseButtonDown()
	{
		return myLMouseButtonDown;
	}

	bool isRMouseButtonDown()
	{
		return myRMouseButtonDown;
	}

	bool isMMouseButtonDown()
	{
		return myMMouseButtonDown;
	}

	////////////////////////////////////////
	// 消息处理函数组
	////////////////////////////////////////
	//
	// 函数：subscribeListener(YCIInputListener* listener, UINT message)
	//
	// 目的：添加消息监听器
	//
	bool subscribeListener(YCIInputListener* listener, UINT message)
	{
		SMART_ASSERT(listener != NULL);
		if (listener == NULL)
		{
			throw YCException(2002, "YCInput::subscribeListener注册listener为NULL");
		}
		
		MSG_LISTENER_MAP::iterator at = myListenerMap.find(message);
		if (at != myListenerMap.end())
		{
			const LISTENER_LIST& listeners = at->second;
			for (LISTENER_LIST::const_iterator it = listeners.begin(); 
				 it != listeners.end(); ++it)
			{
				if (*it == listener)
				{
					return true;
				}
			}
		}

		myListenerMap[message].push_back(listener);
		return true;
	}

	//
	// 函数：unsubscribeListener(YCIInputListener* listener, UINT message)
	//
	// 目的：移除消息监听器
	//
	void unsubscribeListener(YCIInputListener* listener, UINT message)
	{
		SMART_ASSERT(listener != NULL);
		if (listener == NULL)
		{
			throw YCException(2002, "YCInput::unsubscribeListener注册listener为NULL");
		}

		MSG_LISTENER_MAP::iterator at = myListenerMap.find(message);
		if (at != myListenerMap.end())
		{
			LISTENER_LIST& listeners = at->second;
			for (LISTENER_LIST::iterator it = listeners.begin(); 
				 it != listeners.end(); ++it)
			{
				if (*it == listener)
				{
					it = listeners.erase(it);
					return;
				}
			}
		}
	}

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	{
		// 如果是键盘消息，获取键盘状态
		if (message == WM_KEYDOWN || message == WM_KEYUP)
		{
			if (!getKeyboardState())
			{
				return false;
			}
		}
		
		// 获取鼠标状态
		getMouseState(message);

		bool handled = false;

		MSG_LISTENER_MAP::iterator at = myListenerMap.find(message);
		if (at != myListenerMap.end())
		{
			LISTENER_LIST& listeners = at->second;
			for (LISTENER_LIST::iterator it = listeners.begin(); 
				 it != listeners.end() && !handled; ++it)
			{
				YCIInputListener* listener = *it;
				if (listener->onMessage(message, wParam, lParam))
				{
					handled = true;
				}
			}
		}
		return handled;
	}

private:

	int myMousePositionX;
	int myMousePositionY;

	int myMouseDeltaX;

	int myMouseDeltaY;

	bool myLMouseButtonDown;
	bool myMMouseButtonDown;
	bool myRMouseButtonDown;
	bool myLMouseButtonDClick;
	bool myRMouseButtonDClick;

	HWND myHWnd;

	MSG_LISTENER_MAP myListenerMap;
		
	unsigned int myMonitorMode;

	BYTE myKeyboardBuffer[256];
};

#pragma endregion YCInputImpl

YCInput::YCInput(void)
	: myInput(NULL)
{
}


YCInput::~YCInput(void)
{
	SAFE_DELETE(myInput);
}

//
// 函数：initialize(HWND hwnd, unsigned int monitorMode)
//
// 目的：初始化获取鼠标和键盘
//
bool YCInput::initialize(HWND hwnd, unsigned int monitorMode)
{
	myInput = new YCInputImpl();
	return myInput->initialize(hwnd, monitorMode);
}

//
// 函数：finalize()
//
// 目的：释放鼠标和键盘
//
void YCInput::finalize()
{
	if (myInput != NULL)
	{
		myInput->finalize();
		SAFE_DELETE(myInput);
	}
}

////////////////////////////////////////
// Keyboard 函数组
////////////////////////////////////////
bool YCInput::isKeyDown(BYTE key)  { return myInput->isKeyDown(key);        }

bool YCInput::isAltKeyDown()       { return myInput->isKeyDown(VK_MENU);    }
bool YCInput::isCtrlKeyDown()      { return myInput->isKeyDown(VK_CONTROL); }
bool YCInput::isShiftKeyDown()     { return myInput->isKeyDown(VK_SHIFT);   }
bool YCInput::isCapsKeyDown()      { return myInput->isKeyDown(VK_CAPITAL); }
bool YCInput::isEscKeyDown()       { return myInput->isKeyDown(VK_ESCAPE);  }
bool YCInput::isLButtonDown()      { return myInput->isKeyDown(VK_LBUTTON); }
bool YCInput::isRButtonDown()      { return myInput->isKeyDown(VK_RBUTTON); }
bool YCInput::isMButtonDown()      { return myInput->isKeyDown(VK_MBUTTON); }

////////////////////////////////////////
// Mouse 函数组
////////////////////////////////////////
void YCInput::getMousePosition(unsigned int& x, unsigned int& y)
{
	myInput->getMousePosition(x, y);
}

void YCInput::getMouseMoveDelta(int& deltaX, int& deltaY)
{
	myInput->getMouseMoveDelta(deltaX, deltaY);
}

bool YCInput::isLMouseButtonDown()
{
	return myInput->isLMouseButtonDown();
}

bool YCInput::isRMouseButtonDown()
{
	return myInput->isRMouseButtonDown();
}

bool YCInput::isMMouseButtonDown()
{
	return myInput->isMMouseButtonDown();
}

//
// 函数：subscribeListener(YCIInputListener* listener, UINT message)
//
// 目的：添加消息监听器
//
bool YCInput::subscribeListener(YCIInputListener* listener, UINT message)
{
	return myInput->subscribeListener(listener, message);
}

//
// 函数：unsubscribeListener(YCIInputListener* listener, UINT message)
//
// 目的：移除消息监听器
//
void YCInput::unsubscribeListener(YCIInputListener* listener, UINT message)
{
	myInput->unsubscribeListener(listener, message);
}

//
// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：窗口消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCInput::onMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (myInput != NULL) 
	{
		return myInput->onMessage(message, wParam, lParam);
	}
	return false;
}
