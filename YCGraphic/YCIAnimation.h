#ifndef _INCLUDE_YCIANIMATION_H_
#define _INCLUDE_YCIANIMATION_H_

class YCDList;
class YCTexture;

/*
 * YCIAnimation : 场景动画基类
 *                从配置XML文件读取，属于Immutable类
 *                从而播放到那一帧，在什么位置播放都放到
 *                YCIAction类中，客户不能直接操作此类以及继承类
 */
class YCIAnimation
{
public:
	
	YCIAnimation(int animationId);
	
	virtual ~YCIAnimation(void);

	//
	// 函数：draw(void *pD3Device)
	//
	// 目的：绘制动画
	//
	virtual void draw(void *pD3Device) = 0;

	//
	// 函数：appendFrame(YCTexture* texture)
	//
	// 目的：追加一帧到动画中
	//
	void appendFrame(YCTexture* texture);

	//
	// 函数：getAnimationId();
	//
	// 目的：获得动画编号
	//
	int getAnimationId() const;

	//
	// 函数：noOfFrames();
	//
	// 目的：动画的帧数
	//
	int getNoOfFrames() const;

	//
	// 函数：setIntervalInMillisecond();
	//
	// 目的：每一帧的时间间隔,毫秒单位
	//       值限定 10 < n < 1000
	//
	void setIntervalInMillisecond(int millisecondsInterval);

	//
	// 函数：getIntervalInMillisecond();
	//
	// 目的：每一帧的时间间隔,毫秒单位
	//
	int getIntervalInMillisecond() const;

	//
	// 函数：setDirection(int direction)
	//
	// 目的：设置动画的方向
	//
	// 注释：
	//      如果是某方向的，传入0~15之间的一个值
	//      如果是全方向的，传入16
	//
	void setDirection(int direction);

	//
	// 函数：getDirection()
	//
	// 目的：取得动画的方向
	//
	// 注释：
	//      如果是某方向的，返回0~15之间的一个值
	//      如果是全方向的，返回16
	//
	int getDirection() const;

	//
	// 函数：getFrameAt(int frameNo)
	//
	// 目的：返回某一帧的纹理，用于绘制
	//
	YCTexture* getFrameAt(int frameNo);

private:

	int myAnimationId;

	int myMilliSecondsInterval;

	int myDirection;

	YCDList *myFrames;

};

#endif