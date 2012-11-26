#include "YCGameStage.h"

#include "YCGameStageDefine.h"

#include "YCMainPlayer.h"
#include "YCMapFactory.h"
#include "YCChatManager.h"
#include "YCTaskManager.h"
#include "YCBuffManager.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCLogger.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

YCGameStage::YCGameStage(YCGame* game)
	: YCIGameStage(game, YCIGameStage::GAME_STAGE)
	, myMapFactory(NULL)
	, myChatManager(NULL)
	, myTaskManager(NULL)
	, mySessionId(0)
	, myRoleId(0)
	, mySelf(NULL)
{
}


YCGameStage::~YCGameStage(void)
{
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ����󴴽�����
//
bool YCGameStage::initialize(YCConfig& config)
{
	myMapFactory = new YCMapFactory();
	if (!myMapFactory->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize��ʼ��MapFactory�ɹ���");
	
	// ע���ͼ����
	myGraphic->registerMapFactory(myMapFactory);

	myChatManager = new YCChatManager();
	if (!myChatManager->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize��ʼ��ChatManager�ɹ���");

	myTaskManager = new YCTaskManager();
	if (!myTaskManager->initialize(config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize��ʼ��TaskManager�ɹ���");

	//myBuffManager = new YCBuffManager();
	//if (!myBuffManager->)

	return true;
}

//
// ������enter()
//
// Ŀ�ģ�״̬����
//
void YCGameStage::enter()
{
	SMART_ASSERT(mySelf == NULL);
}

//
// ������with(unsigned int id, unsigned int val)
//
// Ŀ�ģ�����sessionId, roleId
//
YCIFsmState* YCGameStage::with(unsigned int id, unsigned int val)
{
	if (id == TRANSFER_SESSION)
	{
		mySessionId = val;
	}
	else if (id == TRANSFER_ROLEID)
	{
		myRoleId = val;
	}

	return this;
}

//
// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ���Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCGameStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// ������leave()
//
// Ŀ�ģ�״̬����
//
void YCGameStage::leave()
{
	SAFE_DELETE(mySelf);
	mySessionId = 0;
	myRoleId = 0;
}

//
// ����: finalize()
//
// Ŀ�ģ�������������
//
void YCGameStage::finalize()
{
	if (myMapFactory != NULL)
	{
		myMapFactory->finalize();
		SAFE_DELETE(myMapFactory);
	}

	if (myChatManager != NULL)
	{
		myChatManager->finalize();
		SAFE_DELETE(myChatManager);
	}

	if (myTaskManager != NULL)
	{
		myTaskManager->finalize();
		SAFE_DELETE(myTaskManager);
	}
}