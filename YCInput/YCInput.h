#ifndef _INCLUDE_YCINPUT_H_
#define _INCLUDE_YCINPUT_H_

#include "YCBasic\YCDef.h"

#include "YCIInputListener.h"

class YCInputImpl;

/*
 * YCInput : 键盘，鼠标消息封装类，不可继承
 */
class YCAPI YCInput
	: public YCIInputListener
{
public:

	enum INPUT_MSG_ENABLE_MODE {
		INPUT_MSG_CHAR          = 0x00000001,
		INPUT_MSG_KEYDOWN       = 0x00000002,
		INPUT_MSG_KEYUP         = 0x00000004,
		INPUT_MSG_MOUSEMOVE     = 0x00000008,
		INPUT_MSG_MOUSELDOWN    = 0x00000010,
		INPUT_MSG_MOUSERDOWN    = 0x00000020,
		INPUT_MSG_MOUSEWHEEL    = 0x00000040,
		INPUT_MSG_MOUSEDCLICK   = 0x00000080,
	};

public:
		
	YCInput(void);

	~YCInput(void);

	//
	// 函数：initialize(HWND hwnd, unsigned int monitorMode)
	//
	// 目的：初始化获取鼠标和键盘
	//
	bool initialize(HWND hwnd, unsigned int monitorMode);

	//
	// 函数：finalize()
	//
	// 目的：释放鼠标和键盘
	//
	void finalize();

	////////////////////////////////////////
	// Keyboard 函数组 键盘扫描码
	////////////////////////////////////////
	bool isKeyDown(BYTE key);  
	bool isAltKeyDown();
	bool isCtrlKeyDown();
	bool isShiftKeyDown();
	bool isCapsKeyDown();
	bool isEscKeyDown();
	bool isLButtonDown();
	bool isRButtonDown();
	bool isMButtonDown();

	////////////////////////////////////////
	// Mouse 函数组
	////////////////////////////////////////

	//
	// 函数：getMousePosition(unsigned int& x, unsigned int& y)
	//
	// 目的：取得鼠标位置，已经变换到ScreenToClient
	//
	void getMousePosition(unsigned int& x, unsigned int& y);

	void getMouseMoveDelta(int& deltaX, int& deltaY);

	bool isLMouseButtonDown();
	bool isRMouseButtonDown();
	bool isMMouseButtonDown();

public:

	//
	// 函数：subscribeListener(YCIInputListener* listener, UINT message)
	//
	// 目的：添加消息监听器
	//
	bool subscribeListener(YCIInputListener* listener, UINT message);

	//
	// 函数：unsubscribeListener(YCIInputListener* listener, UINT message)
	//
	// 目的：移除消息监听器
	//
	void unsubscribeListener(YCIInputListener* listener, UINT message);

	//
	// 函数：bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：窗口消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	YCInputImpl * myInput;
};

#endif

