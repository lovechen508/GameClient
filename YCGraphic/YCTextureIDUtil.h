#ifndef _INCLUDE_YCTEXTUREID_UTILITY_H_
#define _INCLUDE_YCTEXTUREID_UTILITY_H_

#include "YCTextureDef.h"

#define GET_TEXTURE_TYPE(n) (int)(n&15)

/*
 * ����ΨһID����������, ���ɼ̳�
 *
 *  ȫ��Ϊ������̬����
 */
class YCTextureIDUtil
{
public:

	static unsigned int encode(E_TEXTURE_TYPE type, const char* filename);

	static bool buildPackageName(E_TEXTURE_TYPE type, const char* filename, char* buf, int len);

private:

	//
	// ������encodeTilesMAPId(const char* filename)
	// ����: 
	//       mapId			mapId, ȫ��Ψһ
	//
	// Ŀ�ģ������ر�����ΨһID 	
	//
	static unsigned int encodeTilesMapId(const char* filename);
	
	//
	// ������encodeSharedMapId(const char* filename)
	// ����: 
	//       fileNo			[IN]  ��/data/map/shared�ļ��������PNG�ļ�
	//
	// Ŀ�ģ����㹲��ر�����ΨһID 	
	//
	static unsigned int encodeSharedMapId(const char* filename);

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
	static unsigned int encodeObstacleId(const char* filename);

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
	static unsigned int encodeNPCId(const char* filename);

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
	//	     Ŀ¼�ṹ��/data/object/player/compositetype/composityId/state/direction/frameNo.png(tga)
	//
	// Ŀ��: ����PlayerΨһ����ID
	//
	static unsigned int encodePlayerId(const char* filename);

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
	static unsigned int encodeMonsterId(const char* filename);

	//
	// ������encodeEffectId(const char* filename)
	// ����: 
	//       effectId		effectId, ȫ��Ψһ
	//       direction		����
	//       frameNo        ��ǰ֡��
	//
	// Ŀ��: ����EffectΨһ����ID
	//
	static unsigned int encodeEffectId(const char* filename);

	//
	// ������encodeUIId(const char* filename)
	// ����: 
	//       windowId		����Id
	//       pic			picID
	//
	// Ŀ��: ���ɶ�ӦUIΨһ����ID
	//
	static unsigned int encodeUIId(const char* filename);
};

#endif