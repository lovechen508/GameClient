#include "YCConfig.h"

#include "YCXmlReader.h"
#include "YCXmlWriter.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCStringHelper.h"

YCConfig::YCConfig(void)
	: myLoginServer(NULL)
	, myPort(0)
	, myTileMapPath(NULL)
	, mySharedMapPath(NULL)
	, myObstaclePath(NULL)
	, myNPCPath(NULL)
	, myPlayerPath(NULL)
	, myMonsterPath(NULL)
	, myEffectPath(NULL)
	, myUIPath(NULL)
	, mySoundPath(NULL)
	, myLogLevel(YCLogger::LOG_WARNING)
	, myMagicFile(NULL)
	, myChatConfig(NULL)
	, myBuffConfig(NULL)
	, myCoreTagPath(NULL)
	, myDownloadServerURL(NULL)
	, myNoOfConnections(0)
	, myMaxItemsInQueue(10)
{
	memset(myModuleArray, 0, sizeof(myModuleArray));
}

YCConfig::~YCConfig(void)
{
	SAFE_DELETE_ARRAY(myLoginServer);
			   
	SAFE_DELETE_ARRAY(myTileMapPath);
	SAFE_DELETE_ARRAY(mySharedMapPath);
	SAFE_DELETE_ARRAY(myObstaclePath);
	SAFE_DELETE_ARRAY(myNPCPath);
	SAFE_DELETE_ARRAY(myPlayerPath);
	SAFE_DELETE_ARRAY(myMonsterPath);
	SAFE_DELETE_ARRAY(myEffectPath);
	SAFE_DELETE_ARRAY(myUIPath);
	SAFE_DELETE_ARRAY(mySoundPath);
	SAFE_DELETE_ARRAY(myDownloadServerURL);

	//删除模块Name所分配内存
	for (int i = 0; i < DEFAULT_MODULE_NUM; ++i)
	{
		if (myModuleArray[i].name != NULL)
		{
			SAFE_DELETE(myModuleArray[i].name);
		}
	}

	SAFE_DELETE_ARRAY(myMagicFile);
	SAFE_DELETE_ARRAY(myChatConfig);
	SAFE_DELETE_ARRAY(myBuffConfig);
	SAFE_DELETE_ARRAY(myCoreTagPath);
}

//
// 函数：load(const char* filename)
//
// 目的：加载客户端配置信息
//
// 注释：继承类不能覆盖此函数
//       如需加载自定义信息，请实现onLoad函数
//       在此函数结束会调用之
//
bool YCConfig::load(const char* filename)
{
	SMART_ASSERT(filename != NULL);
	YCXmlReader reader(filename);

	memset(myConfigFile, 0, MAX_PATH);
	memcpy(myConfigFile, filename, strlen(filename));

	reader.enter("window")
		    .enter("width")
			  .value(myWindowWidth)
			.leave()
			.enter("height")
			  .value(myWindowHeight)
			.leave()
			.enter("fps")
			  .value(myFPS)
			.leave()
		  .leave()
		  .enter("network")
		    .enter("host")
			  .value(&myLoginServer)
			.leave()
			.enter("port")
			  .value(myPort)
			.leave()
		  .leave()
		  .enter("texture")
		    .enter("durationSeconds")
			  .value(myDurationSeconds)
			.leave()
			.enter("network")
			  .enter("url")
			    .value(&myDownloadServerURL)
			  .leave()
			  .enter("noOfConnections")
			    .value(myNoOfConnections)
			  .leave()
			  .enter("maxItemsInQueue")
			    .value(myMaxItemsInQueue)
			  .leave()
			.leave()
			.enter("data")
			  .foreach("dir", YCXmlReader::YCForEachWrapper<YCConfig>(this, &YCConfig::loadTextureDir))
			.leave()
		  .leave()
		  .enter("log")
		    .attr("level", myLogLevel)
		  .leave()
		  .enter("ui")
		    .enter("core")
			  .value(&myCoreTagPath)
			.leave()
		  .leave()
		  .enter("magic")
		    .enter("file")
			  .value(&myMagicFile)
			.leave()
		  .leave()
		  .enter("chat")
		    .enter("storage")
			  .value(&myChatConfig)
			.leave()
			.enter("color")
			  .foreach("channel", YCXmlReader::YCForEachWrapper<YCConfig>(this, &YCConfig::loadChatColor))
			.leave()
		  .leave()
		  .enter("buff")
			.value(&myBuffConfig)
		  .leave()
		  .enter("modules")
			  .foreach("module", YCXmlReader::YCForEachWrapper<YCConfig>(this, &YCConfig::loadModule))
		  .leave();

	//加载自定义信息
	return onLoad(reader);
}

//
// 函数组：访问屏幕配置信息
//
int YCConfig::width()
{
	return myWindowWidth;
}

int YCConfig::height()
{
	return myWindowHeight;
}

int YCConfig::fps()
{
	return myFPS;
}

//
// 函数组：访问network配置信息
//
const char* YCConfig::loginServer()
{
	return myLoginServer;
}

int YCConfig::port()
{
	return myPort;
}

//
// 函数：获取纹理池存活时间
//
unsigned int YCConfig::durationSeconds()
{
	return myDurationSeconds;
}

