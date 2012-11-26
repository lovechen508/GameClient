#ifndef _INCLUDE_YCSCENEMANAGER_H_
#define _INCLUDE_YCSCENEMANAGER_H_

#include "YCBasic\YCSingleton.h"

class YCScene;
class YCGraphic;
class YCSceneBuilder;
class YCIMapFactory;

struct YCIPackageResponse;

/*
 * YCSceneManager : ����������
 *                  ��ʵ�ʵ���Ϸ�У��������л���ͨ��������Ϣ������
 */
class YCSceneManager 
	: public YCSingleton<YCSceneManager>
{
public:

	YCSceneManager(YCGraphic* graphic);

	~YCSceneManager();

	//
	// ������registerMapFactory(YCIMapFactory* mapFactory)
	//
	// Ŀ�ģ�ע���ͼ����
	//
	void registerMapFactory(YCIMapFactory* mapFactory);

	//
	// ������requireScene(YCIPackageResponse* response)
	//
	// Ŀ�ģ����󳡾�
	//
	YCScene* requireScene(YCIPackageResponse* response);

	//
	// ������releaseScene(YCScene* scene)
	//
	// Ŀ�ģ��ͷų���
	//
	void releaseScene(YCScene* scene);

private:

	//
	// ���ԣ�ͼ������
	//
	YCGraphic *myGraphic;

	//
	// ���ԣ�myMapFactory
	//
	// Ŀ�ģ���ͼ��������������������
	//
	YCIMapFactory *myMapFactory;
};

#endif