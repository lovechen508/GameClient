#ifndef _INCLUDE_YCSCENEDEF_H_
#define _INCLUDE_YCSCENEDEF_H_

/*
 * Zorder ��ѭ���ַ���
 *        ��ֵԽ��Խ��ǰ
 *
 * �Ӻ��浽ǰ�����Σ�
 *
 *			��ر�2��Tiled��
 *			��ر�1��Tiled��
 *
 *          С�ر� ��Tiled��
 *
 *          �������赲�㣬��Ч������ ����̬ZOrder��
 *
 *          UI
 */
typedef enum eSceneLayerZorder{
	BIG_BACKGROUND_2    =    -9,
	BIG_BACKGROUND_1    =    -8,

	SMALL_BACKGROUND    =    -3,

	DYNAMIC_ZORDER		=  1000,

	UI                  =  2000,
} E_SCENELAYER_ZORDER;

#define TILE_WIDTH		64
#define TILE_HEIGHT		32


#endif
