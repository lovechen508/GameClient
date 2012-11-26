#ifndef _INCLUDE_YCCONFIG_H_
#define _INCLUDE_YCCONFIG_H_

#define DEFAULT_MODULE_NUM 50

#include "YCBasic\YCLogger.h"

class YCXmlReader;

/*
 * YC_MODULE_T 系统基本模块
 *
 * 参数 ：
 *        name       模块名称
 *        version    模块版本
 *        state      模块状态
 */
typedef struct _module
{
	const char*  name;
	unsigned int version;
	bool         state;
}YC_MODULE_T;

/*
 * YCConfig　：客户端配置加载接口类
 *             Template Pattern
 *
 * 注释：默认情况下加载 network/texture/log/module
 *
 * 配置文件结构： configure -> network
 *                       | -> texture
 *                       | -> log
 *                       | -> module
 *                       | -> ...
 *                       | -> 客户端自定义配置（onLoad虚函数回调）
 */
class YCAPI YCConfig
{
public:

	YCConfig(void);

	virtual ~YCConfig(void);

	//
	// 函数：load(const char* filename)
	//
	// 目的：加载客户端配置信息
	//
	// 注释：继承类不能覆盖此函数
	//       如需加载自定义信息，请实现onLoad函数
	//       在此函数结束会调用之
	//
	bool load(const char* filename);
	
	//
	// 函数：loadTextureDir(YCXmlReader&)
	//
	// 目的：读取纹理目录配置
	//
	bool loadTextureDir(YCXmlReader&);

	//
	// 函数：loadChatColor(YCXmlReader& reader)
	//
	// 目的：加载配色信息
	//
	bool loadChatColor(YCXmlReader& reader);

	//
	// 函数：loadModule(YCXmlReader&)
	//
	// 目的：读取模块配置
	//
	bool loadModule(YCXmlReader&);

	//
	// 函数组：访问屏幕配置信息
	//
	int width();
	int height();
	int fps();

	//
	// 函数组：访问network配置信息
	//
	const char* loginServer();
	int port();

	// 函数：获取纹理池存活时间
	unsigned int durationSeconds();

	//
	// 函数组：访问texture下载服务器配置信息
	//
	const char * downloadServerURL();
	int noOfConnections();
	int maxItemsInQueue();

	//
	// 函数组：访问data配置信息
	//
	const char * tileMapPath();
	const char * sharedMapPath();
	const char * obstaclePath();
	const char * npcPath();
	const char * playerPath();
	const char * monsterPath();
	const char * effectPath();
	const char * uiPath();
	const char * soundPath();

	//
	// 函数：访问日志级别信息
	//
	YCLogger::LOG_LEVEL logLevel();

	//
	// 函数组：访问Magic配置信息
	//
	const char * magicFile();

	//
	// 函数组：访问Chat配置信息
	//
	const char * chatCfgPath();
	unsigned int chatLightColor();
	unsigned int chatPrivateColor();
	unsigned int chatFriendColor();
	unsigned int chatTeamColor();
	unsigned int chatFamilyColor();
	unsigned int chatCountryColor();
	unsigned int chatWorldColor();
	unsigned int chatSystemColor();
	unsigned int chatGlobalColor();
	
	//
	// 函数：访问UI 核心标签信息
	// 
	const char * coreTagPath();

private:

	//
	// 函数：onLoad(YCXmlReader& root)
	//
	// 目的：加载客户端自定义配置
	//
	// 注释： 
	//     E.g. 配置文件结构： configure -> network
	//                                | -> texture
	//                                | -> log
    //                                | -> ...
    //                                | -> fps             客户端自定义配置（onLoad虚函数回调）
	//
	//     读取方法为：     root.enter("fps")
	//                             .value(fps)
	//                         .leave();
	//
	virtual bool onLoad(YCXmlReader& root);

private:

	// 配置文件绝对路径
	char myConfigFile[MAX_PATH];

	// 屏幕设置
	int myWindowWidth;
	int myWindowHeight;
	int myFPS;

	// 网络配置
	char * myLoginServer;
	int myPort;

	// 纹理池空闲存活时间
	int myDurationSeconds;

	// Texture下载服务器配置
	char * myDownloadServerURL;
	int myNoOfConnections;
	int myMaxItemsInQueue;

	// 资源目录配置
	char * myTileMapPath;
	char * mySharedMapPath;
	char * myObstaclePath;
	char * myNPCPath;
	char * myPlayerPath;
	char * myMonsterPath;
	char * myEffectPath;
	char * myUIPath;
	char * mySoundPath;

	// Log级别配置
	int myLogLevel;

	// 技能信息
	char * myMagicFile;

	// 聊天配置
	char * myChatConfig;

	unsigned int myLightColor;
	unsigned int myPrivateColor;
	unsigned int myFriendColor;
	unsigned int myTeamColor;
	unsigned int myFamilyColor;
	unsigned int myCountryColor;
	unsigned int myWorldColor;
	unsigned int mySystemColor;
	unsigned int myGlobalColor;

	// Buff配置
	char * myBuffConfig;

	// 核心Tag配置
	char * myCoreTagPath;

	// 系统核心模块列表
	YC_MODULE_T myModuleArray[DEFAULT_MODULE_NUM];

};

#endif