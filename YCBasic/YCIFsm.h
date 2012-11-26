#ifndef _INCLUDE_YCIFSM_H_
#define _INCLUDE_YCIFSM_H_

#include "YCDef.h"

class YCDList;
class YCIFsmState;
class YCIFsmTransfer;

/* 
 * YCIFsm : ����״̬����
 *
 * ˵����state pattern
 *
 */
class YCAPI YCIFsm
{
public:

	YCIFsm(void);

	virtual ~YCIFsm(void);

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// ������transition(unsigned int next, YCIFsmTransfer * holder)
	//
	// Ŀ�ģ��л�״̬
	//
	void transition(unsigned int next, YCIFsmTransfer * holder);

	//
	// ������addState(YCIFsmState* state, bool makeCurrent)
	//
	// Ŀ�ģ�����״̬
	//
	void addState(YCIFsmState* state, bool makeCurrent);

	//
	// ������currentState()
	//
	// Ŀ�ģ�ȡ�õ�ǰ״̬
	//
	YCIFsmState* currentState();

protected:

	//
	// ���ԣ���ǰ״̬
	//
	YCIFsmState* myCurrent;

	//
	// ���ԣ�״̬����
	//
	YCDList * myStates;
};

#endif

