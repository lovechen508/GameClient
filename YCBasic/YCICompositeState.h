#ifndef _INCLUDE_YCICOMPOSITESTATE_H_
#define _INCLUDE_YCICOMPOSITESTATE_H_

#include "YCIFsm.h"
#include "YCIFsmState.h"

/*
 * YCICompositionState : ���״̬
 *
 * ˵���������Ǵ�״̬����һ��״̬��
 *       ����Ҳ��һ��״̬��
 */
class YCAPI YCICompositeState 
	: public YCIFsmState
	, public YCIFsm
{
public:

	YCICompositeState(YCIFsm *fsm);

	virtual ~YCICompositeState(void);
};

#endif

