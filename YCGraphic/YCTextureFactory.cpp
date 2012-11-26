#include "YCTextureFactory.h"

#include "YCBasic\YCAssert.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCLogger.h"
#include "YCConfig\YCXmlReader.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

#include "YCITextureFile.h"
#include "YCSingleTexture2D.h"
#include "YCAtlasTexture2D.h"
#include "YCTextureIDUtil.h"
#include "YCFileTextureLoader.h"
#include "YCNetworkFileLoader.h"


#pragma region XmlReader

struct TEXTURE_CUSTOMDATA_T {
	YCTextureFactory* factory; //加载工厂类
	int uniqueId;              //需要加载的纹理
	E_TEXTURE_TYPE type;       //纹理类别
	CHAR initialPath[MAX_PATH];//初始路径
};

struct ATLAS_CUSTOMDATA_T {
	E_TEXTURE_TYPE type;       //纹理类别
	const char* initialPath;   //初始路径
	YCDList * idList;          //atlas包含的frameNo列表
};

/*
 * YCTextureConfigHelper : TextureManager 读取辅助类
 * 
 * TextureManager 生成的XML配置文件，和纹理图片应在同一目录中
 *                建议文件名称保存为XXX_XXX_n_texture.xml
 *
 *
 * 文件样例：
 *
 *          <?xml version="1.0" encoding="UTF-8"?>
 *			<!-- Created with TexturePacker http://texturepacker.com-->
 *			<!--  -->
 *			<!--Format:
 *				n  => name of the sprite
 *				x  => sprite x pos in texture
 *				y  => sprite y pos in texture
 *				w  => sprite width (may be trimmed)
 *				h  => sprite height (may be trimmed)
 *				oX => sprite's x-corner offset (only available if trimmed)
 *				oY => sprite's y-corner offset (only available if trimmed)
 *				oW => sprite's original width (only available if trimmed)
 *				oH => sprite's original height (only available if trimmed)
 *				r => 'y' only set if sprite is rotated
 *			-->
 *			<TextureAtlas imagePath="00.png" width="1024" height="1024">
 *				<sprite n="0000" x="389" y="0" w="389" h="576"/>
 *				<sprite n="0001" x="0" y="0" w="389" h="576"/>
 *				<sprite n="0002" x="843" y="0" w="58" h="52"/>
 *				<sprite n="0003" x="778" y="0" w="65" h="65"/>
 *				<sprite n="0004" x="901" y="22" w="42" h="33"/>
 *				<sprite n="0005" x="901" y="0" w="55" h="22"/>
 *				<sprite n="0006" x="956" y="0" w="55" h="22"/>
 *			</TextureAtlas>
 */

static void* handle(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(customData != NULL);
		
	const char *fileExt = YCFileUtil::GetFileExt(filename);
	if (fileExt == NULL)
	{
		LOG_WARNING("YCTextureFactory::notify未知文件名：" << filename);
		return NULL;
	}

	YCITexture2D* texture = NULL;
	if (strcmp(fileExt, "xml") == 0)
	{
		std::string strFilename(filename);
		if (strFilename.find("texture.xml") != std::string::npos)
		{
			TEXTURE_CUSTOMDATA_T* textureData = (TEXTURE_CUSTOMDATA_T*)customData;
			return textureData->factory->loadTextureInAtlasXML(textureData->type, 
				textureData->uniqueId, textureData->initialPath, filename);
		}
	}
	else
	{
		LOG_WARNING("YCTextureFactory::notify未知文件类型：" << fileExt);
	}

	return NULL;
}

#pragma endregion XmlReader

YCTextureFactory::YCTextureFactory(void)
	: myTextureManager(NULL)
	, myFileLoader(NULL)
	, myNetworkLoader(NULL)
{
}


YCTextureFactory::~YCTextureFactory(void)
{
	delete myFileLoader;
}

