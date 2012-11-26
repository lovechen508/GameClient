#include "YCLoginStage.h"

#include "YCGameStageDefine.h"

#include "YCLua\YCLua.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"
#include "YCGraphic\YCGraphic.h"
#include "YCGraphic\YCUIManager.h"
#include "YCNetwork\YCNetwork.h"

YCLoginStage::YCLoginStage(YCGame* game)
	: YCIGameStage(game, YCIGameStage::LOGIN_STAGE)
	, myHost(NULL)
	, myPort(0)
{
}


YCLoginStage::~YCLoginStage(void)
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：对象创建调用
//
bool YCLoginStage::initialize(YCConfig& config)
{
	return true;
}

//
// 函数：with(const char* ip)
//
// 目的：登录服务器IP
//
YCIFsmState* YCLoginStage::with(unsigned int id, const char* ip)
{
	SMART_ASSERT(id == TRANSFER_IP);
	SMART_ASSERT(ip != NULL);

	if (ip == NULL)
	{
		throw YCException(2002, "YCLoginStage::with无效登录服务器网络地址, NULL");
	}

	int len = strlen(ip);
	myHost = new char[len+1];
	memcpy(myHost, ip, len);
	myHost[len] = '\0';

	return this;
}

//
// 函数：with(unsigned int id, int port)
//
// 目的：登录服务器Port
//
YCIFsmState* YCLoginStage::with(unsigned int id, int port)
{
	SMART_ASSERT(id == TRANSFER_PORT);
	SMART_ASSERT(port > 1024);

	if (port <= 1024)
	{
		throw YCException(2002, "YCLoginStage::with无效登录服务器网络端口, <1024");
	}
	myPort = port;

	return this;
}
	
//
// 函数：enter()
//
// 目的：状态进入
//
void YCLoginStage::enter()
{

}

//
// 函数：bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// 目的：消息处理函数
//
// 返回值：已处理，返回true，否则返回false
//
bool YCLoginStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// 函数：leave()
//
// 目的：状态结束
//
void YCLoginStage::leave()
{

}

//
// 函数: finalize()
//
// 目的：对象析构调用
//
void YCLoginStage::finalize()
{
	SAFE_DELETE_ARRAY(myHost);
}

//////////////////////////////////////////////////////////////
// Private Part 
//////////////////////////////////////////////////////////////

#pragma region MessageHandler
//
// 函数：login(const char* username, const char* password)
//
// 目的：从LoginServer登录
//
// 注释：阻塞函数，知道服务器返回或者超时
//
bool YCLoginStage::login(const char* username, const char* password)
{
	return true;
}

#pragma endregion MessageHandler