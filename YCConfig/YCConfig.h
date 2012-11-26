#ifndef _INCLUDE_YCCONFIG_H_
#define _INCLUDE_YCCONFIG_H_

#define DEFAULT_MODULE_NUM 50

#include "YCBasic\YCLogger.h"

class YCXmlReader;

/*
 * YC_MODULE_T ϵͳ����ģ��
 *
 * ���� ��
 *        name       ģ������
 *        version    ģ��汾
 *        state      ģ��״̬
 */
typedef struct _module
{
	const char*  name;
	unsigned int version;
	bool         state;
}YC_MODULE_T;

/*
 * YCConfig�����ͻ������ü��ؽӿ���
 *             Template Pattern
 *
 * ע�ͣ�Ĭ������¼��� network/texture/log/module
 *
 * �����ļ��ṹ�� configure -> network
 *                       | -> texture
 *                       | -> log
 *                       | -> module
 *                       | -> ...
 *                       | -> �ͻ����Զ������ã�onLoad�麯���ص���
 */
class YCAPI YCConfig
{
public:

	YCConfig(void);

	virtual ~YCConfig(void);

	//
	// ������load(const char* filename)
	//
	// Ŀ�ģ����ؿͻ���������Ϣ
	//
	// ע�ͣ��̳��಻�ܸ��Ǵ˺���
	//       ��������Զ�����Ϣ����ʵ��onLoad����
	//       �ڴ˺������������֮
	//
	bool load(const char* filename);
	
	//
	// ������loadTextureDir(YCXmlReader&)
	//
	// Ŀ�ģ���ȡ����Ŀ¼����
	//
	bool loadTextureDir(YCXmlReader&);

	//
	// ������loadChatColor(YCXmlReader& reader)
	//
	// Ŀ�ģ�������ɫ��Ϣ
	//
	bool loadChatColor(YCXmlReader& reader);

	//
	// ������loadModule(YCXmlReader&)
	//
	// Ŀ�ģ���ȡģ������
	//
	bool loadModule(YCXmlReader&);

	//
	// �����飺������Ļ������Ϣ
	//
	int width();
	int height();
	int fps();

	//
	// �����飺����network������Ϣ
	//
	const char* loginServer();
	int port();

	// ��������ȡ����ش��ʱ��
	unsigned int durationSeconds();

	//
	// �����飺����texture���ط�����������Ϣ
	//
	const char * downloadServerURL();
	int noOfConnections();
	int maxItemsInQueue();

	//
	// �����飺����data������Ϣ
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
	// ������������־������Ϣ
	//
	YCLogger::LOG_LEVEL logLevel();

	//
	// �����飺����Magic������Ϣ
	//
	const char * magicFile();

	//
	// �����飺����Chat������Ϣ
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
	// ����������UI ���ı�ǩ��Ϣ
	// 
	const char * coreTagPath();

private:

	//
	// ������onLoad(YCXmlReader& root)
	//
	// Ŀ�ģ����ؿͻ����Զ�������
	//
	// ע�ͣ� 
	//     E.g. �����ļ��ṹ�� configure -> network
	//                                | -> texture
	//                                | -> log
    //                                | -> ...
    //                                | -> fps             �ͻ����Զ������ã�onLoad�麯���ص���
	//
	//     ��ȡ����Ϊ��     root.enter("fps")
	//                             .value(fps)
	//                         .leave();
	//
	virtual bool onLoad(YCXmlReader& root);

private:

	// �����ļ�����·��
	char myConfigFile[MAX_PATH];

	// ��Ļ����
	int myWindowWidth;
	int myWindowHeight;
	int myFPS;

	// ��������
	char * myLoginServer;
	int myPort;

	// ����ؿ��д��ʱ��
	int myDurationSeconds;

	// Texture���ط���������
	char * myDownloadServerURL;
	int myNoOfConnections;
	int myMaxItemsInQueue;

	// ��ԴĿ¼����
	char * myTileMapPath;
	char * mySharedMapPath;
	char * myObstaclePath;
	char * myNPCPath;
	char * myPlayerPath;
	char * myMonsterPath;
	char * myEffectPath;
	char * myUIPath;
	char * mySoundPath;

	// Log��������
	int myLogLevel;

	// ������Ϣ
	char * myMagicFile;

	// ��������
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

	// Buff����
	char * myBuffConfig;

	// ����Tag����
	char * myCoreTagPath;

	// ϵͳ����ģ���б�
	YC_MODULE_T myModuleArray[DEFAULT_MODULE_NUM];

};

#endif