//
// 函数：initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig* config)
//
// 目的：配置工厂类，如WebServer Address, 并发连接数，等待超时...
//
bool YCTextureFactory::initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig& config)
{
	SMART_ASSERT(textureManager != NULL);
	SMART_ASSERT(networkLoader != NULL);

	//获取当前工作目录
	char theExePath[MAX_PATH] = {0};
	if (!YCFileUtil::GetExePath(theExePath, MAX_PATH))
	{
		return false;
	}

	//TEXTURE_MAP              =  0,		// 地表
	//TEXTURE_SHAREDMAP        =  1,        // 共享地表
	//TEXTURE_OBSTACLE		   =  2,		// 建筑及树木遮挡
	//TEXTURE_NPC              =  3,		// NPC
	//TEXTURE_PLAYER           =  4,		// 玩家
	//TEXTURE_MONSTER          =  5,		// 怪物
	//TEXTURE_EFFECT           =  6,		// 魔法特效
	//TEXTURE_UI               =  7,		// 界面
	const char* texturePath = config.tileMapPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::TileMap路径配置有误!");
	}
	ZeroMemory(myTileMapPath, MAX_PATH);
	sprintf_s(myTileMapPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.sharedMapPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::SharedMap路径配置有误!");
	}
	ZeroMemory(mySharedMapPath, MAX_PATH);
	sprintf_s(mySharedMapPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.obstaclePath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::Obstacle路径配置有误!");
	}
	ZeroMemory(myObstaclePath, MAX_PATH);
	sprintf_s(myObstaclePath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.npcPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::NPC路径配置有误!");
	}
	ZeroMemory(myNPCPath, MAX_PATH);
	sprintf_s(myNPCPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.playerPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::Player路径配置有误!");
	}
	ZeroMemory(myPlayerPath, MAX_PATH);
	sprintf_s(myPlayerPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.monsterPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::Monster路径配置有误!");
	}
	ZeroMemory(myMonsterPath, MAX_PATH);
	sprintf_s(myMonsterPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.effectPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::Effect路径配置有误!");
	}
	ZeroMemory(myEffectPath, MAX_PATH);
	sprintf_s(myEffectPath, "%s\\..\\%s", theExePath, texturePath);

	texturePath = config.uiPath();
	if (strlen(texturePath) >= MAX_PATH)
	{
		throw YCException(2002, "YCConfig::UI路径配置有误!");
	}
	ZeroMemory(myUIPath, MAX_PATH);
	sprintf_s(myUIPath, "%s\\..\\%s", theExePath, texturePath);
	
	myFileLoader = new YCFileTextureLoader();
	myFileLoader->configure(config);

	myNetworkLoader = networkLoader;

	myTextureManager = textureManager;

	return true;
}

