#include "YCFileTextureLoader.h"

#include "d3d9.h"

#include "YCTextureDef.h"
#include "YCITexture2D.h"
#include "YCPNGTextureFile.h"
#include "YCTGATextureFile.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCFileUtil.h"
#include "YCConfig\YCConfig.h"
#include "YCBasic\YCException.h"

YCFileTextureLoader::YCFileTextureLoader()
	: myLPDevice(NULL)
{

}

YCFileTextureLoader::~YCFileTextureLoader()
{

}
	
//
// 函数：configure()
//
// 目的：加载纹理相关配置
//
void YCFileTextureLoader::configure(YCConfig& config)
{
	myLPDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
	if (myLPDevice == NULL)
	{
		throw YCException(2002, "YCFileTextureLoader::configure无法从YCRegistry中获取LPDIRECT3DDEVICE9句柄，请确认注册D3DDevice！");
	}
}

//
// 函数：load(int uniqueId)
//
// 目的：加载纹理接口实现函数
//
YCITextureFile* YCFileTextureLoader::load(const char* filename)
{
	if (myLPDevice == NULL)
	{
		throw YCException(2002, "YCFileTextureLoader::loade无法从YCRegistry中获取LPDIRECT3DDEVICE9句柄，请确认注册D3DDevice！");
	}

	YCITextureFile* texture = NULL;
	try {
		const char* fileExt = YCFileUtil::GetFileExt(filename);
		if (fileExt != NULL)
		{
			if (strcmp("png", fileExt) == 0) 
			{
				texture = new YCPNGTextureFile(filename);
			}
			else if (strcmp("tga", fileExt) == 0)
			{
				texture = new YCTGATextureFile(filename);
			}
			else 
			{
				throw YCException(2002, "YCFileTextureLoader::load无效纹理类型");
			}

			if (!texture->load(myLPDevice))
			{
				throw YCException(2002, "YCFileTextureLoader::load加载纹理失败");
			}
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCFileTextureLoader::load加载纹理失败: " << e.what());
	}

	return texture;
}