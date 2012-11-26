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
// ������transition(unsigned int next, YCIFsmTransfer * holder)
//
// Ŀ�ģ��л�״̬
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
		throw YCException(2002, "YCIFsm::transition��Ч״̬ת��");
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
// ������addState(YCIFsmState* state, bool makeCurrent)
//
// Ŀ�ģ�����״̬
//
void YCIFsm::addState(YCIFsmState* state, bool makeCurrent)
{
	if (state == NULL)
	{
		throw YCException(2002, "YCIFsm::addState��Ч״̬��");
	}
	
	myStates->append(state, NULL);
	
	if (makeCurrent) 
	{
		myCurrent = state;
		myCurrent->enter();
	}
}

//
// ������currentState()
//
// Ŀ�ģ�ȡ�õ�ǰ״̬
//
YCIFsmState* YCIFsm::currentState()
{
	return myCurrent;
}
