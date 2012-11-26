#ifndef _INCLUDE_YCANIMATION_EVENT_LISTENER_H_
#define _INCLUDE_YCANIMATION_EVENT_LISTENER_H_

/*
 * YCAnimationEventListener : 动画事件监听器
 */
class YCAnimationEventListener
{
public:

	typedef enum eAnimationEvent {
		PRE_ANIMATION				= 1,		//动画开始的回调函数
		BEFORE_FRAME_N_ANIMATION    = 2,        //第n帧动画开始之前
		AFTER_FRAME_N_ANIMATION     = 3,        //第n帧动画完成之后

		FINISHED_ONETIME_ANIMATION	= 9,		//一次循环结束
		POST_ANIMATION				= 10,       //动画结束，销毁之前
	} E_ANIMATION_EVENT;

public:

	YCAnimationEventListener() {}

	virtual ~YCAnimationEventListener() {}

	//
	// 函数：notify(E_ANIMATION_EVENT event, YCIAnimation* animation, unsigned int customData)
	//
	// 目的：动画事件回调
	//
	// 参数：
	//      customData : BEFORE_FRAME_N_ANIMATION/AFTER_FRAME_N_ANIMATION 代表第n帧
	//                   FINISHED_ONETIME_ANIMATION 代表第n次动画结束
	//
	virtual void notify(E_ANIMATION_EVENT event, YCIAnimation* animation, unsigned int customData) = 0;
};

#endif