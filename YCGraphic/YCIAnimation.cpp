#include "YCIAnimation.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCIAnimation::YCIAnimation(int animationId)
	: myAnimationId(animationId)
	, myFrames(new YCDList())
	, myDirection(-1)
	, myMilliSecondsInterval(100)
{
}


YCIAnimation::~YCIAnimation(void)
{
}

//
// 函数：appendFrame(YCTexture* texture)
//
// 目的：追加一帧到动画中
//
void YCIAnimation::appendFrame(YCTexture* texture)
{
	myFrames->append(texture, NULL);
}

//
// 函数：getAnimationId();
//
// 目的：获得动画编号
//
int YCIAnimation::getAnimationId() const
{
	return myAnimationId;
}

//
// 函数：getNoOfFrames();
//
// 目的：动画的帧数
//
int YCIAnimation::getNoOfFrames() const
{
	return myFrames->size();
}

//
// 函数：setIntervalInMillisecond();
//
// 目的：每一帧的时间间隔,毫秒单位
//
void YCIAnimation::setIntervalInMillisecond(int millisecondsInterval)
{
	if (millisecondsInterval <= 10 || millisecondsInterval >= 1000)
	{
		LOG_WARNING("YCIAnimation::setIntervalInMillisecond 动画时间间隔设置可能有误：" << millisecondsInterval);
	}
	else
	{
		myMilliSecondsInterval = millisecondsInterval;
	}
}

//
// 函数：getIntervalInMilliseconds();
//
// 目的：每一帧的时间间隔
//
int YCIAnimation::getIntervalInMillisecond() const
{
	return myMilliSecondsInterval;
}

//
// 函数：setDirection(int direction)
//
// 目的：设置动画的方向
//
// 注释：
//      如果是某方向的，传入0~15之间的一个值
//      如果是全方向的，传入16
//
void YCIAnimation::setDirection(int direction)
{
	if (direction < 0 || direction > 16)
	{
		throw YCException(2002, "YCIAnimation::setDirection方向设置有误!");
	}
	else
	{
		myDirection = direction;
	}
}

//
// 函数：getDirection()
//
// 目的：取得动画的方向
//
// 注释：
//      如果是某方向的，返回0~15之间的一个值
//      如果是全方向的，返回16
//
int YCIAnimation::getDirection() const
{
	return myDirection;
}

//
// 函数：getFrameAt(int frameNo)
//
// 目的：返回某一帧的纹理，用于绘制
//
YCTexture* YCIAnimation::getFrameAt(int frameNo)
{
	return NULL;
}

