#include "YCSceneBuilder.h"

#include <memory>

#include "YCScene.h"
#include "YCGraphic.h"
#include "YCUILayer.h"
#include "YCMapLayer.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCException.h"

/*
 * ������¼����
 */
YCScene* YCSceneBuilder::buildLoginScene(YCGraphic* graphic)
{
	return NULL;
}

/*
 * ������ɫѡ�񳡾�
 */
YCScene* YCSceneBuilder::buildRoleScene(YCGraphic* graphic)
{
	return NULL;
}

/*
 * ������Ϸ����
 */
YCScene* YCSceneBuilder::buildGameScene(YCGraphic* graphic, YCIMap* map)
{
	SMART_ASSERT(map != NULL);

	std::auto_ptr<YCScene> scene(new YCScene(graphic));
	if (!scene->initialize())
	{
		throw YCException(2002, "YCSceneBuilder::buildGameScene��ʼ������ʧ��!");
	}

	// ��ӵ�ͼ��
	YCISceneLayer * layer = NULL;
	layer = new YCMapLayer(map);
	scene->addLayer(layer);

	// ��ӽ����
	layer = new YCUILayer();
	scene->addLayer(layer);
	return scene.release();
}

/*
 * �����˳�����
 */ 
YCScene* YCSceneBuilder::buildQuitScene(YCGraphic* graphic)
{
	return NULL;
}