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
// ������appendFrame(YCTexture* texture)
//
// Ŀ�ģ�׷��һ֡��������
//
void YCIAnimation::appendFrame(YCTexture* texture)
{
	myFrames->append(texture, NULL);
}

//
// ������getAnimationId();
//
// Ŀ�ģ���ö������
//
int YCIAnimation::getAnimationId() const
{
	return myAnimationId;
}

//
// ������getNoOfFrames();
//
// Ŀ�ģ�������֡��
//
int YCIAnimation::getNoOfFrames() const
{
	return myFrames->size();
}

//
// ������setIntervalInMillisecond();
//
// Ŀ�ģ�ÿһ֡��ʱ����,���뵥λ
//
void YCIAnimation::setIntervalInMillisecond(int millisecondsInterval)
{
	if (millisecondsInterval <= 10 || millisecondsInterval >= 1000)
	{
		LOG_WARNING("YCIAnimation::setIntervalInMillisecond ����ʱ�������ÿ�������" << millisecondsInterval);
	}
	else
	{
		myMilliSecondsInterval = millisecondsInterval;
	}
}

//
// ������getIntervalInMilliseconds();
//
// Ŀ�ģ�ÿһ֡��ʱ����
//
int YCIAnimation::getIntervalInMillisecond() const
{
	return myMilliSecondsInterval;
}

//
// ������setDirection(int direction)
//
// Ŀ�ģ����ö����ķ���
//
// ע�ͣ�
//      �����ĳ����ģ�����0~15֮���һ��ֵ
//      �����ȫ����ģ�����16
//
void YCIAnimation::setDirection(int direction)
{
	if (direction < 0 || direction > 16)
	{
		throw YCException(2002, "YCIAnimation::setDirection������������!");
	}
	else
	{
		myDirection = direction;
	}
}

//
// ������getDirection()
//
// Ŀ�ģ�ȡ�ö����ķ���
//
// ע�ͣ�
//      �����ĳ����ģ�����0~15֮���һ��ֵ
//      �����ȫ����ģ�����16
//
int YCIAnimation::getDirection() const
{
	return myDirection;
}

//
// ������getFrameAt(int frameNo)
//
// Ŀ�ģ�����ĳһ֡���������ڻ���
//
YCTexture* YCIAnimation::getFrameAt(int frameNo)
{
	return NULL;
}

