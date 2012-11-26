#ifndef _INCLUDE_YCDLIST_H_
#define _INCLUDE_YCDLIST_H_

#include "YCDef.h"

template<typename T>
static inline void YCDListClearHelper(void* item)
{
	T* realItem = (T*)item;
	SAFE_DELETE(realItem);
}


/*
 * YCDList : 双向链表类 <不可继承>
 *
 * 注释：用于解决DLL不能暴露STL的问题
 *       替换std::list<T>
 */

struct Item_List_T;

class YCAPI YCDList
{
public:

	YCDList();

	~YCDList();

	//
	// 函数：size()
	//
	// 目的：返回当天链表的长度
	//
	// 返回值：>=0
	//
	int size() const;

	//
	// 函数：find(void* payload)
	//
	// 目的：查找某payload是否在链表中
	//
	// 返回值：>=0
	//
	Item_List_T* find(void* payload);

	//
	// 函数：sort(bool(*compare)(const void *left, const void *right))
	//
	// 目的： 根据compare比较函数排序，越大越靠前
	//
	void sort(bool(*compare)(const void *left, const void *right));

	//
	// 函数：value(Item_List_T* item)
	//
	// 目的：返回当前节点携带的内容
	//
	// 返回值：item的payload
	//
	void* payload(Item_List_T* item);
	const void* payload(const Item_List_T* item) const;

	//
	// 函数：insert(void* payload, Item_List_T* position)
	//
	// 目的：在position节点前插入节点item
	//       如果position==NULL，在链表头插入
	//
	// 返回值：插入成功返回True，否则返回False
	//
	bool insert(void* payload, Item_List_T* position);

	//
	// 函数：append(void* payload, Item_List_T* position)
	//
	// 目的：在position节点后插入节点item
	//       如果position==NULL，在链表尾插入
	//
	// 返回值：插入成功返回True，否则返回False
	//
	bool append(void* payload, Item_List_T* position);

	//
	// 函数：remove(Item_List_T* item)
	//
	// 目的：将当前节点从链表移除
	//
	// 返回值：如果item == NULL, 返回NULL
	//         如果item != NULL, 移除并返回该节点payload
	//         如果item != NULL, 但是没有找到，抛出异常
	//
	// 注意：如果删除成功，参数item已经被释放！
	//
	void* remove(Item_List_T* item);

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
	void clear(void(*release)(void*) = NULL);

	//
	// 函数：begin()
	//
	// 目的：正向迭代，在myHeader指向的头节点
	//
	// 返回值：size==0，返回NULL
	//
	Item_List_T* begin();
	const Item_List_T* begin() const;

	//
	// 函数：prev()
	//
	// 目的：反向迭代，返回current的前一个节点
	//
	Item_List_T* prev(Item_List_T* current);
	const Item_List_T* prev(Item_List_T* current) const;

	//
	// 函数：next()
	//
	// 目的：正向迭代，返回current的下一个节点
	//
	// 返回值：size==0，返回NULL
	//
	Item_List_T* next(Item_List_T* current);
	const Item_List_T* next(const Item_List_T* current) const;

	//
	// 函数：end()
	//
	// 目的：正向迭代，在myHeader指向的尾节点
	//
	// 返回值：始终为NULL
	//
	Item_List_T* end();
	const Item_List_T* end() const;

	//
	// 函数：r_begin()
	//
	// 目的：反向迭代，在myHeader指向的尾节点
	//
	// 返回值：size==0，返回myHeader
	//
	Item_List_T* r_begin();

	//
	// 函数：next()
	//
	// 目的：反向迭代，返回current的前一个节点
	//
	// 返回值：size==0，返回myHeader
	//
	Item_List_T* r_next(Item_List_T* current);

	//
	// 函数：end()
	//
	// 目的：反向迭代，在myHeader指向的头节点
	//
	// 返回值：始终为myHeader
	//
	Item_List_T* r_end();

private:

	Item_List_T* myHead;
	Item_List_T* myTail;

	unsigned int length;
};

#endif