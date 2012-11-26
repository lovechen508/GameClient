#ifndef _INCLUDE_YCSELECTROLESTAGE_H_
#define _INCLUDE_YCSELECTROLESTAGE_H_

#include "YCIGameStage.h"

/*
 * YCSelectRoleStage : ѡ��ɫ����
 */
class YCAPI YCSelectRoleStage :
	public YCIGameStage
{
public:

	YCSelectRoleStage(YCGame* game);

	virtual ~YCSelectRoleStage(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ����󴴽�����
	//
	virtual bool initialize(YCConfig& config);

	//
	// ������enter()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void enter();

	//
	// ������with(unsigned int id, unsigned int sessionId)
	//
	// Ŀ�ģ�����sessionId
	//
	virtual YCIFsmState* with(unsigned int id, unsigned int sessionId);

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	bool handle(UINT message, WPARAM wParam, LPARAM lParam);

	//
	// ������leave()
	//
	// Ŀ�ģ�״̬����
	//
	virtual void leave();

	//
	// ����: finalize()
	//
	// Ŀ�ģ�������������
	//
	virtual void finalize();

private:

	//
	// �ػ�ID
	//
	unsigned int mySessionId;

	//
	// ��ɫID
	//
	unsigned int myRoleId;
};

#endif

