#include "YCGame.h"

#include "YCModuleManager.h"

#include "YCLoginStage.h"
#include "YCSelectRoleStage.h"
#include "YCGameStage.h"
#include "YCGameStageDefine.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCDList.h"
#include "YCInput\YCInput.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"
#include "YCConfig\YCConfig.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

#pragma region createStage

//
// ������createStage(T*)
//
// Ŀ�ģ�ģ�溯����������ϷStage
//
template<typename T>
struct YCGameStageHelper
{
	static YCIGameStage* createStage(YCGame *game)
	{
		YCIGameStage* stage = new T(game);
		stage->with(TRANSFER_LUA, game->getLua())
			 ->with(TRANSFER_NETWORK, game->getNetwork())
			 ->with(TRANSFER_GRAPHIC, game->getGraphic())
			 ->with(TRANSFER_UI, game->getUIManager());
		return stage;
	}
};

#pragma endregion createStage

YCGame::YCGame()
	: myLua(NULL)
	, myNetwork(NULL)
	, myGraphic(NULL)
	, myUIManager(NULL)
	, myModuleManager(NULL)
{

}

YCGame::~YCGame()
{
}

//
// �����飺����ȫ��״̬
//
YCLua* YCGame::getLua() const
{
	return myLua;
}

YCNetwork* YCGame::getNetwork() const
{
	return myNetwork;
}

YCGraphic* YCGame::getGraphic() const
{
	return myGraphic;
}

YCUIManager* YCGame::getUIManager() const
{
	return myUIManager;
}

//
// ������initialize(HWND hWnd, YCInput& input, YCConfig& config)
//
// Ŀ�ģ���ʼ����Ϸ������
//
bool YCGame::initialize(HWND hWnd, YCInput& input, YCConfig& config)
{
	myWnd = hWnd;

	myNetwork = new YCNetwork();
	if (!myNetwork->initialize(myWnd, &input, config))
	{
		return false;
	}
	LOG_DEBUG("YCGame::initialize��ʼ�����粿�ֳɹ�!");

	myLua = new YCLua();
	if (!myLua->bootstrap())
	{
		return false;
	}
	YCRegistry::set("lua", myLua);
	LOG_DEBUG("YCGame::initialize��ʼ��Lua�����ɹ�!");

	return true;
}

//
// ������bindGraphic(YCGraphic* graphic)
//
// Ŀ�ģ���ͼ������
//
void YCGame::bindGraphic(YCGraphic* graphic)
{
	SMART_ASSERT(graphic != NULL);

	if (graphic == NULL)
	{
		throw YCException(2002, "YCGame::bindGraphic�󶨿�graphic��");
	}
	myGraphic = graphic;

	myUIManager = myGraphic->getUIManager();

	//ע����ͼ������
	initializeController();
}

//
// ������launchStage(YCConfig& config)
//
// Ŀ�ģ�������Ϸ����
//
void YCGame::launchStage(YCConfig& config)
{
	// ������Ϸ״̬��
	YCIGameStage* stage = YCGameStageHelper<YCLoginStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage������¼����ʧ��");
	}
	addState(stage, true);

	stage = YCGameStageHelper<YCSelectRoleStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage������ɫ����ʧ��");
	}
	addState(stage, false);

	stage = YCGameStageHelper<YCGameStage>::createStage(this);
	if (!stage->initialize(config))
	{
		throw YCException(2002, "YCGame::launchStage������Ϸ����ʧ��");
	}
	addState(stage,false);
}

//
// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ���Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCGame::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	// if () ����ע���д�����
	// {
	//		
	// }
	// else	
	// {
	//		if (myCurrent != NULL)
	//      {
	// 	        return myCurrent->handle(message, wParam, lParam);
	//      }
	// }

	return false;
}

//
// ������finalize()
//
// Ŀ�ģ��ͷ���Ϸ������
//
void YCGame::finalize()
{	
	if (myLua != NULL)
	{
		YCRegistry::remove("lua");
		SAFE_DELETE(myLua);
		myLua = NULL;
	}

	if (myNetwork != NULL)
	{
		SAFE_DELETE(myNetwork);
		myNetwork = NULL;
	}

	if (myModuleManager != NULL)
	{
		SAFE_DELETE(myModuleManager);
		myModuleManager = NULL;
	}

	for (Item_List_T* item = myStates->begin();
		 item != myStates->end();
		 item = myStates->next(item))
	{
		YCIGameStage* stage = (YCIGameStage*)myStates->payload(item);
		try
		{
			stage->finalize();
		}
		catch (...)
		{
			LOG_ERROR("�ͷ���Ϸ�����쳣��" << stage->stageNo());
		}
	}
}

/////////////////////////////////////////////////////////////
// Private Part
/////////////////////////////////////////////////////////////

#include "YCAvatarController.h"

//
// ������initializeController()
//
// Ŀ�ģ���YCUIManagerע��YCIController
//
void YCGame::initializeController()
{
	SMART_ASSERT(myUIManager != NULL).fatal("���������ΪNULL���޷�ע����ͼ������");

	myUIManager->registController("avatar", new YCAvatarController(myUIManager, myLua));
}


