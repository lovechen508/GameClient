#include "YCDList.h"

#include "YCAssert.h"

#include "YCException.h"

//�ڵ㶨��
struct Item_List_T{
	void *payload;
	Item_List_T *next;
	Item_List_T *prev;
};

YCDList::YCDList()
	: myHead(new Item_List_T()), myTail(NULL), length(0)
{
	memset(myHead, 0, sizeof(Item_List_T));
}

YCDList::~YCDList()
{
	SMART_ASSERT(length == 0);
	delete myHead;
}

//
// ������size()
//
// Ŀ�ģ����ص�������ĳ���
//
// ����ֵ��>=0
//
int YCDList::size() const
{
	return length;
}

//
// ������find(void* payload)
//
// Ŀ�ģ�����ĳpayload�Ƿ���������
//
// ����ֵ��>=0
//
Item_List_T* YCDList::find(void* payload)
{
	for (Item_List_T* it = begin(); it != end(); it = next(it))
	{
		if (payload == it->payload)
		{
			return it;
		}
	}
	return NULL;
}

//
// ������sort(bool(*compare)(const void *left, const void *right))
//
// Ŀ�ģ� ����compare�ȽϺ�������Խ��Խ��ǰ
//
void sort(bool(*compare)(const void *left, const void *right))
{

}

//
// ������value(Item_List_T* item)
//
// Ŀ�ģ����ص�ǰ�ڵ�Я��������
//
// ����ֵ��item��payload
//
void* YCDList::payload(Item_List_T* item)
{
	SMART_ASSERT(item != NULL);
	if (item != NULL)
	{
		return item->payload;
	}
	return NULL;
}

const void* YCDList::payload(const Item_List_T* item) const
{
	SMART_ASSERT(item != NULL);
	if (item != NULL)
	{
		return item->payload;
	}
	return NULL;
}

//
// ������insert(void* payload, Item_List_T* position)
//
// Ŀ�ģ���position�ڵ�ǰ����ڵ�item
//       ���position==NULL������ʧ��
//
// ����ֵ������ɹ�����True�����򷵻�False
//
bool YCDList::insert(void* payload, Item_List_T* position)
{
	SMART_ASSERT(payload != NULL); 

	if (payload != NULL)
	{
		Item_List_T* item = new Item_List_T();
		memset(item, 0, sizeof(Item_List_T));
		item->payload = payload;

		if (position != NULL)
		{
			item->prev = position->prev;
			position->prev->next = item;
			item->next = position;
			position->prev = item;
		}
		else
		{
			item->prev = myHead;
			if (myHead->next != NULL)
			{
				myHead->next->prev = item;
				item->next = myHead->next;
			}
			else
			{
				myTail = item; //��һ���ڵ�
			}
			myHead->next = item;			
		}

		length += 1;
		return true;
	}

	return false;
}

//
// ������append(void* payload, Item_List_T* position)
//
// Ŀ�ģ���position�ڵ�����ڵ�item
//       ���position==NULL������ʧ��
//
// ����ֵ������ɹ�����True�����򷵻�False
//
bool YCDList::append(void* payload, Item_List_T* position)
{
	SMART_ASSERT(payload != NULL); 

	if (payload != NULL)
	{
		Item_List_T* item = new Item_List_T();
		memset(item, 0, sizeof(Item_List_T));
		item->payload = payload;

		if (position != NULL)
		{
			item->prev = position;
			if (position->next != NULL)
			{
				position->next->prev = item;
			}
			item->next = position->next;
			position->next = item;		
		}
		else //β������
		{
			if (myTail == NULL) //��һ���ڵ�
			{
				item->prev = myHead;
				myHead->next = item;
			}
			else
			{
				myTail->next = item;
				item->prev = myTail;
			}
			myTail = item;
		}
			
		length += 1;
		return true;
	}
	return false;
}

