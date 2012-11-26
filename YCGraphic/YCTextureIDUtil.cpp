#include "YCTextureIDUtil.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCStringHelper.h"

#include "YCBasic\YCAssert.h"
#include <vector>

#pragma pack(push,1)

union __TileMap {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;   // E_TEXTURE_TYPE
		unsigned int mapId:8;    // 地表Id 上限256张大地图
		unsigned int picId:12;   // 图片ID, [1, 4093] 支持4093种控件纹理
		unsigned int reserved:8; // 保留
	} composite;
};

union __Obstacle {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		  // E_TEXTURE_TYPE
		unsigned int obstacleId:10;   // ObstacleId [1 .. 1023]
		unsigned int direction:3;     // direction [0,7]
		unsigned int frameNo:5;       // 动画[0,31] 最大31帧
		unsigned int reserved:10;     // 保留
	} composite;
};

union __NPC {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int npcId:9;       // npcId [1..511]
		unsigned int composite:8;   // composite
		unsigned int direction:3;   // direction [0,7]
		unsigned int state:4;       // state [0,15]
		unsigned int frameNo:4;     // 动画[0,15] 最大16帧
	} composite;
};

union __Player {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		    // E_TEXTURE_TYPE
		unsigned int compositeType:6;   // compositeType
		unsigned int compositeId:9;     // compositeId
		unsigned int direction:3;       // direction [0,7]
		unsigned int state:5;           // state [0,31]
		unsigned int frameNo:5;         // 动画[0,31] 最大31帧
	} composite;
};

union __Monster {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int monsterId:12;  // 怪物ID, [1, 4095] 支持4095种怪物
		unsigned int direction:3;   // direction [0,7]
		unsigned int state:5;       // state [0,31]
		unsigned int frameNo:5;     // 动画[0,31] 最大31帧
		unsigned int reserved:3;    // 保留
	} composite;
};

union __Effect {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int effectId:14;   // 特效ID, [1, 4095] 支持4095种特效
		unsigned int direction:3;   // direction [0,7]
		unsigned int frameNo:6;     // 动画[0,63] 最大64帧
		unsigned int reserved:5;    // 保留
	} composite;
};

union __UI {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int windowId:8;    // 窗口编号, [0, 255] 支持256种窗口 0号为shared共享图片纹理
		unsigned int picId:12;      // 图片ID, [1, 4093] 支持4093种控件纹理
		unsigned int reserved:8;    // 保留
	} composite;
};

#pragma pack(pop)

//
// 函数：decode(int uniqueId, int& param1, int& param2, int& param3, int& param4, int& param5, int& param6)
//
// 参数：
//       uniqueId     [IN]   全局唯一纹理ID
//       param1       [OUT]  参数一，应该是typeId
//       param2       [OUT]  参数二
//       param3       [OUT]  参数三
//       param4       [OUT]  参数四
//       param5       [OUT]  参数五
//       param6       [OUT]  参数六
//
// 注释：
//       返回的参数有多有少，有些起始值可以是0，所以第一个小于零的表示有效参数结束
//       因为第一个表示typeId，举例如下：
//           返回值为:
//                    param1 = 3     在规定中3代表NPC   
//                    param2 = 18    npcId
//                    param3 = 0x47  compositeId 代表上衣 71
//                    param4 = 3     direction 代表3号方向
//                    param5 = 1     state 站立
//                    param6 = 13    frameNo 站立动画的第13帧
//           映射到纹理路径为：
//                   YCSingleTexture2D : data/object/npc/18/71/3/1/13.png 或13.lzx（加密纹理）
//                   YCAtlasTexture2D  : data/object/npc/18/71/3/1/texture.xml, texture.png(AltasTexture), textuer.lzx(加密纹理)
//
//void YCTextureIDUtil::decode(unsigned int uniqueId, int& param1, int& param2, int& param3, int& param4, int& param5, int& param6)
//{
//	param1 = GET_TEXTURE_TYPE(uniqueId);
//	param4 = param5 = param6 = -1;
//	if      (param1 == TEXTURE_MAP)      decodeTilesMapId(uniqueId, param2, param3, param4);
//	else if (param1 == TEXTURE_SMALLMAP) decodeSmallMapId(uniqueId, param2, param3, param4);
//	else if (param1 == TEXTURE_OBSTACLE) decodeObstacleId(uniqueId, param2, param3, param4);
//	else if (param1 == TEXTURE_NPC)      decodeNPCId(uniqueId, param2, param3, param4, param5, param6);
//	else if (param1 == TEXTURE_PLAYER)   decodePlayerId(uniqueId, param2, param3, param4, param5, param6);
//	else if (param1 == TEXTURE_MONSTER)  decodeMonsterId(uniqueId, param2, param3, param4, param5);
//	else if (param1 == TEXTURE_EFFECT)   decodeEffectId(uniqueId, param2, param3, param4);
//	else if (param1 == TEXTURE_UI)       decodeUIId(uniqueId, param2, param3, param4);
//	else                                 { SMART_ASSERT(false); throw YCException(2002, "未知纹理编号!"); }
//}

