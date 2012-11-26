#ifndef _INCLUDE_YCMAPBUILDER_H_
#define _INCLUDE_YCMAPBUILDER_H_

class YCTiledMap;

//
// YCMapBuilder: 地图构造器
//
// 说明：地图构造器
//       所有地图必须放在YCConfig中TiledMap配置的目录下面
//       常规地图ID从1开始。。。
//       目录下的0号文件夹存放共享图片，如阻挡点图块等
//       所有图片必须以数字存放，便于YCTextureManager统一管理纹理
//
struct YCMapBuilder
{
	//
	// 函数：build(unsigned int mapId, const char* filename)
	//
	// 目的：构造地图对象
	//
	static YCTiledMap* build(unsigned int mapId, const char* filename);

};

#endif

