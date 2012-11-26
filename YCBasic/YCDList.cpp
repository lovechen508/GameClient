#include "YCDList.h"

#include "YCAssert.h"

#include "YCException.h"

//节点定义
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
// 函数：size()
//
// 目的：返回当天链表的长度
//
// 返回值：>=0
//
int YCDList::size() const
{
	return length;
}

//
// 函数：find(void* payload)
//
// 目的：查找某payload是否在链表中
//
// 返回值：>=0
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
// 函数：sort(bool(*compare)(const void *left, const void *right))
//
// 目的： 根据compare比较函数排序，越大越靠前
//
void sort(bool(*compare)(const void *left, const void *right))
{

}

//
// 函数：value(Item_List_T* item)
//
// 目的：返回当前节点携带的内容
//
// 返回值：item的payload
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
// 函数：insert(void* payload, Item_List_T* position)
//
// 目的：在position节点前插入节点item
//       如果position==NULL，插入失败
//
// 返回值：插入成功返回True，否则返回False
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
				myTail = item; //第一个节点
			}
			myHead->next = item;			
		}

		length += 1;
		return true;
	}

	return false;
}

//
// 函数：append(void* payload, Item_List_T* position)
//
// 目的：在position节点后插入节点item
//       如果position==NULL，插入失败
//
// 返回值：插入成功返回True，否则返回False
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
		else //尾部插入
		{
			if (myTail == NULL) //第一个节点
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
// 函数：remove(Item_List_T* item)
//
// 目的：将当前节点从链表移除
//
// 返回值：如果item == NULL, 返回NULL
//         如果item != NULL, 移除并返回该节点
//         如果item != NULL, 但是没有找到，抛出异常
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
			throw YCException(2002, "YCDList待删除节点未发现！");
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
// 函数：clear()
//
// 目的：清空当前链表，Delete所有item
//       由于保存的payload都是void*指针，所以delete不会
//       调用到对应的析构函数，在调用析构函数前必须调用clear
//       但是链表头仍然保留
//
// 注释：
//       如果release == NULL, 表明链表里面的payload都不持有生命周期，不会释放
//       如果release != NULL, 则会对链表中的每一项调用release(payload)以释放资源
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
// 函数：begin()
//
// 目的：正向迭代，在myHead指向的头节点
//
// 返回值：size==0，返回NULL
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
// 函数：prev()
//
// 目的：反向迭代，返回current的前一个节点
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
// 函数：next()
//
// 目的：正向迭代，返回current的下一个节点
//
// 返回值：size==0，返回NULL
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
// 函数：end()
//
// 目的：正向迭代，在myHead指向的尾节点
//
// 返回值：始终为NULL
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
// 函数：r_begin()
//
// 目的：反向迭代，在myHead指向的尾节点
//
// 返回值：size==0，返回myHead
//
Item_List_T* YCDList::r_begin()
{
	return myTail; //(myTail != NULL) ? myTail : myHead;
}

//
// 函数：next()
//
// 目的：反向迭代，返回current的前一个节点
//
// 返回值：size==0，返回myHead
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
// 函数：end()
//
// 目的：反向迭代，在myHead指向的头节点
//
// 返回值：始终为myHead
//
Item_List_T* YCDList::r_end()
{
	return NULL; //myHead
}