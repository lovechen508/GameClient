#ifndef _INCLUDE_YCTEXTUREID_UTILITY_H_
#define _INCLUDE_YCTEXTUREID_UTILITY_H_

#include "YCTextureDef.h"

#define GET_TEXTURE_TYPE(n) (int)(n&15)

/*
 * 纹理唯一ID辅助计算类, 不可继承
 *
 *  全部为内联静态函数
 */
class YCTextureIDUtil
{
public:

	static unsigned int encode(E_TEXTURE_TYPE type, const char* filename);

	static bool buildPackageName(E_TEXTURE_TYPE type, const char* filename, char* buf, int len);

private:

	//
	// 函数：encodeTilesMAPId(const char* filename)
	// 参数: 
	//       mapId			mapId, 全局唯一
	//
	// 目的：计算大地表纹理唯一ID 	
	//
	static unsigned int encodeTilesMapId(const char* filename);
	
	//
	// 函数：encodeSharedMapId(const char* filename)
	// 参数: 
	//       fileNo			[IN]  在/data/map/shared文件夹下面的PNG文件
	//
	// 目的：计算共享地表纹理唯一ID 	
	//
	static unsigned int encodeSharedMapId(const char* filename);

	//
	// 函数：encodeObstacleId(const char* filename)
	// 
	// 参数: 
	//       obstacleId		obstacleId, 全局唯一
	//       direction		方向
	//       frameNo        当前帧号
	//
	// 目的：计算建筑,树木等遮挡物体纹理唯一ID 	
	//
	static unsigned int encodeObstacleId(const char* filename);

	//
	// 函数：encodeNPCId(const char* filename)
	// 参数: 
	//       npcId			NPCId, 全局唯一
	//       composite      身体部位
	//       direction		方向
	//       state          NPC状态：1, 站立　2, 行走
	//       frameNo        当前帧号
	//
	// 目的: 生成NPC唯一纹理ID
	//
	static unsigned int encodeNPCId(const char* filename);

	//
	// 函数：encodePlayerId(const char* filename)
	// 参数: 
	//      composite		compositeType, 全局唯一 YCTextureDef::E_OBJECT_COMPOSITE_TYPE
	//      composityId     在该compositeType下面的compositeId，例如鞋子类型下的雪地靴等，刀类型下的XX宝刀      
	//      state           人物状态：1, 站立　2, 行走　3, 奔跑, 攻击...
	//      direction		方向
	//      frameNo         当前帧号
	//
	// 注释：
	//	     目录结构：/data/object/player/compositetype/composityId/state/direction/frameNo.png(tga)
	//
	// 目的: 生成Player唯一纹理ID
	//
	static unsigned int encodePlayerId(const char* filename);

	//
	// 函数：encodeMonsterId(const char* filename)
	// 参数: 
	//       monsterId  	MonsterId, 全局唯一
	//　　　 direction		方向
	//       state          NPC状态：1, 站立　2, 行走, 攻击 ...
	//       frameNo        当前帧号
	//
	// 目的: 生成Monster唯一纹理ID
	//
	static unsigned int encodeMonsterId(const char* filename);

	//
	// 函数：encodeEffectId(const char* filename)
	// 参数: 
	//       effectId		effectId, 全局唯一
	//       direction		方向
	//       frameNo        当前帧号
	//
	// 目的: 生成Effect唯一纹理ID
	//
	static unsigned int encodeEffectId(const char* filename);

	//
	// 函数：encodeUIId(const char* filename)
	// 参数: 
	//       windowId		窗口Id
	//       pic			picID
	//
	// 目的: 生成对应UI唯一纹理ID
	//
	static unsigned int encodeUIId(const char* filename);
};

#endif