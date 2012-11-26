#ifndef _INCLUDE_YCTEXTUREDEF_H_
#define _INCLUDE_YCTEXTUREDEF_H_

/*
 * 定义纹理池类别参数
 * 将不同的纹理放入不同的池中
 * 便于管理
 * [1, 15] 占4个bit位
 */

typedef enum _texturetype {
	TEXTURE_MAP              =  0,		// 地表
	TEXTURE_SHAREDMAP        =  1,      // 共享地表
	TEXTURE_OBSTACLE		 =  2,		// 建筑及树木遮挡
	TEXTURE_NPC              =  3,		// NPC
	TEXTURE_PLAYER           =  4,		// 玩家
	TEXTURE_MONSTER          =  5,		// 怪物
	TEXTURE_EFFECT           =  6,		// 魔法特效
	TEXTURE_UI               =  7,		// 界面

	TEXTURE_RESERVED_0		 =  8,		//保留部分
	TEXTURE_RESERVED_1		 =  9,		
	TEXTURE_RESERVED_2		 = 10,		
	TEXTURE_RESERVED_3		 = 11,
	TEXTURE_RESERVED_4		 = 12,
	TEXTURE_RESERVED_5		 = 13,
	TEXTURE_RESERVED_6		 = 14,
	TEXTURE_RESERVED_7		 = 15,

}E_TEXTURE_TYPE;

#define TEXTURE_POOL_SIZE (E_TEXTURE_TYPE::TEXTURE_UI+1)

/*
 * 主成人物/NPC身上各部份纹理 COMPOSITEID
 * [1, 255] 占8个bit位
 */
typedef enum _object_composite_type {
	TEXTURE_OBJECT_BASE_MAN_1		= 0X01, //男角色1号基础纹理
	TEXTURE_OBJECT_BASE_MAN_2		= 0X02, //男角色2号基础纹理
	TEXTURE_OBJECT_BASE_MAN_3		= 0X03, //男角色3号基础纹理
	TEXTURE_OBJECT_BASE_MAN_4		= 0X04, //男角色4号基础纹理
	TEXTURE_OBJECT_BASE_MAN_5		= 0X05, //男角色5号基础纹理
	TEXTURE_OBJECT_BASE_MAN_6		= 0X06, //男角色6号基础纹理
	TEXTURE_OBJECT_BASE_MAN_7		= 0X07, //男角色7号基础纹理
	TEXTURE_OBJECT_BASE_MAN_8		= 0X08, //男角色8号基础纹理
	TEXTURE_OBJECT_BASE_MAN_9		= 0X09, //男角色9号基础纹理

	// 保留部分

	TEXTURE_OBJECT_BASE_WOMAN_1		= 0X21, //女角色1号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_2		= 0X22, //女角色2号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_3		= 0X23, //女角色3号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_4		= 0X24, //女角色4号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_5		= 0X25, //女角色5号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_6		= 0X26, //女角色6号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_7		= 0X27, //女角色7号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_8		= 0X28, //女角色8号基础纹理
	TEXTURE_OBJECT_BASE_WOMAN_9		= 0X29, //女角色9号基础纹理

	// 保留部分

	// ==================  人物装备部分开始 ====================
	TEXTURE_OBJECT_HAIR             = 0X41, //头发
	TEXTURE_OBJECT_CAP              = 0X42, //帽子

	TEXTURE_OBJECT_COAT             = 0X47, //上衣
	TEXTURE_OBJECT_WEAPON           = 0X48, //武器
	
	TEXTURE_OBJECT_TROUSERS         = 0X4A, //下装

	TEXTURE_OBJECT_SHOES            = 0X4D, //鞋子
	// ================== 人物装备部分结束 =====================

	// 其它保留部分

}E_OBJECT_COMPOSITE_TYPE;

#endif