unsigned int YCTextureIDUtil::encode(E_TEXTURE_TYPE type, const char* filename)
{
	SMART_ASSERT(filename != NULL);

	if      (type == TEXTURE_MAP)       return encodeTilesMapId(filename);
	else if (type == TEXTURE_SHAREDMAP) return encodeSharedMapId(filename);
	else if (type == TEXTURE_OBSTACLE)  return encodeObstacleId(filename);
	else if (type == TEXTURE_NPC)       return encodeNPCId(filename);
	else if (type == TEXTURE_PLAYER)    return encodePlayerId(filename);
	else if (type == TEXTURE_MONSTER)   return encodeMonsterId(filename);
	else if (type == TEXTURE_EFFECT)    return encodeEffectId(filename);
	else if (type == TEXTURE_UI)        return encodeUIId(filename);
	else                                throw YCException(2002, "未知纹理编号!"); 

	return 0;
}

bool buildPackageName(E_TEXTURE_TYPE type, const char* filename, char* buf, int len)
{
	return true;
}

//
// 函数：encodeTilesMAPId(const char* filename)
// 参数: 
//       mapId			mapId, 全局唯一
//       x				X坐标
//       y			    Y坐标
//
// 目的：计算大地表纹理唯一ID 	
//
unsigned int YCTextureIDUtil::encodeTilesMapId(const char* filename)
{
	std::vector<std::string> dirs;
	YCStringHelper::split(filename, "//", &dirs);

	if (dirs.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId文件路径不对!");
	}

	if (!YCStringHelper::isType(dirs[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId文件路径名不对!");
	}

	std::vector<std::string> fileVector;
	YCStringHelper::split(dirs[1].c_str(), ".", &fileVector);
	if (fileVector.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId无效纹理文件!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId无效纹理文件主名!");
	}

	__TileMap tilemap;

	tilemap.composite.typeId = TEXTURE_MAP;
	tilemap.composite.mapId = atoi(dirs[0].c_str());
	tilemap.composite.picId = atoi(fileVector[0].c_str());
	tilemap.composite.reserved = 0;

	return tilemap.uniqueid;
}

//
// 函数：encodeSharedMapId(const char* filename)
// 参数: 
//       fileNo			[IN]  在/data/map/shared文件夹下面的PNG文件
//
// 目的：计算共享地表纹理唯一ID 	
//
unsigned int YCTextureIDUtil::encodeSharedMapId(const char* filename)
{
	return 0;
}

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
unsigned int YCTextureIDUtil::encodeObstacleId(const char* filename)
{
	//__Obstacle obstacle;

	//obstacle.composite.typeId = TEXTURE_OBSTACLE;
	//obstacle.composite.obstacleId = obstacleId;
	//obstacle.composite.direction = direction;
	//obstacle.composite.frameNo = frameNo;

	//return obstacle.uniqueid;
	return 0;
}

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
unsigned int YCTextureIDUtil::encodeNPCId(const char* filename)
{
	//__NPC npc;

	//npc.composite.typeId = TEXTURE_NPC;
	//npc.composite.npcId = npcId;
	//npc.composite.composite = composite;
	//npc.composite.direction = direction;
	//npc.composite.state = state;
	//npc.composite.frameNo = frameNo;

	//return npc.uniqueid;
	return 0;
}

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
//	     目录结构：\data\object\player\compositetype\composityId\state\direction\frameNo.png(tga)
//
// 目的: 生成Player唯一纹理ID
//
unsigned int YCTextureIDUtil::encodePlayerId(const char* filename)
{
	std::vector<std::string> dirs;

	// compositetype\composityId\state\direction\frameNo
	YCStringHelper::split(filename, "\\", &dirs);
	if (dirs.size() != 5)
	{
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId无效文件路径!");
	}

	for (int i = 0; i < 4; i++)
	{
		if (!YCStringHelper::isType(dirs[i].c_str(), YCStringHelper::POSITIVE_INTEGER))
		{
			throw YCException(2002, "YCTextureIDUtil::encodePlayerId文件路径名不对!");
		}
	}
	
	__Player player;

	player.composite.typeId = TEXTURE_PLAYER;
	player.composite.compositeType = atoi(dirs[0].c_str());
	player.composite.compositeId = atoi(dirs[1].c_str());
	player.composite.direction = atoi(dirs[2].c_str());
	player.composite.state = atoi(dirs[3].c_str());

	std::vector<std::string> fileVector;
	YCStringHelper::split(dirs[4].c_str(), ".", &fileVector);
	if (fileVector.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId无效纹理文件!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId无效纹理文件主名!");
	}

	player.composite.frameNo = atoi(fileVector[0].c_str());

	return player.uniqueid;
}

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
unsigned int YCTextureIDUtil::encodeMonsterId(const char* filename)
{
	//__Monster monster;

	//monster.composite.typeId = TEXTURE_MONSTER;
	//monster.composite.monsterId = monsterId;
	//monster.composite.direction = direction;
	//monster.composite.state = state;
	//monster.composite.frameNo = frameNo;

	//return monster.uniqueid;
	return 0;
}

//
// 函数：encodeEffectId(const char* filename)
// 参数: 
//       effectId		effectId, 全局唯一
//       direction		方向
//       frameNo        当前帧号
//
// 目的: 生成Effect唯一纹理ID
//
unsigned int YCTextureIDUtil::encodeEffectId(const char* filename)
{
	//__Effect effect;

	//effect.composite.typeId = TEXTURE_EFFECT;
	//effect.composite.effectId = effectId;
	//effect.composite.direction = direction;
	//effect.composite.frameNo = frameNo;

	//return effect.uniqueid;
	return 0;
}

//
// 函数：encodeUIId(const char* filename)
// 参数: 
//       windowId		窗口ID
//       pic			picID
//
// 目的: 生成对应UI唯一纹理ID
//
unsigned int YCTextureIDUtil::encodeUIId(const char* filename)
{
	std::vector<std::string> dirs;
	YCStringHelper::split(filename, "\\", &dirs);

	if (dirs.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId文件路径不对!");
	}

	if (!YCStringHelper::isType(dirs[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId文件路径名不对!");
	}

	std::vector<std::string> fileVector;
	YCStringHelper::split(dirs[1].c_str(), ".", &fileVector);
	if (fileVector.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId无效纹理文件!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId无效纹理文件主名!");
	}

	__UI ui;
	
	ui.composite.typeId = TEXTURE_UI;
	ui.composite.windowId = atoi(dirs[0].c_str());
	ui.composite.picId = atoi(fileVector[0].c_str());
	ui.composite.reserved = 0;

	return ui.uniqueid;
}

#pragma region decode
/*
//
// 函数：decodeTilesMAPId(unsigned int uniqueId, int& mapId, int& x, int& y)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       mapId			[OUT]  mapId, 全局唯一
//       x				[OUT]  X坐标
//       y			    [OUT]  Y坐标
//
// 目的：计算大地表纹理唯一ID 	
//
void YCTextureIDUtil::decodeTilesMapId(unsigned int uniqueId, int& mapId, int& x, int& y)
{
}
	
//
// 函数：decodeSmallMAPId(unsigned int uniqueId, int& mapId, int& x, int& y)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       mapId			[OUT]  mapId, 全局唯一
//       x				[OUT]  X坐标
//       y			    [OUT]  Y坐标
//
// 目的：计算小地表纹理存储路径 	
//
void YCTextureIDUtil::decodeSmallMapId(unsigned int uniqueId, int& mapId, int& x, int& y)
{
}

//
// 函数：decodeObstacleId(unsigned int uniqueId, int& obstacleId, int& direction, int& frameNo)
// 
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       obstacleId		[OUT] obstacleId, 全局唯一
//       direction		[OUT] 方向
//       frameNo        [OUT] 当前帧号
//
// 目的：计算建筑,树木等遮挡物体纹理唯一ID 	
//
void YCTextureIDUtil::decodeObstacleId(unsigned int uniqueId, int& obstacleId, int& direction, int& frameNo)
{
}

//
// 函数：decodeNPCId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       npcId			NPCId, 全局唯一
//       composite      身体部位
//       direction		方向
//       state          NPC状态：1, 站立　2, 行走
//       frameNo        当前帧号
//
// 目的: 生成NPC唯一纹理ID
//
void YCTextureIDUtil::decodeNPCId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
{
}

//
// 函数：decodePlayerId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       composite		compositeId, 全局唯一
//　　　 direction		方向
//       state          人物状态：1, 站立　2, 行走　3, 奔跑, 攻击...
//　　　 frameNo        当前帧号
//
// 目的: 生成Player唯一纹理ID
//
void YCTextureIDUtil::decodePlayerId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
{
}

//
// 函数：decodeMonsterId(unsigned int uniqueId, int& monsterId, int& direction, int& state, int& frameNo)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       monsterId  	[OUT]  MonsterId, 全局唯一
//　　　 direction		[OUT]  方向
//       state          [OUT]  NPC状态：1, 站立　2, 行走, 攻击 ...
//       frameNo        [OUT]  当前帧号
//
// 目的: 生成Monster唯一纹理ID
//
void YCTextureIDUtil::decodeMonsterId(unsigned int uniqueId, int& monsterId, int& direction, int& state, int& frameNo)
{
}

//
// 函数：decodeEffectId(int effectId, int direction, int frameNo)
// 参数: 
//       uniqueId       [IN]   纹理唯一ID
//       effectId		[OUT]  effectId, 全局唯一
//       direction		[OUT]  方向
//       frameNo        [OUT]  当前帧号
//
// 目的: 生成Effect唯一纹理ID
//
void YCTextureIDUtil::decodeEffectId(unsigned int uniqueId, int& effectId, int& direction, int& frameNo)
{
}

//
// 函数：decodeUIId(unsigned int uniqueId, int& composite, int& controlId, int& state)
// 参数:
//       uniqueId       [IN]   纹理唯一ID
//       composite		[OUT]  控件类型
//       id				[OUT]  小ID
//       state          [OUT]  状态 1, 正常　2, MouseOn 3, 按下 4, 失效
//
// 目的: 生成对应UI唯一纹理ID
//
void YCTextureIDUtil::decodeUIId(unsigned int uniqueId, int& composite, int& controlId, int& state)
{
}

*/
#pragma endregion decode