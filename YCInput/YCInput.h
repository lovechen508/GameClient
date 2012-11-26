#ifndef _INCLUDE_YCINPUT_H_
#define _INCLUDE_YCINPUT_H_

#include "YCBasic\YCDef.h"

#include "YCIInputListener.h"

class YCInputImpl;

/*
 * YCInput : ���̣������Ϣ��װ�࣬���ɼ̳�
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
	// ������initialize(HWND hwnd, unsigned int monitorMode)
	//
	// Ŀ�ģ���ʼ����ȡ���ͼ���
	//
	bool initialize(HWND hwnd, unsigned int monitorMode);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ͷ����ͼ���
	//
	void finalize();

	////////////////////////////////////////
	// Keyboard ������ ����ɨ����
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
	// Mouse ������
	////////////////////////////////////////

	//
	// ������getMousePosition(unsigned int& x, unsigned int& y)
	//
	// Ŀ�ģ�ȡ�����λ�ã��Ѿ��任��ScreenToClient
	//
	void getMousePosition(unsigned int& x, unsigned int& y);

	void getMouseMoveDelta(int& deltaX, int& deltaY);

	bool isLMouseButtonDown();
	bool isRMouseButtonDown();
	bool isMMouseButtonDown();

public:

	//
	// ������subscribeListener(YCIInputListener* listener, UINT message)
	//
	// Ŀ�ģ������Ϣ������
	//
	bool subscribeListener(YCIInputListener* listener, UINT message);

	//
	// ������unsubscribeListener(YCIInputListener* listener, UINT message)
	//
	// Ŀ�ģ��Ƴ���Ϣ������
	//
	void unsubscribeListener(YCIInputListener* listener, UINT message);

	//
	// ������bool onMessage(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ�������Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool onMessage(UINT message, WPARAM wParam, LPARAM lParam);

private:

	YCInputImpl * myInput;
};

#endif

