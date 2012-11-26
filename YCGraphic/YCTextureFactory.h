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
 * YCTextureFactory ��������ع�����
 *
 * ע�ͣ�
 *      �����������أ����أ���YCTextureManager���������Ӳ���洢ϸ��
 */
class YCTextureFactory
	: public YCNetworkFileLoadListener
{
public:
	
	YCTextureFactory(void);
	
	~YCTextureFactory(void);

	//
	// ������initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig* config)
	//
	// Ŀ�ģ����ù����࣬��WebServer Address, �������������ȴ���ʱ...
	//
	bool initialize(YCTextureManager* textureManager, YCNetworkFileLoader* networkLoader, YCConfig& config);

	//
	// ������fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename)
	//
	// Ŀ�ģ���ȡ����
	//
	YCITexture2D* fetch(E_TEXTURE_TYPE type, int uniqueId, const char* filename);

	//
	// ������finalize()
	//
	// Ŀ�ģ��ر��̳߳أ��ͷ������Դ
	//
	void finalize();

	//
	// ������getBaseURL()
	//
	// Ŀ�ģ�ȡ��BaseURL
	//
	const char* getBaseURL() const;

	//
	// ������notify(const char* filename, bool result)
	//
	// Ŀ�ģ������ļ����֪ͨ
	//
	// ������
	//       filename : ���ص��ļ���
	//       result   �����ؽ��
	//
	virtual void notify(const char* filename, bool result);

	//
	// ������loadTextureInAtlasXML(E_TEXTURE_TYPE type, int uniqueId, const char* xmlFile)
	//
	// Ŀ�ģ�����Atlas����XML�����ļ�
	//
	// ������
	//
	//     uniqueId : ��������uniqueId
	//     url      : �����ύ��url����
	//     xmlFile  : Atlas XML�ļ���
	//
	YCITexture2D* loadTextureInAtlasXML(E_TEXTURE_TYPE type, int uniqueId, const char* path, const char* xmlFile);

public:

	//
	// ������loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData)
	//
	// Ŀ�ģ���ȡAltas�������е�ÿһ������
	//
	bool loadSpriteInAtlasTexture(YCXmlReader& reader, ATLAS_CUSTOMDATA_T* atlasData);

private:

	//
	// ������getPathByType(E_TEXTURE_TYPE type)
	//
	// Ŀ�ģ�ȡ������·��
	//
	const char* getPathByType(E_TEXTURE_TYPE type);

private:

	//�����Դ·��
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

