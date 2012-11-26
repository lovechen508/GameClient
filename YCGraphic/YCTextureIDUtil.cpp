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
		unsigned int mapId:8;    // �ر�Id ����256�Ŵ��ͼ
		unsigned int picId:12;   // ͼƬID, [1, 4093] ֧��4093�ֿؼ�����
		unsigned int reserved:8; // ����
	} composite;
};

union __Obstacle {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		  // E_TEXTURE_TYPE
		unsigned int obstacleId:10;   // ObstacleId [1 .. 1023]
		unsigned int direction:3;     // direction [0,7]
		unsigned int frameNo:5;       // ����[0,31] ���31֡
		unsigned int reserved:10;     // ����
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
		unsigned int frameNo:4;     // ����[0,15] ���16֡
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
		unsigned int frameNo:5;         // ����[0,31] ���31֡
	} composite;
};

union __Monster {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int monsterId:12;  // ����ID, [1, 4095] ֧��4095�ֹ���
		unsigned int direction:3;   // direction [0,7]
		unsigned int state:5;       // state [0,31]
		unsigned int frameNo:5;     // ����[0,31] ���31֡
		unsigned int reserved:3;    // ����
	} composite;
};

union __Effect {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int effectId:14;   // ��ЧID, [1, 4095] ֧��4095����Ч
		unsigned int direction:3;   // direction [0,7]
		unsigned int frameNo:6;     // ����[0,63] ���64֡
		unsigned int reserved:5;    // ����
	} composite;
};

union __UI {
	unsigned int uniqueid;
	struct {
		unsigned int typeId:4;		// E_TEXTURE_TYPE
		unsigned int windowId:8;    // ���ڱ��, [0, 255] ֧��256�ִ��� 0��Ϊshared����ͼƬ����
		unsigned int picId:12;      // ͼƬID, [1, 4093] ֧��4093�ֿؼ�����
		unsigned int reserved:8;    // ����
	} composite;
};

#pragma pack(pop)

//
// ������decode(int uniqueId, int& param1, int& param2, int& param3, int& param4, int& param5, int& param6)
//
// ������
//       uniqueId     [IN]   ȫ��Ψһ����ID
//       param1       [OUT]  ����һ��Ӧ����typeId
//       param2       [OUT]  ������
//       param3       [OUT]  ������
//       param4       [OUT]  ������
//       param5       [OUT]  ������
//       param6       [OUT]  ������
//
// ע�ͣ�
//       ���صĲ����ж����٣���Щ��ʼֵ������0�����Ե�һ��С����ı�ʾ��Ч��������
//       ��Ϊ��һ����ʾtypeId���������£�
//           ����ֵΪ:
//                    param1 = 3     �ڹ涨��3����NPC   
//                    param2 = 18    npcId
//                    param3 = 0x47  compositeId �������� 71
//                    param4 = 3     direction ����3�ŷ���
//                    param5 = 1     state վ��
//                    param6 = 13    frameNo վ�������ĵ�13֡
//           ӳ�䵽����·��Ϊ��
//                   YCSingleTexture2D : data/object/npc/18/71/3/1/13.png ��13.lzx����������
//                   YCAtlasTexture2D  : data/object/npc/18/71/3/1/texture.xml, texture.png(AltasTexture), textuer.lzx(��������)
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
//	else                                 { SMART_ASSERT(false); throw YCException(2002, "δ֪������!"); }
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
	else                                throw YCException(2002, "δ֪������!"); 

	return 0;
}

bool buildPackageName(E_TEXTURE_TYPE type, const char* filename, char* buf, int len)
{
	return true;
}

