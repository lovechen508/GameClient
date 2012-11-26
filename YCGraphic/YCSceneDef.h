#ifndef _INCLUDE_YCSCENEDEF_H_
#define _INCLUDE_YCSCENEDEF_H_

/*
 * Zorder 遵循右手法则
 *        即值越大越靠前
 *
 * 从后面到前面依次：
 *
 *			大地表2（Tiled）
 *			大地表1（Tiled）
 *
 *          小地表 （Tiled）
 *
 *          建筑，阻挡点，特效，人物 （动态ZOrder）
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
