#include "YCStack.h"

#include "YCDef.h"
#include "YCDList.h"
#include "YCAssert.h"

YCStack::YCStack(void)
	: myList(new YCDList())
{
}


YCStack::~YCStack(void)
{
	SAFE_DELETE(myList);
}

//
// 函数：push(void * payload)
//
// 目的：压栈
//
void YCStack::push(void * payload)
{
	SMART_ASSERT(payload != NULL);
	myList->insert(payload, NULL);
}

//
// 函数：front()
//
// 目的：返回栈定元素，不弹出
//
void* YCStack::front()
{
	Item_List_T* item = myList->begin();
	return item == NULL
		 ? NULL
		 : myList->payload(item);
}

//
// 函数：pop()
//
// 目的：弹出并返回栈顶元素
//
void* YCStack::pop()
{
	if (myList->size() == 0)
	{
		return NULL;
	}

	return myList->remove(myList->begin());
}
