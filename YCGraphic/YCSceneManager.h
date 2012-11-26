#ifndef _INCLUDE_YCSCENEMANAGER_H_
#define _INCLUDE_YCSCENEMANAGER_H_

#include "YCBasic\YCSingleton.h"

class YCScene;
class YCGraphic;
class YCSceneBuilder;
class YCIMapFactory;

struct YCIPackageResponse;

/*
 * YCSceneManager : 场景管理器
 *                  在实际的游戏中，场景的切换是通过网络消息驱动的
 */
class YCSceneManager 
	: public YCSingleton<YCSceneManager>
{
public:

	YCSceneManager(YCGraphic* graphic);

	~YCSceneManager();

	//
	// 函数：registerMapFactory(YCIMapFactory* mapFactory)
	//
	// 目的：注册地图工厂
	//
	void registerMapFactory(YCIMapFactory* mapFactory);

	//
	// 函数：requireScene(YCIPackageResponse* response)
	//
	// 目的：请求场景
	//
	YCScene* requireScene(YCIPackageResponse* response);

	//
	// 函数：releaseScene(YCScene* scene)
	//
	// 目的：释放场景
	//
	void releaseScene(YCScene* scene);

private:

	//
	// 属性：图形引擎
	//
	YCGraphic *myGraphic;

	//
	// 属性：myMapFactory
	//
	// 目的：地图工厂，不持有生命周期
	//
	YCIMapFactory *myMapFactory;
};

#endif