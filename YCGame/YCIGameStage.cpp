#include "YCIGameStage.h"

#include "YCGame.h"
#include "YCGameStageDefine.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCException.h"

YCIGameStage::YCIGameStage(YCGame* game, unsigned int stage)
	: YCIFsmState(game)
	, myStage(stage)
	, myLua(NULL)
	, myNetwork(NULL)
	, myGraphic(NULL)
	, myUIManager(NULL)
{
}

YCIGameStage::~YCIGameStage(void)
{
}



//
// ������stageNo() const
//
// Ŀ�ģ�ȡ��״̬���
//
unsigned int YCIGameStage::stageNo() const
{
	return myStage;
}

//
// ������nextState(unsigned int)
//
// Ŀ�ģ��Ƿ���һ״̬
//
bool YCIGameStage::nextState(unsigned int next)
{
	return (myStage == next);
}

//
// ������with(unsigned int id, T*)
//
// Ŀ�ģ������ڲ��������ݣ�YCIFsmTransfer����
//      ��ʵ�֣�����ʵ��
//
YCIFsmState* YCIGameStage::with(unsigned int id, void* val)
{
	if (val == NULL)
	{
		throw YCException(2002, "YCIGameStage::with����ֵΪ�գ�");
	}

	if (id == TRANSFER_LUA)
	{
		myLua = (YCLua*)val;
	}
	else if (id == TRANSFER_NETWORK)
	{
		myNetwork = (YCNetwork*)val;
	}
	else if (id == TRANSFER_GRAPHIC)
	{
		myGraphic = (YCGraphic*)val;
	}
	else if (id == TRANSFER_UI)
	{
		myUIManager = (YCUIManager*)val;
	}

	return this;
}
