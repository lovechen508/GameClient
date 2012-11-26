#ifndef _INCLUDE_YCBLACKFILTER_H_
#define _INCLUDE_YCBLACKFILTER_H_

#include "YCIChatFilter.h"

class YCDList;

/*
 * YCBlackFilter : ������������
 */
class YCBlackFilter :
	public YCIChatFilter
{
public:

	YCBlackFilter(const YCDList* blacklist, YCIChatFilter* next);

	virtual ~YCBlackFilter(void);

private:

	//
	// ������selfValidate(const NET_CHAT_MSG_NTF_T* msg)
	//
	// Ŀ�ģ�����У����Ϣ�Ƿ�Ϸ�
	//
	virtual bool selfValidate(const NET_CHAT_MSG_NTF_T* msg);

private:

	//
	// �������б�����, ��������������
	//
	const YCDList* myRefBlacklist;
};

#endif