//
// ������remove(Item_List_T* item)
//
// Ŀ�ģ�����ǰ�ڵ�������Ƴ�
//
// ����ֵ�����item == NULL, ����NULL
//         ���item != NULL, �Ƴ������ظýڵ�
//         ���item != NULL, ����û���ҵ����׳��쳣
//
void* YCDList::remove(Item_List_T* item)
{
	SMART_ASSERT(item != NULL);
	void* payload = NULL;

	if (item != NULL)
	{
		bool found = false;
		for (Item_List_T* it = begin(); it != end(); it = next(it))
		{
			if (it == item)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			throw YCException(2002, "YCDList��ɾ���ڵ�δ���֣�");
		}

		item->prev->next = item->next;
		if (item->next)
		{
			item->next->prev = item->prev;
		}
		payload = item->payload;
		if (item == myTail)
		{
			myTail = NULL;
		}
		delete item;
		length -= 1;
	}
	return payload;
}

//
// ������clear()
//
// Ŀ�ģ���յ�ǰ����Delete����item
//       ���ڱ����payload����void*ָ�룬����delete����
//       ���õ���Ӧ�������������ڵ�����������ǰ�������clear
//       ��������ͷ��Ȼ����
//
// ע�ͣ�
//       ���release == NULL, �������������payload���������������ڣ������ͷ�
//       ���release != NULL, ���������е�ÿһ�����release(payload)���ͷ���Դ
void YCDList::clear(void(*release)(void*))
{
	Item_List_T* it = myHead->next;
	while (it != NULL)
	{
		Item_List_T* next = it->next;
		if (release != NULL)
		{
			release(it->payload);
		}
		delete it;
		it = next;
	}

	length = 0;
	myHead->next = NULL;
	myTail = NULL;
}

//
// ������begin()
//
// Ŀ�ģ������������myHeadָ���ͷ�ڵ�
//
// ����ֵ��size==0������NULL
//
Item_List_T* YCDList::begin()
{
	return myHead->next;
}

const Item_List_T* YCDList::begin() const
{
	return myHead->next;
}

//
// ������prev()
//
// Ŀ�ģ��������������current��ǰһ���ڵ�
//
Item_List_T* YCDList::prev(Item_List_T* current)
{
	SMART_ASSERT(current != NULL);
	if (current != NULL)
	{
		return current->prev;
	}
	return NULL;
}

const Item_List_T* YCDList::prev(Item_List_T* current) const
{
	SMART_ASSERT(current != NULL);
	if (current != NULL)
	{
		return current->prev;
	}
	return NULL;
}

//
// ������next()
//
// Ŀ�ģ��������������current����һ���ڵ�
//
// ����ֵ��size==0������NULL
//
Item_List_T* YCDList::next(Item_List_T* current)
{
	SMART_ASSERT(current != NULL);
	if (current != NULL)
	{
		return current->next;
	}
	return NULL;
}

const Item_List_T* YCDList::next(const Item_List_T* current) const
{
	SMART_ASSERT(current != NULL);
	if (current != NULL)
	{
		return current->next;
	}
	return NULL;
}

//
// ������end()
//
// Ŀ�ģ������������myHeadָ���β�ڵ�
//
// ����ֵ��ʼ��ΪNULL
//
Item_List_T* YCDList::end()
{
	return NULL;
}

const Item_List_T* YCDList::end() const 
{
	return NULL;
}

//
// ������r_begin()
//
// Ŀ�ģ������������myHeadָ���β�ڵ�
//
// ����ֵ��size==0������myHead
//
Item_List_T* YCDList::r_begin()
{
	return myTail; //(myTail != NULL) ? myTail : myHead;
}

//
// ������next()
//
// Ŀ�ģ��������������current��ǰһ���ڵ�
//
// ����ֵ��size==0������myHead
//
Item_List_T* YCDList::r_next(Item_List_T* current)
{
	SMART_ASSERT(current != NULL);
	if (current != NULL)
	{
		return (current->prev == myHead ? NULL : current->prev);
	}
	return NULL;
}

//
// ������end()
//
// Ŀ�ģ������������myHeadָ���ͷ�ڵ�
//
// ����ֵ��ʼ��ΪmyHead
//
Item_List_T* YCDList::r_end()
{
	return NULL; //myHead
}