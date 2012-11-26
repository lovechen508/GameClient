#ifndef _INCLUDE_YCGAMESTAGE_H_
#define _INCLUDE_YCGAMESTAGE_H_

#include "YCIGameStage.h"

class YCMainPlayer;

class YCGame;
class YCGraphic;
class YCUIManager;
class YCMapFactory;
class YCChatManager;
class YCTaskManager;
class YCBuffManager;

class YCAPI YCGameStage :
	public YCIGameStage
{
public:

	YCGameStage(YCGame* game);

	virtual ~YCGameStage(void);

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
	// ������with(unsigned int id, unsigned int val)
	//
	// Ŀ�ģ�����sessionId, roleId
	//
	virtual YCIFsmState* with(unsigned int id, unsigned int val);

	//
	// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
	//
	// Ŀ�ģ���Ϣ������
	//
	// ����ֵ���Ѵ�������true�����򷵻�false
	//
	virtual bool handle(UINT message, WPARAM wParam, LPARAM lParam);

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

	//
	// �����
	//
	YCMainPlayer* mySelf;
	
	//
	// ��ͼ��������������������
	//
	YCMapFactory* myMapFactory;

	//
	// �����������������������
	//
	YCChatManager* myChatManager;

	//
	// �����������������������
	//
	YCTaskManager* myTaskManager;

	//
	// Buff��������������������
	//
	YCBuffManager* myBuffManager;

};

#endif