//
// ������encodeTilesMAPId(const char* filename)
// ����: 
//       mapId			mapId, ȫ��Ψһ
//       x				X����
//       y			    Y����
//
// Ŀ�ģ������ر�����ΨһID 	
//
unsigned int YCTextureIDUtil::encodeTilesMapId(const char* filename)
{
	std::vector<std::string> dirs;
	YCStringHelper::split(filename, "//", &dirs);

	if (dirs.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId�ļ�·������!");
	}

	if (!YCStringHelper::isType(dirs[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId�ļ�·��������!");
	}

	std::vector<std::string> fileVector;
	YCStringHelper::split(dirs[1].c_str(), ".", &fileVector);
	if (fileVector.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId��Ч�����ļ�!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeTilesMapId��Ч�����ļ�����!");
	}

	__TileMap tilemap;

	tilemap.composite.typeId = TEXTURE_MAP;
	tilemap.composite.mapId = atoi(dirs[0].c_str());
	tilemap.composite.picId = atoi(fileVector[0].c_str());
	tilemap.composite.reserved = 0;

	return tilemap.uniqueid;
}

//
// ������encodeSharedMapId(const char* filename)
// ����: 
//       fileNo			[IN]  ��/data/map/shared�ļ��������PNG�ļ�
//
// Ŀ�ģ����㹲��ر�����ΨһID 	
//
unsigned int YCTextureIDUtil::encodeSharedMapId(const char* filename)
{
	return 0;
}

//
// ������encodeObstacleId(const char* filename)
// 
// ����: 
//       obstacleId		obstacleId, ȫ��Ψһ
//       direction		����
//       frameNo        ��ǰ֡��
//
// Ŀ�ģ����㽨��,��ľ���ڵ���������ΨһID 	
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
// ������encodeNPCId(const char* filename)
// ����: 
//       npcId			NPCId, ȫ��Ψһ
//       composite      ���岿λ
//       direction		����
//       state          NPC״̬��1, վ����2, ����
//       frameNo        ��ǰ֡��
//
// Ŀ��: ����NPCΨһ����ID
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
// ������encodePlayerId(const char* filename)
// ����: 
//      composite		compositeType, ȫ��Ψһ YCTextureDef::E_OBJECT_COMPOSITE_TYPE
//      composityId     �ڸ�compositeType�����compositeId������Ь�������µ�ѩ��ѥ�ȣ��������µ�XX����      
//      state           ����״̬��1, վ����2, ���ߡ�3, ����, ����...
//      direction		����
//      frameNo         ��ǰ֡��
//
// ע�ͣ�
//	     Ŀ¼�ṹ��\data\object\player\compositetype\composityId\state\direction\frameNo.png(tga)
//
// Ŀ��: ����PlayerΨһ����ID
//
unsigned int YCTextureIDUtil::encodePlayerId(const char* filename)
{
	std::vector<std::string> dirs;

	// compositetype\composityId\state\direction\frameNo
	YCStringHelper::split(filename, "\\", &dirs);
	if (dirs.size() != 5)
	{
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId��Ч�ļ�·��!");
	}

	for (int i = 0; i < 4; i++)
	{
		if (!YCStringHelper::isType(dirs[i].c_str(), YCStringHelper::POSITIVE_INTEGER))
		{
			throw YCException(2002, "YCTextureIDUtil::encodePlayerId�ļ�·��������!");
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
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId��Ч�����ļ�!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodePlayerId��Ч�����ļ�����!");
	}

	player.composite.frameNo = atoi(fileVector[0].c_str());

	return player.uniqueid;
}

//
// ������encodeMonsterId(const char* filename)
// ����: 
//       monsterId  	MonsterId, ȫ��Ψһ
//������ direction		����
//       state          NPC״̬��1, վ����2, ����, ���� ...
//       frameNo        ��ǰ֡��
//
// Ŀ��: ����MonsterΨһ����ID
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
// ������encodeEffectId(const char* filename)
// ����: 
//       effectId		effectId, ȫ��Ψһ
//       direction		����
//       frameNo        ��ǰ֡��
//
// Ŀ��: ����EffectΨһ����ID
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
// ������encodeUIId(const char* filename)
// ����: 
//       windowId		����ID
//       pic			picID
//
// Ŀ��: ���ɶ�ӦUIΨһ����ID
//
unsigned int YCTextureIDUtil::encodeUIId(const char* filename)
{
	std::vector<std::string> dirs;
	YCStringHelper::split(filename, "\\", &dirs);

	if (dirs.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId�ļ�·������!");
	}

	if (!YCStringHelper::isType(dirs[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId�ļ�·��������!");
	}

	std::vector<std::string> fileVector;
	YCStringHelper::split(dirs[1].c_str(), ".", &fileVector);
	if (fileVector.size() != 2)
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId��Ч�����ļ�!");
	}

	if (!YCStringHelper::isType(fileVector[0].c_str(), YCStringHelper::POSITIVE_INTEGER))
	{
		throw YCException(2002, "YCTextureIDUtil::encodeUIId��Ч�����ļ�����!");
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
// ������decodeTilesMAPId(unsigned int uniqueId, int& mapId, int& x, int& y)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       mapId			[OUT]  mapId, ȫ��Ψһ
//       x				[OUT]  X����
//       y			    [OUT]  Y����
//
// Ŀ�ģ������ر�����ΨһID 	
//
void YCTextureIDUtil::decodeTilesMapId(unsigned int uniqueId, int& mapId, int& x, int& y)
{
}
	
//
// ������decodeSmallMAPId(unsigned int uniqueId, int& mapId, int& x, int& y)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       mapId			[OUT]  mapId, ȫ��Ψһ
//       x				[OUT]  X����
//       y			    [OUT]  Y����
//
// Ŀ�ģ�����С�ر�����洢·�� 	
//
void YCTextureIDUtil::decodeSmallMapId(unsigned int uniqueId, int& mapId, int& x, int& y)
{
}

//
// ������decodeObstacleId(unsigned int uniqueId, int& obstacleId, int& direction, int& frameNo)
// 
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       obstacleId		[OUT] obstacleId, ȫ��Ψһ
//       direction		[OUT] ����
//       frameNo        [OUT] ��ǰ֡��
//
// Ŀ�ģ����㽨��,��ľ���ڵ���������ΨһID 	
//
void YCTextureIDUtil::decodeObstacleId(unsigned int uniqueId, int& obstacleId, int& direction, int& frameNo)
{
}

//
// ������decodeNPCId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       npcId			NPCId, ȫ��Ψһ
//       composite      ���岿λ
//       direction		����
//       state          NPC״̬��1, վ����2, ����
//       frameNo        ��ǰ֡��
//
// Ŀ��: ����NPCΨһ����ID
//
void YCTextureIDUtil::decodeNPCId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
{
}

//
// ������decodePlayerId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       composite		compositeId, ȫ��Ψһ
//������ direction		����
//       state          ����״̬��1, վ����2, ���ߡ�3, ����, ����...
//������ frameNo        ��ǰ֡��
//
// Ŀ��: ����PlayerΨһ����ID
//
void YCTextureIDUtil::decodePlayerId(unsigned int uniqueId, int& npcId, int& composite, int& direction, int& state, int& frameNo)
{
}

//
// ������decodeMonsterId(unsigned int uniqueId, int& monsterId, int& direction, int& state, int& frameNo)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       monsterId  	[OUT]  MonsterId, ȫ��Ψһ
//������ direction		[OUT]  ����
//       state          [OUT]  NPC״̬��1, վ����2, ����, ���� ...
//       frameNo        [OUT]  ��ǰ֡��
//
// Ŀ��: ����MonsterΨһ����ID
//
void YCTextureIDUtil::decodeMonsterId(unsigned int uniqueId, int& monsterId, int& direction, int& state, int& frameNo)
{
}

//
// ������decodeEffectId(int effectId, int direction, int frameNo)
// ����: 
//       uniqueId       [IN]   ����ΨһID
//       effectId		[OUT]  effectId, ȫ��Ψһ
//       direction		[OUT]  ����
//       frameNo        [OUT]  ��ǰ֡��
//
// Ŀ��: ����EffectΨһ����ID
//
void YCTextureIDUtil::decodeEffectId(unsigned int uniqueId, int& effectId, int& direction, int& frameNo)
{
}

//
// ������decodeUIId(unsigned int uniqueId, int& composite, int& controlId, int& state)
// ����:
//       uniqueId       [IN]   ����ΨһID
//       composite		[OUT]  �ؼ�����
//       id				[OUT]  СID
//       state          [OUT]  ״̬ 1, ������2, MouseOn 3, ���� 4, ʧЧ
//
// Ŀ��: ���ɶ�ӦUIΨһ����ID
//
void YCTextureIDUtil::decodeUIId(unsigned int uniqueId, int& composite, int& controlId, int& state)
{
}

*/
#pragma endregion decode