#ifndef _INCLUDE_YCIUIACTION_H_
#define _INCLUDE_YCIUIACTION_H_

/*
 * YCIUIAction : UIԪ�����������ӿ���
 *
 * ˵������moveTo, fadeIn, fadeOut, animation ��
 *       �����Զ�����Ҫѹ�뵽YCIUITagԪ����ActionQueue��
 *       �ȵ���һ������ִ����ɺ���ִ����һ������
 *       ��ʵ����ʽ����YCQueue.selector("window").fadeIn(slow).moveTo(x, y);
 */
class YCIUIAction
{

public:

	//
	// ������bool run()
	//
	// Ŀ�ģ���YCTimer��������run����
	//
	// ע�ͣ�
	//       ����������front��ActionQueue��front()�õ������
	//       ����ִ����ɷ���true, ���ActionQueue��pop����
	//       ����ִ����ɷ���false, ����Ȼ������ActionQueue��
	//
	virtual bool run() = 0;

protected:

	//
	// ������YCIUIAction(int durationTimes)
	//
	// Ŀ�ģ�
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