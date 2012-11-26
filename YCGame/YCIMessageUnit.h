#ifndef _INCLUDE_YCIMESSAGE_H_
#define _INCLUDE_YCIMESSAGE_H_

#include "YCGraphic/YCIModel.h"

struct YCIUnitAction;

/*
 * YCIMessageUnit : ������Ϣ��Ԫ�ĳ��� ������Ϣ��������Ϣ��������Ϣ������
 *                  ������Ϣ��Ԫ���ӳ�һ������һ�������ʾһ��
 */
class YCIMessageUnit : YCIModel
{
public:

	YCIMessageUnit(void);

	virtual ~YCIMessageUnit(void);

	//
	// ������bindMouseClick(YCIUnitAction* action)
	//
	// Ŀ�ģ���������¼�
	//
	void bindMouseClick(YCIUnitAction* action);

	//
	// ������isMouseClickEnabled()
	//
	// Ŀ�ģ�����Ѱ�������¼�������true
	//      ���򷵻�false
	//
	bool isMouseClickEnabled();

	//
	// ������onMouseClick()
	//
	// Ŀ�ģ�������¼�����
	//
	void onMouseClick();

	//
	// ������append(YCIMessageUnit* next)
	//
	// Ŀ�ģ�׷����Ϣ��Ԫ
	//
	void append(YCIMessageUnit* next);

	//
	// ������next() const
	//
	// Ŀ�ģ������Ϣ��Ԫ
	//
	YCIMessageUnit* next();
	const YCIMessageUnit* next() const;

private:

	//
	// ������������
	//
	YCIUnitAction* myMouseClickAction;

	//
	// �����Ϣ��Ԫ
	//
	YCIMessageUnit* myNext;
};

#endif

