#ifndef _INCLUDE_YCIUNITACTION_H_
#define _INCLUDE_YCIUNITACTION_H_

#include <functional>
#include <algorithm>

//
// ��Ϣ��Ԫ��Ӧģ���� ��VS2010
//
// ʹ��ʵ����
//
//    1. lambda
//
//
//
//
//
//
//
//
//
//
//
//   2. str::tr1::bind()
//
//
//
//
//
//
//
//
//
//
//
//
//
//


//
// YCIUnitAction ������Ϣ��Ԫ������Ϊ
//
struct YCIUnitAction
{

	//
	// ע�ͣ�������¼�����
	//
	virtual void onMouseClick() = 0;
};

//
// ������¼���Ӧģ����
//
template<typename Fn>
struct YCUnitActionFunctor : public YCIUnitAction
{
	YCUnitActionFunctor(Fn& fn) : myFunctor(fn) {}

	//
	// ע�ͣ�������¼�����
	//
	virtual void onMouseClick() { myFunctor(); }

private:

	Fn myFunctor;

};

//
// ģ��������������ñ������Զ������Ƶ�
//
template<typename Fn>
YCIUnitAction* buildUnitAction(Fn& fn)
{
	return new YCUnitActionFunctor<Fn>(fn);
}

#endif