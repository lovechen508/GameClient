#ifndef _INCLUDE_YCIANIMATION_H_
#define _INCLUDE_YCIANIMATION_H_

class YCDList;
class YCTexture;

/*
 * YCIAnimation : ������������
 *                ������XML�ļ���ȡ������Immutable��
 *                �Ӷ����ŵ���һ֡����ʲôλ�ò��Ŷ��ŵ�
 *                YCIAction���У��ͻ�����ֱ�Ӳ��������Լ��̳���
 */
class YCIAnimation
{
public:
	
	YCIAnimation(int animationId);
	
	virtual ~YCIAnimation(void);

	//
	// ������draw(void *pD3Device)
	//
	// Ŀ�ģ����ƶ���
	//
	virtual void draw(void *pD3Device) = 0;

	//
	// ������appendFrame(YCTexture* texture)
	//
	// Ŀ�ģ�׷��һ֡��������
	//
	void appendFrame(YCTexture* texture);

	//
	// ������getAnimationId();
	//
	// Ŀ�ģ���ö������
	//
	int getAnimationId() const;

	//
	// ������noOfFrames();
	//
	// Ŀ�ģ�������֡��
	//
	int getNoOfFrames() const;

	//
	// ������setIntervalInMillisecond();
	//
	// Ŀ�ģ�ÿһ֡��ʱ����,���뵥λ
	//       ֵ�޶� 10 < n < 1000
	//
	void setIntervalInMillisecond(int millisecondsInterval);

	//
	// ������getIntervalInMillisecond();
	//
	// Ŀ�ģ�ÿһ֡��ʱ����,���뵥λ
	//
	int getIntervalInMillisecond() const;

	//
	// ������setDirection(int direction)
	//
	// Ŀ�ģ����ö����ķ���
	//
	// ע�ͣ�
	//      �����ĳ����ģ�����0~15֮���һ��ֵ
	//      �����ȫ����ģ�����16
	//
	void setDirection(int direction);

	//
	// ������getDirection()
	//
	// Ŀ�ģ�ȡ�ö����ķ���
	//
	// ע�ͣ�
	//      �����ĳ����ģ�����0~15֮���һ��ֵ
	//      �����ȫ����ģ�����16
	//
	int getDirection() const;

	//
	// ������getFrameAt(int frameNo)
	//
	// Ŀ�ģ�����ĳһ֡���������ڻ���
	//
	YCTexture* getFrameAt(int frameNo);

private:

	int myAnimationId;

	int myMilliSecondsInterval;

	int myDirection;

	YCDList *myFrames;

};

#endif