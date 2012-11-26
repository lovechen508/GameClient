#include "YCSelectRoleStage.h"

#include "YCGameStageDefine.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCAssert.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

YCSelectRoleStage::YCSelectRoleStage(YCGame* game)
	: YCIGameStage(game, YCIGameStage::SELECTROLE_STAGE)
	, mySessionId(0)
{
}


YCSelectRoleStage::~YCSelectRoleStage(void)
{
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ����󴴽�����
//
bool YCSelectRoleStage::initialize(YCConfig& config)
{
	return true;
}

//
// ������enter()
//
// Ŀ�ģ�״̬����
//
void YCSelectRoleStage::enter()
{
	mySessionId = 0;
}

//
// ������with(unsigned int id, unsigned int sessionId)
//
// Ŀ�ģ�����sessionId
//
YCIFsmState* YCSelectRoleStage::with(unsigned int id, unsigned int sessionId)
{
	SMART_ASSERT(id == TRANSFER_SESSION);
	mySessionId = sessionId;

	return this;
}

//
// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ���Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCSelectRoleStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// ������leave()
//
// Ŀ�ģ�״̬����
//
void YCSelectRoleStage::leave()
{
	mySessionId = 0;
}

//
// ����: finalize()
//
// Ŀ�ģ�������������
//
void YCSelectRoleStage::finalize()
{

}
