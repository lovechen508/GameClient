#ifndef _INCLUDE_YCTEXTUREDEF_H_
#define _INCLUDE_YCTEXTUREDEF_H_

/*
 * ���������������
 * ����ͬ��������벻ͬ�ĳ���
 * ���ڹ���
 * [1, 15] ռ4��bitλ
 */

typedef enum _texturetype {
	TEXTURE_MAP              =  0,		// �ر�
	TEXTURE_SHAREDMAP        =  1,      // ����ر�
	TEXTURE_OBSTACLE		 =  2,		// ��������ľ�ڵ�
	TEXTURE_NPC              =  3,		// NPC
	TEXTURE_PLAYER           =  4,		// ���
	TEXTURE_MONSTER          =  5,		// ����
	TEXTURE_EFFECT           =  6,		// ħ����Ч
	TEXTURE_UI               =  7,		// ����

	TEXTURE_RESERVED_0		 =  8,		//��������
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
 * ��������/NPC���ϸ��������� COMPOSITEID
 * [1, 255] ռ8��bitλ
 */
typedef enum _object_composite_type {
	TEXTURE_OBJECT_BASE_MAN_1		= 0X01, //�н�ɫ1�Ż�������
	TEXTURE_OBJECT_BASE_MAN_2		= 0X02, //�н�ɫ2�Ż�������
	TEXTURE_OBJECT_BASE_MAN_3		= 0X03, //�н�ɫ3�Ż�������
	TEXTURE_OBJECT_BASE_MAN_4		= 0X04, //�н�ɫ4�Ż�������
	TEXTURE_OBJECT_BASE_MAN_5		= 0X05, //�н�ɫ5�Ż�������
	TEXTURE_OBJECT_BASE_MAN_6		= 0X06, //�н�ɫ6�Ż�������
	TEXTURE_OBJECT_BASE_MAN_7		= 0X07, //�н�ɫ7�Ż�������
	TEXTURE_OBJECT_BASE_MAN_8		= 0X08, //�н�ɫ8�Ż�������
	TEXTURE_OBJECT_BASE_MAN_9		= 0X09, //�н�ɫ9�Ż�������

	// ��������

	TEXTURE_OBJECT_BASE_WOMAN_1		= 0X21, //Ů��ɫ1�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_2		= 0X22, //Ů��ɫ2�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_3		= 0X23, //Ů��ɫ3�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_4		= 0X24, //Ů��ɫ4�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_5		= 0X25, //Ů��ɫ5�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_6		= 0X26, //Ů��ɫ6�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_7		= 0X27, //Ů��ɫ7�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_8		= 0X28, //Ů��ɫ8�Ż�������
	TEXTURE_OBJECT_BASE_WOMAN_9		= 0X29, //Ů��ɫ9�Ż�������

	// ��������

	// ==================  ����װ�����ֿ�ʼ ====================
	TEXTURE_OBJECT_HAIR             = 0X41, //ͷ��
	TEXTURE_OBJECT_CAP              = 0X42, //ñ��

	TEXTURE_OBJECT_COAT             = 0X47, //����
	TEXTURE_OBJECT_WEAPON           = 0X48, //����
	
	TEXTURE_OBJECT_TROUSERS         = 0X4A, //��װ

	TEXTURE_OBJECT_SHOES            = 0X4D, //Ь��
	// ================== ����װ�����ֽ��� =====================

	// ������������

}E_OBJECT_COMPOSITE_TYPE;

#endif
