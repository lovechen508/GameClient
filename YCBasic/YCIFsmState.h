#ifndef _INCLUDE_YCIFSMSTATE_H_
#define _INCLUDE_YCIFSMSTATE_H_

#include "YCDef.h"

class YCIFsm;

/*
 * YCIFsmState : ״̬��
 */
class YCAPI YCIFsmState
{
public:

	YCIFsmState(YCIFsm* fsm);

	virtual ~YCIFsmState(void);

	//
	// ������nextState(unsigned int)
	//
	// Ŀ�ģ��Ƿ���һ״̬
	//
	virtual bool nextState(unsigned int) = 0;

	//
	// ������with(unsigned int id, T*)
	//
	// Ŀ�ģ������ڲ��������ݣ�YCIFsmTransfer����
	//      ��ʵ�֣�����ʵ��
	//
	virtual YCIFsmState* with(unsigned int id, void*);
	virtual YCIFsmState* with(unsigned int id, int);
	virtual YCIFsmState* with(unsigned int id, unsigned int);
	virtual YCIFsmState* with(unsigned int id, const char*);

	//
	// ������enter()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void enter() = 0;

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam) = 0;

	//
	// ������leave()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void leave() = 0;

private:

	//
	// ���ԣ�����״̬��
	//
	YCIFsm* myFsm;

};

#endif

