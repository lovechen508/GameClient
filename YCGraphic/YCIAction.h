#ifndef _INCLUDE_YCIACTION_H_
#define _INCLUDE_YCIACTION_H_

class YCIAnimation;

/*
 * YCIAction : ���ж����Ļ���
 *
 *     վ�������ߣ��ܲ���ħ��������
 *
 *     ���Բ���MemPool����Action
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
	// ������bind2Animation(YCIAnimation* animation)
	//
	// Ŀ�ģ��󶨶���������
	//
	virtual void bind2Animation(YCIAnimation* animation);

	//
	// ������start()
	//
	// Ŀ�ģ���������
	//
	void start();

	//
	// ������run(unsigned int timestamp)
	//
	// Ŀ�ģ��ܶ���
	//
	virtual void run(unsigned int timestamp) = 0;

	//
	// ������stop()
	//
	// Ŀ�ģ�ֹͣ����
	//
	void stop();

	//
	// ������isStopped()
	//
	// Ŀ�ģ������Ƿ����
	//
	bool isStopped();

private:

	E_ACTION_STATE myState;

	//
	// ���ԣ�myAnimation
	//
	// Ŀ�ģ���������
	//
	YCIAnimation *myAnimation;
};

#endif
