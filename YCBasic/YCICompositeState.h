#ifndef _INCLUDE_YCICOMPOSITESTATE_H_
#define _INCLUDE_YCICOMPOSITESTATE_H_

#include "YCIFsm.h"
#include "YCIFsmState.h"

/*
 * YCICompositionState : 组合状态
 *
 * 说明：对外是大状态机的一个状态，
 *       本身也是一个状态机
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

