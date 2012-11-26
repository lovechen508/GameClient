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
// ������initialize(YCConfig& config)
//
// Ŀ�ģ����󴴽�����
//
bool YCLoginStage::initialize(YCConfig& config)
{
	return true;
}

//
// ������with(const char* ip)
//
// Ŀ�ģ���¼������IP
//
YCIFsmState* YCLoginStage::with(unsigned int id, const char* ip)
{
	SMART_ASSERT(id == TRANSFER_IP);
	SMART_ASSERT(ip != NULL);

	if (ip == NULL)
	{
		throw YCException(2002, "YCLoginStage::with��Ч��¼�����������ַ, NULL");
	}

	int len = strlen(ip);
	myHost = new char[len+1];
	memcpy(myHost, ip, len);
	myHost[len] = '\0';

	return this;
}

//
// ������with(unsigned int id, int port)
//
// Ŀ�ģ���¼������Port
//
YCIFsmState* YCLoginStage::with(unsigned int id, int port)
{
	SMART_ASSERT(id == TRANSFER_PORT);
	SMART_ASSERT(port > 1024);

	if (port <= 1024)
	{
		throw YCException(2002, "YCLoginStage::with��Ч��¼����������˿�, <1024");
	}
	myPort = port;

	return this;
}
	
//
// ������enter()
//
// Ŀ�ģ�״̬����
//
void YCLoginStage::enter()
{

}

//
// ������bool handle(UINT message, WPARAM wParam, LPARAM lParam)
//
// Ŀ�ģ���Ϣ������
//
// ����ֵ���Ѵ�������true�����򷵻�false
//
bool YCLoginStage::handle(UINT message, WPARAM wParam, LPARAM lParam)
{
	return false;
}

//
// ������leave()
//
// Ŀ�ģ�״̬����
//
void YCLoginStage::leave()
{

}

//
// ����: finalize()
//
// Ŀ�ģ�������������
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
// ������login(const char* username, const char* password)
//
// Ŀ�ģ���LoginServer��¼
//
// ע�ͣ�����������֪�����������ػ��߳�ʱ
//
bool YCLoginStage::login(const char* username, const char* password)
{
	return true;
}

#pragma endregion MessageHandler