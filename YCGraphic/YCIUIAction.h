#ifndef _INCLUDE_YCIUIACTION_H_
#define _INCLUDE_YCIUIACTION_H_

/*
 * YCIUIAction : UI元件持续动作接口类
 *
 * 说明：如moveTo, fadeIn, fadeOut, animation 等
 *       持续性动作需要压入到YCIUITag元件的ActionQueue中
 *       等到上一个动作执行完成后，再执行下一个动作
 *       以实现链式调用YCQueue.selector("window").fadeIn(slow).moveTo(x, y);
 */
class YCIUIAction
{

public:

	//
	// 函数：bool run()
	//
	// 目的：由YCTimer触发调用run函数
	//
	// 注释：
	//       动作首先由front从ActionQueue中front()得到，如果
	//       动作执行完成返回true, 则从ActionQueue中pop出来
	//       动作执行完成返回false, 则仍然保留在ActionQueue中
	//
	virtual bool run() = 0;

protected:

	//
	// 函数：YCIUIAction(int durationTimes)
	//
	// 目的：
	//
	YCIUIAction(int durationTimes)
		: myDurationTimes(durationTimes)
	{
	}

	virtual ~YCIUIAction()
	{
	}
	
	int myDurationTimes;
};



#endif