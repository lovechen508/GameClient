#ifndef _INCLUDE_YCTEXTUREFACTORY_H_
#define _INCLUDE_YCTEXTUREFACTORY_H_

#include <string>

#include "YCBasic\YCDef.h"
#include "YCBasic\YCThreadPool.h"

#include "YCTextureDef.h"
#include "YCNetworkFileLoadListener.h"

class YCConfig;
class YCXmlReader;
class YCITexture2D;
class YCTextureManager;
class YCFileTextureLoader;
class YCNetworkFileLoader;

struct ATLAS_CUSTOMDATA_T;

/*
 * YCTextureFactory ：纹理加载工厂类
 *
 * 注释：
 *      完成纹理的下载，加载，对YCTextureManager屏蔽纹理的硬件存储细节
 */
class YCTextureFactory
	: public YCNetworkFileLoadListener
{
public:
	
	YCTextureFactory(void);
	
	~YCTextureFactory(void);

	//
	// 函数：initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig* config)
	//
	// 目的：配置工厂类，如WebServer Address, 并发连接数，等待超时...
	//
	bool initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig& config);

	//
	// 函数：fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename)
	//
	// 目的：获取纹理
	//
	YCITexture2D* fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename);

	//
	// 函数：finalize()
	//
	// 目的：关闭线程池，释放相关资源
	//
	void finalize();

	//
	// 函数：getBaseURL()
	//
	// 目的：取得BaseURL
	//
	const char* getBaseURL() const;

	//
	// 函数：notify(const char* filename, bool result)
	//
	// 目的：下载文件结果通知
	//
	// 参数：
	//       filename : 下载的文件名
	//       result   ：下载结果
	//
	virtual void notify(const char* filename, bool result);

	//
	// 函数：loadTextureInAtlasXML(E_TEXTURE_TYPE type, int uniqueId, const char* xmlFile)
	//
	// 目的：处理Atlas纹理XML配置文件
	//
	// 参数：
	//
	//     uniqueId : 最初请求的uniqueId
	//     url      : 曾经提交的url请求
	//     xmlFile  : Atlas XML文件名
	//
	YCITexture2D* loadTextureInAtlasXML(E_TEXTURE_TYPE type, int uniqueId, const char* path, const char* xmlFile);

public:

	//
	// 函数：loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData)
	//
	// 目的：读取Altas纹理集合中的每一个纹理
	//
	bool loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData);

private:

	//
	// 函数：getPathByType(E_TEXTURE_TYPE type)
	//
	// 目的：取得纹理路径
	//
	const char* getPathByType(E_TEXTURE_TYPE type);

private:

	//相关资源路径
	char myTileMapPath[MAX_PATH];
	char mySharedMapPath[MAX_PATH];
	char myObstaclePath[MAX_PATH];
	char myNPCPath[MAX_PATH];
	char myPlayerPath[MAX_PATH];
	char myMonsterPath[MAX_PATH];
	char myEffectPath[MAX_PATH];
	char myUIPath[MAX_PATH];
	
	YCTextureManager* myTextureManager;

	YCFileTextureLoader* myFileLoader;

	YCNetworkFileLoader* myNetworkLoader;
};

#endif

