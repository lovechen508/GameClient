#ifndef _INCLUDE_YCISPRITE_H_
#define _INCLUDE_YCISPRITE_H_

#include <list>

#include "YCIModel.h"

class YCDList;
class YCIAction;

/*
 * YCISprite : ��Ϸ����������Ļ���
 *
 *          ��������ľ  
 *          ������NPC
 *          ħ��������
 *          ������Ч��
 *
 */
class YCAPI YCISprite : public YCIModel
{
public:

	YCISprite(void);

	virtual ~YCISprite(void);

	//
	// ������setId(unsigned long long id)
	//
	// Ŀ�ģ�����ΨһID
	//
	void setId(unsigned long long id);
	unsigned long long getId() const;

	//
	// ������draw(unsigned int timestamp)
	//
	// Ŀ�ģ��ڵ�ͼ���������Ļ���
	//
	virtual void draw(unsigned int timestamp);

	//
	// ������addAction(YCIAction *action)
	//
	// Ŀ�ģ����õ�ǰ����������¶���������ͷ��
	//
	void insertAction(YCIAction *action);

	//
	// ������appendAction(YCIAction *action)
	//
	// Ŀ�ģ������¶���������β
	//
	void appendAction(YCIAction *action);

private:

	//
	// ��Ϸ����ΨһID
	//
	unsigned long long myID;  
	
	//
	// X, Y����
	//
	unsigned int X;	
	unsigned int Y;	

	YCDList* myActions;
};

#endif
