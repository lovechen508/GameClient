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
// 函数：stageNo() const
//
// 目的：取得状态编号
//
unsigned int YCIGameStage::stageNo() const
{
	return myStage;
}

//
// 函数：nextState(unsigned int)
//
// 目的：是否下一状态
//
bool YCIGameStage::nextState(unsigned int next)
{
	return (myStage == next);
}

//
// 函数：with(unsigned int id, T*)
//
// 目的：设置内部必须数据，YCIFsmTransfer调用
//      不实现，子类实现
//
YCIFsmState* YCIGameStage::with(unsigned int id, void* val)
{
	if (val == NULL)
	{
		throw YCException(2002, "YCIGameStage::with设置值为空！");
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