//
// 函数：fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename)
//
// 目的：获取纹理
//
YCITexture2D* YCTextureFactory::fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename)
{
	char fullFileName[MAX_PATH] = {0};
	const char *basePath = getPathByType(type);
	sprintf_s(fullFileName, MAX_PATH, "%s\\%s", basePath, filename);
	LOG_DEBUG("YCTextureFactory::fetch加载纹理：" << fullFileName);

	YCITexture2D* texture = NULL;

	try
	{
		YCITextureFile *textureFile = NULL;
		if (YCFileUtil::IsFileExist(fullFileName))   //尝试单纹理
		{
			textureFile = myFileLoader->load(fullFileName);
			if (textureFile != NULL)
			{
				texture = new YCSingleTexture2D(textureFile, YCITexture2D::SUCC, uniqueId);
			}
		}
		else //尝试Atlas纹理
		{
			char path[MAX_PATH] = {0};
			if (!YCFileUtil::GetFilePath(fullFileName, path, MAX_PATH))
			{
				return NULL;
			}

			if (YCFileUtil::IsFileExist(path))
			{
				std::string pathWithXMLFilter(path);
				pathWithXMLFilter += "\\*texture.xml";

				TEXTURE_CUSTOMDATA_T customData;
				customData.factory = this;
				customData.uniqueId = uniqueId;
				customData.type = type;
				YCFileUtil::GetFilePath(filename, customData.initialPath, MAX_PATH);

				texture = (YCITexture2D*)YCFileUtil::HandleFileInDir(pathWithXMLFilter.c_str(), handle, &customData); 
			}
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCTextureFactory::fetch加载纹理失败: " << e.what());
	}

	// 提交网络请求
	if (texture == NULL)
	{
		myNetworkLoader->request(this, filename);		
		texture = new YCSingleTexture2D(NULL, YCITexture2D::FAKE, uniqueId);  //stub
	}
	
	return texture;
}

//
// 函数：finalize()
//
// 目的：关闭线程池，释放相关资源
//
void YCTextureFactory::finalize()
{
	
}

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
YCITexture2D* YCTextureFactory::loadTextureInAtlasXML(E_TEXTURE_TYPE type, int uniqueId, const char* path, const char* xmlFile)
{
	SMART_ASSERT(path != NULL);
	SMART_ASSERT(xmlFile != NULL);

	char xmlFullPath[MAX_PATH] = {0};
	const char* basePath = getPathByType(type);
	sprintf_s(xmlFullPath, "%s\\%s\\%s", basePath, path, xmlFile);
	
	YCITexture2D* texture = NULL;
	char* imageFile = NULL;
	YCDList* idList = new YCDList();
	
	//<TextureAtlas imagePath="texture.png" width="128" height="256">
    //    <sprite n="00" x="2" y="126" w="62" h="122"/>
    //    <sprite n="01" x="2" y="2" w="62" h="122"/>
    //</TextureAtlas>

	try 
	{
		ATLAS_CUSTOMDATA_T atlasData;
		atlasData.idList = idList;
		atlasData.type = type;
		atlasData.initialPath = path;

		YCXmlReader reader(xmlFullPath);
		reader.attr("imagePath", &imageFile)
			  .foreach("sprite", YCXmlReader::YCForEachWrapper1<YCTextureFactory, ATLAS_CUSTOMDATA_T>(this, &YCTextureFactory::loadSpriteInAtlasTexture, &atlasData));
	}
	catch (YCException& e)
	{
		delete imageFile;
		delete idList;
		throw e;
	}

	// 读取纹理文件
	char imageFullPath[MAX_PATH] ={0};
	sprintf_s(imageFullPath, "%s\\%s\\%s", basePath, path, imageFile);
	YCITextureFile * textureFile = myFileLoader->load(imageFullPath);;
	if (textureFile != NULL)
	{
		texture = new YCAtlasTexture2D(idList, textureFile, YCITexture2D::SUCC);
	}
	return texture;
}

//
// 函数：loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData)
//
// 目的：读取Altas纹理集合中的每一个纹理
//
// 注释：
//      <sprite n="00" x="2" y="126" w="62" h="122"/>
//
bool YCTextureFactory::loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData)
{
	SMART_ASSERT(atlasData != NULL);
	ATLAS_TEXTURE_INFO_T* texture_info = NULL;
	try
	{
		texture_info = new ATLAS_TEXTURE_INFO_T;

		int frameNo;
		reader.attr("n", frameNo)
			  .attr("x", texture_info->x)
			  .attr("y", texture_info->y)
			  .attr("w", texture_info->w)
			  .attr("h", texture_info->h);

		char relationPath[MAX_PATH] = {0};
		sprintf_s(relationPath, "%s\\%d.dummy", atlasData->initialPath, frameNo);
		texture_info->id = YCTextureIDUtil::encode(atlasData->type, relationPath);		
	}
	catch (YCException& e)
	{
		//LOG_WARNING("YCTextureFactory::loadSpriteInAtlasTexture读取Atlas纹理文件失败:"<<reader.getFilename());
		delete texture_info;
		throw e;
	}

	if (texture_info != NULL)
	{
		atlasData->idList->append(texture_info, NULL);
	}
	return true;
}

//
// 函数：notify(const char* filename, bool result)
//
// 目的：下载文件结果通知
//
// 参数：
//       filename : 下载的文件名
//       result   ：下载结果
//
void YCTextureFactory::notify(const char* filename, bool result)
{

}


//////////////////////////////////////////////////////////////////////////////
// Private Part
//////////////////////////////////////////////////////////////////////////////

//
// 函数：getPathByType(E_TEXTURE_TYPE type)
//
// 目的：取得纹理路径
//
const char* YCTextureFactory::getPathByType(E_TEXTURE_TYPE type)
{
	if      (type == TEXTURE_MAP      ) return myTileMapPath;
	else if (type == TEXTURE_SHAREDMAP) return mySharedMapPath;
	else if (type == TEXTURE_OBSTACLE ) return myObstaclePath;
	else if (type == TEXTURE_NPC      ) return myNPCPath;
	else if (type == TEXTURE_PLAYER   ) return myPlayerPath;
	else if (type == TEXTURE_MONSTER  ) return myMonsterPath;
	else if (type == TEXTURE_EFFECT   ) return myEffectPath;
	else if (type == TEXTURE_UI       ) return myUIPath;
	else throw YCException(2002, "YCTextureFactory::getPathByType纹理type不正确!");
	return NULL;
}