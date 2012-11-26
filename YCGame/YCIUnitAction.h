#ifndef _INCLUDE_YCIUNITACTION_H_
#define _INCLUDE_YCIUNITACTION_H_

#include <functional>
#include <algorithm>

//
// 消息单元响应模版类 ：VS2010
//
// 使用实例：
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
// YCIUnitAction ：给消息单元增加行为
//
struct YCIUnitAction
{

	//
	// 注释：鼠标点击事件到达
	//
	virtual void onMouseClick() = 0;
};

//
// 鼠标点击事件响应模版类
//
template<typename Fn>
struct YCUnitActionFunctor : public YCIUnitAction
{
	YCUnitActionFunctor(Fn& fn) : myFunctor(fn) {}

	//
	// 注释：鼠标点击事件到达
	//
	virtual void onMouseClick() { myFunctor(); }

private:

	Fn myFunctor;

};

//
// 模版帮助函数，利用编译器自动类型推导
//
template<typename Fn>
YCIUnitAction* buildUnitAction(Fn& fn)
{
	return new YCUnitActionFunctor<Fn>(fn);
}

#endif