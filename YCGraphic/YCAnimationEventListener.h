#ifndef _INCLUDE_YCANIMATION_EVENT_LISTENER_H_
#define _INCLUDE_YCANIMATION_EVENT_LISTENER_H_

/*
 * YCAnimationEventListener : �����¼�������
 */
class YCAnimationEventListener
{
public:

	typedef enum eAnimationEvent {
		PRE_ANIMATION				= 1,		//������ʼ�Ļص�����
		BEFORE_FRAME_N_ANIMATION    = 2,        //��n֡������ʼ֮ǰ
		AFTER_FRAME_N_ANIMATION     = 3,        //��n֡�������֮��

		FINISHED_ONETIME_ANIMATION	= 9,		//һ��ѭ������
		POST_ANIMATION				= 10,       //��������������֮ǰ
	} E_ANIMATION_EVENT;

public:

	YCAnimationEventListener() {}

	virtual ~YCAnimationEventListener() {}

	//
	// ������notify(E_ANIMATION_EVENT event, YCIAnimation* animation, unsigned int customData)
	//
	// Ŀ�ģ������¼��ص�
	//
	// ������
	//      customData : BEFORE_FRAME_N_ANIMATION/AFTER_FRAME_N_ANIMATION �����n֡
	//                   FINISHED_ONETIME_ANIMATION �����n�ζ�������
	//
	virtual void notify(E_ANIMATION_EVENT event, YCIAnimation* animation, unsigned int customData) = 0;
};

#endif