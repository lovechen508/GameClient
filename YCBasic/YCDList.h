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
 * YCDList : ˫�������� <���ɼ̳�>
 *
 * ע�ͣ����ڽ��DLL���ܱ�¶STL������
 *       �滻std::list<T>
 */

struct Item_List_T;

class YCAPI YCDList
{
public:

	YCDList();

	~YCDList();

	//
	// ������size()
	//
	// Ŀ�ģ����ص�������ĳ���
	//
	// ����ֵ��>=0
	//
	int size() const;

	//
	// ������find(void* payload)
	//
	// Ŀ�ģ�����ĳpayload�Ƿ���������
	//
	// ����ֵ��>=0
	//
	Item_List_T* find(void* payload);

	//
	// ������sort(bool(*compare)(const void *left, const void *right))
	//
	// Ŀ�ģ� ����compare�ȽϺ�������Խ��Խ��ǰ
	//
	void sort(bool(*compare)(const void *left, const void *right));

	//
	// ������value(Item_List_T* item)
	//
	// Ŀ�ģ����ص�ǰ�ڵ�Я��������
	//
	// ����ֵ��item��payload
	//
	void* payload(Item_List_T* item);
	const void* payload(const Item_List_T* item) const;

	//
	// ������insert(void* payload, Item_List_T* position)
	//
	// Ŀ�ģ���position�ڵ�ǰ����ڵ�item
	//       ���position==NULL��������ͷ����
	//
	// ����ֵ������ɹ�����True�����򷵻�False
	//
	bool insert(void* payload, Item_List_T* position);

	//
	// ������append(void* payload, Item_List_T* position)
	//
	// Ŀ�ģ���position�ڵ�����ڵ�item
	//       ���position==NULL��������β����
	//
	// ����ֵ������ɹ�����True�����򷵻�False
	//
	bool append(void* payload, Item_List_T* position);

	//
	// ������remove(Item_List_T* item)
	//
	// Ŀ�ģ�����ǰ�ڵ�������Ƴ�
	//
	// ����ֵ�����item == NULL, ����NULL
	//         ���item != NULL, �Ƴ������ظýڵ�payload
	//         ���item != NULL, ����û���ҵ����׳��쳣
	//
	// ע�⣺���ɾ���ɹ�������item�Ѿ����ͷţ�
	//
	void* remove(Item_List_T* item);

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
	void clear(void(*release)(void*) = NULL);

	//
	// ������begin()
	//
	// Ŀ�ģ������������myHeaderָ���ͷ�ڵ�
	//
	// ����ֵ��size==0������NULL
	//
	Item_List_T* begin();
	const Item_List_T* begin() const;

	//
	// ������prev()
	//
	// Ŀ�ģ��������������current��ǰһ���ڵ�
	//
	Item_List_T* prev(Item_List_T* current);
	const Item_List_T* prev(Item_List_T* current) const;

	//
	// ������next()
	//
	// Ŀ�ģ��������������current����һ���ڵ�
	//
	// ����ֵ��size==0������NULL
	//
	Item_List_T* next(Item_List_T* current);
	const Item_List_T* next(const Item_List_T* current) const;

	//
	// ������end()
	//
	// Ŀ�ģ������������myHeaderָ���β�ڵ�
	//
	// ����ֵ��ʼ��ΪNULL
	//
	Item_List_T* end();
	const Item_List_T* end() const;

	//
	// ������r_begin()
	//
	// Ŀ�ģ������������myHeaderָ���β�ڵ�
	//
	// ����ֵ��size==0������myHeader
	//
	Item_List_T* r_begin();

	//
	// ������next()
	//
	// Ŀ�ģ��������������current��ǰһ���ڵ�
	//
	// ����ֵ��size==0������myHeader
	//
	Item_List_T* r_next(Item_List_T* current);

	//
	// ������end()
	//
	// Ŀ�ģ������������myHeaderָ���ͷ�ڵ�
	//
	// ����ֵ��ʼ��ΪmyHeader
	//
	Item_List_T* r_end();

private:

	Item_List_T* myHead;
	Item_List_T* myTail;

	unsigned int length;
};

#endif