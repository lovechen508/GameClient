#ifndef _INCLUDE_YCIFSMSTATE_H_
#define _INCLUDE_YCIFSMSTATE_H_

#include "YCDef.h"

class YCIFsm;

/*
 * YCIFsmState : 状态类
 */
class YCAPI YCIFsmState
{
public:

	YCIFsmState(YCIFsm* fsm);

	virtual ~YCIFsmState(void);

	//
	// 函数：nextState(unsigned int)
	//
	// 目的：是否下一状态
	//
	virtual bool nextState(unsigned int) = 0;

	//
	// 函数：with(unsigned int id, T*)
	//
	// 目的：设置内部必须数据，YCIFsmTransfer调用
	//      不实现，子类实现
	//
	virtual YCIFsmState* with(unsigned int id, void*);
	virtual YCIFsmState* with(unsigned int id, int);
	virtual YCIFsmState* with(unsigned int id, unsigned int);
	virtual YCIFsmState* with(unsigned int id, const char*);

	//
	// 函数：enter()
	//
	// 目的：状态进入
	//
	virtual void enter() = 0;

	//
	// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// 目的：消息处理函数
	//
	// 返回值：已处理，返回true，否则返回false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// 函数：leave()
	//
	// 目的：状态结束
	//
	virtual void leave() = 0;

private:

	//
	// 属性：所属状态机
	//
	YCIFsm* myFsm;

};

#endif

