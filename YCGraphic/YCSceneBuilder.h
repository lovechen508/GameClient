#ifndef _INCLUDE_YCSCENEBUILDER_H_
#define _INCLUDE_YCSCENEBUILDER_H_

class YCIMap;
class YCScene;
class YCGraphic;

/*
 * ������������Builder(Factory) Pattern
 */
class YCSceneBuilder
{
public:

	/*
	 * ������¼����
	 */
	static YCScene *buildLoginScene(YCGraphic* graphic);

	/*
	 * ������ɫѡ�񳡾�
	 */
	static YCScene *buildRoleScene(YCGraphic* graphic);

	/*
	 * ������Ϸ����
	 */
	static YCScene *buildGameScene(YCGraphic* graphic, YCIMap* map);

	/*
	 * �����˳�����
	 */ 
	static YCScene *buildQuitScene(YCGraphic* graphic);
};

#endif