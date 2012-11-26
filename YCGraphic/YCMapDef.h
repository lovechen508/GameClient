#ifndef _INCLUDE_YCMAPDEFINE_H_
#define _INCLUDE_YCMAPDEFINE_H_

#define DYNAMIC_ZORDER			0

#define TILE_WIDTH				64
#define TILE_HEIGHT				32

//
// 地图层定义
//

#define BLOCK_LAYER					99      // 阻挡

#define WEATHER_LAYER				20		// 天气

#define TITLE_LAYER					15      // 称号，血条...

#define DRAGGED_LAYER				14      // 鼠标上

#define BEFORE_OBJECT_LAYER			12		// 人物上
#define OBJECT_LAYER				11		// 人物
#define UNDER_OBJECT_LAYER			10		// 人物下

#define NEAR_GROUND_LAYER			7		// 贴地层
#define LEVEL_2_MAP_LITTLE_BG_LAYER	6		// 上层地图，小地表
#define LEVEL_2_MAP_BIG_BG_LAYER	5		// 上层地图, 大地表

#define LEVEL_1_MAP_LAYER			1		// 下层地图, 仅支持单张图片

#endif