#ifndef _INCLUDE_YCIACTION_H_
#define _INCLUDE_YCIACTION_H_

class YCIAnimation;

/*
 * YCIAction : 所有动作的基类
 *
 *     站立，行走，跑步，魔法。。。
 *
 *     可以采用MemPool缓存Action
 *
 *                                                                         -----------------
 *                                                                        |    YCIAction    |
 *                                                                         -----------------
 *                                                                                 |
 *                                        ------------------------------------------------------------------------------------------
 *                                       |                              |                             |                             |
 *                               ------------------           -------------------              ----------------             ------------------
 *                              | YCSequenceAction |         | YCConditionAction |            | YCRepeatAction |           | YCParallelAction |
 *                               ------------------           -------------------              ----------------             ------------------
 *
 *
 */
class YCIAction
{
	typedef enum eActionState {RUNNING, STOPPED} E_ACTION_STATE;

public:

	YCIAction(void);

	virtual ~YCIAction(void);

	//
	// 函数：bind2Animation(YCIAnimation* animation)
	//
	// 目的：绑定动作到动画
	//
	virtual void bind2Animation(YCIAnimation* animation);

	//
	// 函数：start()
	//
	// 目的：启动动画
	//
	void start();

	//
	// 函数：run(unsigned int timestamp)
	//
	// 目的：跑动画
	//
	virtual void run(unsigned int timestamp) = 0;

	//
	// 函数：stop()
	//
	// 目的：停止动画
	//
	void stop();

	//
	// 函数：isStopped()
	//
	// 目的：动画是否结束
	//
	bool isStopped();

private:

	E_ACTION_STATE myState;

	//
	// 属性：myAnimation
	//
	// 目的：动画对象
	//
	YCIAnimation *myAnimation;
};

#endif
