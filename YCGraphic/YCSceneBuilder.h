#ifndef _INCLUDE_YCSCENEBUILDER_H_
#define _INCLUDE_YCSCENEBUILDER_H_

class YCIMap;
class YCScene;
class YCGraphic;

/*
 * 场景构建器，Builder(Factory) Pattern
 */
class YCSceneBuilder
{
public:

	/*
	 * 创建登录场景
	 */
	static YCScene *buildLoginScene(YCGraphic* graphic);

	/*
	 * 创建角色选择场景
	 */
	static YCScene *buildRoleScene(YCGraphic* graphic);

	/*
	 * 创建游戏场景
	 */
	static YCScene *buildGameScene(YCGraphic* graphic, YCIMap* map);

	/*
	 * 创建退出场景
	 */ 
	static YCScene *buildQuitScene(YCGraphic* graphic);
};

#endif