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
// ������configure()
//
// Ŀ�ģ����������������
//
void YCFileTextureLoader::configure(YCConfig& config)
{
	myLPDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
	if (myLPDevice == NULL)
	{
		throw YCException(2002, "YCFileTextureLoader::configure�޷���YCRegistry�л�ȡLPDIRECT3DDEVICE9�������ȷ��ע��D3DDevice��");
	}
}

//
// ������load(int uniqueId)
//
// Ŀ�ģ���������ӿ�ʵ�ֺ���
//
YCITextureFile* YCFileTextureLoader::load(const char* filename)
{
	if (myLPDevice == NULL)
	{
		throw YCException(2002, "YCFileTextureLoader::loade�޷���YCRegistry�л�ȡLPDIRECT3DDEVICE9�������ȷ��ע��D3DDevice��");
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
				throw YCException(2002, "YCFileTextureLoader::load��Ч��������");
			}

			if (!texture->load(myLPDevice))
			{
				throw YCException(2002, "YCFileTextureLoader::load��������ʧ��");
			}
		}
	}
	catch (YCException& e)
	{
		LOG_WARNING("YCFileTextureLoader::load��������ʧ��: " << e.what());
	}

	return texture;
}