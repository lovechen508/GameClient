#ifndef _INCLUDE_YCOBJECTDEF_H_
#define _INCLUDE_YCOBJECTDEF_H_

//
// ��ɫ������
//
#define ROLE_NAME_LENGTH 64

//
// ����������
//
#define MAGIC_NAME_LENGTH 64

/*
 * E_MAGIC_TYPE : ħ������
 */
typedef enum _MAGIC_TYPE
{

} E_MAGIC_TYPE;

/*
 * E_EQUIP_TYPE : װ������
 */ 
typedef enum _EQUIP_TYPE
{
	
} E_EQUIP_TYPE;

/*
 * E_PLAY_SEX : ����Ա�
 */
typedef enum _PLAYER_SEX
{
	FEMALE	= 0,
	MALE	= 1
} E_PLAYER_SEX;

/*
 * E_PLAY_MODE : �����Ϸģʽ
 *
 * ˵������ƽ��PK...
 *
 */
typedef enum _PLAY_MODE
{
	PEACE_MODE		= 1,

} E_PLAY_MODE;

/*
 * E_PACKAGE_NO : �������
 */ 
typedef enum _PACKAGE_NO
{
	PACKAGE_HANDY	= 1,
	PACKAGE_STORAGE = 2,
} E_PACKAGE_NO;

#endif