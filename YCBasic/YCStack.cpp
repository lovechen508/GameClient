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
// ������push(void * payload)
//
// Ŀ�ģ�ѹջ
//
void YCStack::push(void * payload)
{
	SMART_ASSERT(payload != NULL);
	myList->insert(payload, NULL);
}

//
// ������front()
//
// Ŀ�ģ�����ջ��Ԫ�أ�������
//
void* YCStack::front()
{
	Item_List_T* item = myList->begin();
	return item == NULL
		 ? NULL
		 : myList->payload(item);
}

//
// ������pop()
//
// Ŀ�ģ�����������ջ��Ԫ��
//
void* YCStack::pop()
{
	if (myList->size() == 0)
	{
		return NULL;
	}

	return myList->remove(myList->begin());
}