//
// 函数组：访问texture下载服务器配置信息
//
const char * YCConfig::downloadServerURL()
{
	return myDownloadServerURL;
}

int YCConfig::noOfConnections()
{
	return myNoOfConnections;
}

int YCConfig::maxItemsInQueue()
{
	return myMaxItemsInQueue;
}

//
// 函数组：访问data配置信息
//
const char * YCConfig::tileMapPath()
{
	return myTileMapPath;
}

const char * YCConfig::sharedMapPath()
{
	return mySharedMapPath;
}

const char * YCConfig::obstaclePath()
{
	return myObstaclePath;
}

const char * YCConfig::npcPath()
{
	return myNPCPath;
}

const char * YCConfig::playerPath()
{
	return myPlayerPath;
}

const char * YCConfig::monsterPath()
{
	return myMonsterPath;
}

const char * YCConfig::effectPath()
{
	return myEffectPath;
}

const char * YCConfig::uiPath()
{
	return myUIPath;
}

const char * YCConfig::soundPath()
{
	return mySoundPath;
}

//
// 函数组：访问Chat配置信息
//
const char * YCConfig::chatCfgPath()
{
	return myChatConfig;
}

unsigned int YCConfig::chatLightColor()
{
	return myLightColor;
}

unsigned int YCConfig::chatPrivateColor()
{
	return myPrivateColor;
}

unsigned int YCConfig::chatFriendColor()
{
	return myFriendColor;
}

unsigned int YCConfig::chatTeamColor()
{
	return myTeamColor;
}

unsigned int YCConfig::chatFamilyColor()
{
	return myFamilyColor;
}

unsigned int YCConfig::chatCountryColor()
{
	return myCountryColor;
}

unsigned int YCConfig::chatWorldColor()
{
	return myWorldColor;
}

unsigned int YCConfig::chatSystemColor()
{
	return mySystemColor;
}

unsigned int YCConfig::chatGlobalColor()
{
	return myGlobalColor;
}

//
// 函数：访问日志级别信息
//
YCLogger::LOG_LEVEL YCConfig::logLevel()
{
	return (YCLogger::LOG_LEVEL)myLogLevel;
}

//
// 函数：访问UI 核心标签信息
// 
const char * YCConfig::coreTagPath()
{
	return myCoreTagPath;
}

//
// 函数：onLoad(YCXmlReader* root)
//
// 目的：加载客户端自定义配置, 缺省空实现
//
// 注释： 
//     E.g. 配置文件结构： configure -> network
//                                | -> data
//                                | -> log
//                                | -> ...
//                                | -> fps             客户端自定义配置（onLoad虚函数回调）
//
//     读取方法为：     root.enter("fps")
//                             .value(fps)
//                         .leave();
//
bool YCConfig::onLoad(YCXmlReader& root)
{
	return true;
}

///////////////////////////////////////////////////////////////////
// 私有成员函数定义部分
///////////////////////////////////////////////////////////////////

//
// 函数：loadModule(YCXmlReader&)
//
// 目的：读取模块配置
//
bool YCConfig::loadModule(YCXmlReader& reader)
{
	return true;
}

//
// 函数：loadTextureDir(YCXmlReader&)
//
// 目的：读取纹理目录配置
//
bool YCConfig::loadTextureDir(YCXmlReader& reader)
{
	int no;
	reader.attr("no", no);

	if      (no == 1) reader.attr("path", &myTileMapPath);
	else if (no == 2) reader.attr("path", &mySharedMapPath);
	else if (no == 3) reader.attr("path", &myObstaclePath);
	else if (no == 4) reader.attr("path", &myNPCPath);
	else if (no == 5) reader.attr("path", &myPlayerPath);
	else if (no == 6) reader.attr("path", &myMonsterPath);
	else if (no == 7) reader.attr("path", &myEffectPath);
	else if (no == 8) reader.attr("path", &myUIPath);
	else              return false;

	return true;
}


//
// 函数：loadChatColor(YCXmlReader& reader)
//
// 目的：加载配色信息
//
bool YCConfig::loadChatColor(YCXmlReader& reader)
{
	char* strColor = NULL;
	char* channel = NULL;

	reader.attr("name", &channel)
		  .value(&strColor);

	unsigned int color;
	if (!YCStringHelper::parseColor(strColor, color))
	{
		return false;
	}
	color += 0xFF000000;

	if      (strcmp(channel, "light") == 0)   myLightColor = color;
	else if (strcmp(channel, "private") == 0) myPrivateColor = color;
	else if (strcmp(channel, "friend") == 0)  myFriendColor = color;
	else if (strcmp(channel, "team") == 0)    myTeamColor = color;
	else if (strcmp(channel, "family") == 0)  myFamilyColor = color;
	else if (strcmp(channel, "country") == 0) myCountryColor = color;
	else if (strcmp(channel, "system") == 0)  mySystemColor = color;
	else if (strcmp(channel, "world") == 0)   myWorldColor = color;
	else if (strcmp(channel, "global") == 0)  myGlobalColor = color;

	SAFE_DELETE_ARRAY(channel);
	SAFE_DELETE_ARRAY(strColor);

	return true;
}