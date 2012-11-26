#ifndef _INCLUDE_YCSPLITBACKGROUND_H_
#define _INCLUDE_YCSPLITBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

class YCTexture;
class YCTextureManager;

/*
 * YCSplitBackground 自动拼接背景类
 *
 *
 *      -----------------------------------------------------------------------------------
 *     |  Left_top    |                         Top                         | Right_top    |
 *     |-----------------------------------------------------------------------------------|
 *     |              |                                                     |              |
 *     |              |                                                     |              |    
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     | Left_Middle  |                       Middle                        | Right_Middle |
 *     |              |                                                     |              |
 *     |              |                                                     |              |   
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |              |                                                     |              |
 *     |-----------------------------------------------------------------------------------|
 *     | Left_Bottom  |                      Bottom                         | Right_Bottom |
 *      -----------------------------------------------------------------------------------
 *
 *    Description:
 *             Left_top, Right_top, Left_Bottom, Right_Bottom 不会在X,Y方向重复
 *             Left_Middle, Right_Middle只会在Y方向重复
 *             Top, Bottom 只会在X方向重复
 *             Middle 可以在X,Y两个方向重复
 */

class YCAPI YCSplitBackground :
	public YCIBackground
{

	enum E_SPLIT_MODE {
		FULL_9_TILES = 1,       //  9块图拼接
		TMB_3_TILES  = 2,       //  Top-Middle-Bottom 3块图拼接
		LMR_3_TILES  = 3,       //  Left-Middle-Right 3块图拼接
	};

public:

	//
	// 函数：Build9TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// 目的：全部九块创建背景
	//
	static YCIBackground *Build_9_TileBG(YCTextureManager* textureManager, YCIUITag* host);

	//
	// 函数：Build_TMB3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// 目的：Top, Middle, Bottom 创建背景
	//       Middle在Y方向重复
	//
	// 注释：
	//       有效背景块：myTopLeftTexture
	//                  myMiddleLeftTexture    => y方向重复
	//                  myBottomLeftTexture
	//
	static YCIBackground *Build_TMB3_TileBG(YCTextureManager* textureManager, YCIUITag* host);

	//
	// 函数：Build_LMR3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// 目的：Left, Middle, Right 创建背景
	//       Middle在X方向重复
	//
	// 注释：
	//       有效背景块：myTopLeftTexture
	//                  myTopTexture    => x方向重复
	//                  myTopRightTexture
	//
	static YCIBackground *Build_LMR3_TileBG(YCTextureManager* textureManager, YCIUITag* host);

protected:

	YCSplitBackground(YCIUITag *host, 
		              E_SPLIT_MODE mode,
					  YCTexture * topLeft,
	                  YCTexture * top,
					  YCTexture * topRight,
					  YCTexture * middleLeft,
	                  YCTexture * middle,
					  YCTexture * middleRight,
					  YCTexture * bottomLeft,
	                  YCTexture * bottom,
					  YCTexture * bottomRight);

	virtual ~YCSplitBackground(void);

	//
	// 函数：draw()
	//
	// 目的：在指定host.(myPositionX, myPositionY)处
	//      以透明度alpha绘制host.(myWidth, myHeight)的背景
	//
	// 注释：
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	//
	virtual void draw(float alpha, int left, int top, int width, int height);

private:

	//
	// 函数：draw9Tiles(float alpha, float left, float top, float width, float height)
	//
	// 目的：用9块图画背景
	//
	void draw9Tiles(float alpha, float left, float top, float width, float height);

	//
	// 函数：drawTMBTiles(float alpha, float left, float top, float width, float height)
	//
	// 目的：用上中下三块图画背景
	//
	void drawTMBTiles(float alpha, float left, float top, float width, float height);

	//
	// 函数：drawLMRTiles(float alpha, float left, float top, float width, float height)
	//
	// 目的：用左中右三块图画背景
	//
	void drawLMRTiles(float alpha, float left, float top, float width, float height);

private:

	E_SPLIT_MODE mySplitMode;

	// 纹理集合
	YCTexture *myTopLeftTexture;     // => no-repeat
	YCTexture *myTopTexture;         // => repeat-x
	YCTexture *myTopRightTexture;    // => no-repeat

	YCTexture *myMiddleLeftTexture;  // => repeat-y
	YCTexture *myMiddleTexture;      // => repeat
	YCTexture *myMiddleRightTexture; // => repeat-y

	YCTexture *myBottomLeftTexture;  // => no-repeat
	YCTexture *myBottomTexture;      // => repeat-x
	YCTexture *myBottomRightTexture; // => no-repeat

	YCGraphic::YCD3DSprite* myDrawer;

};

#endif

