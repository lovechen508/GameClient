#ifndef _INCLUDE_YCOBJECTDEF_H_
#define _INCLUDE_YCOBJECTDEF_H_

//
// 角色名长度
//
#define ROLE_NAME_LENGTH 64

//
// 技能名长度
//
#define MAGIC_NAME_LENGTH 64

/*
 * E_MAGIC_TYPE : 魔法类型
 */
typedef enum _MAGIC_TYPE
{

} E_MAGIC_TYPE;

/*
 * E_EQUIP_TYPE : 装备类型
 */ 
typedef enum _EQUIP_TYPE
{
	
} E_EQUIP_TYPE;

/*
 * E_PLAY_SEX : 玩家性别
 */
typedef enum _PLAYER_SEX
{
	FEMALE	= 0,
	MALE	= 1
} E_PLAYER_SEX;

/*
 * E_PLAY_MODE : 玩家游戏模式
 *
 * 说明：和平，PK...
 *
 */
typedef enum _PLAY_MODE
{
	PEACE_MODE		= 1,

} E_PLAY_MODE;

/*
 * E_PACKAGE_NO : 包裹编号
 */ 
typedef enum _PACKAGE_NO
{
	PACKAGE_HANDY	= 1,
	PACKAGE_STORAGE = 2,
} E_PACKAGE_NO;

#endif