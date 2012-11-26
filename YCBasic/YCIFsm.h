#ifndef _INCLUDE_YCIFSM_H_
#define _INCLUDE_YCIFSM_H_

#include "YCDef.h"

class YCDList;
class YCIFsmState;
class YCIFsmTransfer;

/* 
 * YCIFsm : 有限状态机类
 *
 * 说明：state pattern
 *
 */
class YCAPI YCIFsm
{
public:

	YCIFsm(void);

	virtual ~YCIFsm(void);

	//
	// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// 函数：transition(unsigned int next, YCIFsmTransfer * holder)
	//
	// 目的：切换状态
	//
	void transition(unsigned int next, YCIFsmTransfer * holder);

	//
	// 函数：addState(YCIFsmState* state, bool makeCurrent)
	//
	// 目的：增加状态
	//
	void addState(YCIFsmState* state, bool makeCurrent);

	//
	// 函数：currentState()
	//
	// 目的：取得当前状态
	//
	YCIFsmState* currentState();

protected:

	//
	// 属性：当前状态
	//
	YCIFsmState* myCurrent;

	//
	// 属性：状态集合
	//
	YCDList * myStates;
};

#endif

