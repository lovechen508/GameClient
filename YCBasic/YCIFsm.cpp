#include "YCIFsm.h"

#include "YCIFsmState.h"
#include "YCIFsmTransfer.h"

#include "YCDList.h"
#include "YCException.h"

YCIFsm::YCIFsm(void)
	: myStates(new YCDList())
{
}


YCIFsm::~YCIFsm(void)
{
	myStates->clear(&YCDListClearHelper<YCIFsmState>);
	SAFE_DELETE(myStates);
}

//
// 函数：transition(unsigned int next, YCIFsmTransfer * holder)
//
// 目的：切换状态
//
void YCIFsm::transition(unsigned int next, YCIFsmTransfer * holder)
{
	//Find the named state
	YCIFsmState *nextState = NULL;
	for(Item_List_T* item = myStates->begin(); item != myStates->end(); item = myStates->next(item))
	{
		YCIFsmState* state = (YCIFsmState*)myStates->payload(item);
		if (state->nextState(next))
		{
			nextState = state;
			break;
		}
	}

	if (nextState == NULL)
	{
		throw YCException(2002, "YCIFsm::transition无效状态转换");
	}

	myCurrent->leave();
	
	if (holder != NULL)
	{
		holder->apply(nextState);
	}

	nextState->enter();
	myCurrent = nextState;
}

//
// 函数：addState(YCIFsmState* state, bool makeCurrent)
//
// 目的：增加状态
//
void YCIFsm::addState(YCIFsmState* state, bool makeCurrent)
{
	if (state == NULL)
	{
		throw YCException(2002, "YCIFsm::addState无效状态！");
	}
	
	myStates->append(state, NULL);
	
	if (makeCurrent) 
	{
		myCurrent = state;
		myCurrent->enter();
	}
}

//
// 函数：currentState()
//
// 目的：取得当前状态
//
YCIFsmState* YCIFsm::currentState()
{
	return myCurrent;
}
