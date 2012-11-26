#ifndef _INCLUDE_YCSPLITBACKGROUND_H_
#define _INCLUDE_YCSPLITBACKGROUND_H_

#include "YCIBackground.h"

#include "YCGraphic.h"

class YCTexture;
class YCTextureManager;

/*
 * YCSplitBackground �Զ�ƴ�ӱ�����
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
 *             Left_top, Right_top, Left_Bottom, Right_Bottom ������X,Y�����ظ�
 *             Left_Middle, Right_Middleֻ����Y�����ظ�
 *             Top, Bottom ֻ����X�����ظ�
 *             Middle ������X,Y���������ظ�
 */

class YCAPI YCSplitBackground :
	public YCIBackground
{

	enum E_SPLIT_MODE {
		FULL_9_TILES = 1,       //  9��ͼƴ��
		TMB_3_TILES  = 2,       //  Top-Middle-Bottom 3��ͼƴ��
		LMR_3_TILES  = 3,       //  Left-Middle-Right 3��ͼƴ��
	};

public:

	//
	// ������Build9TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// Ŀ�ģ�ȫ���ſ鴴������
	//
	static YCIBackground *Build_9_TileBG(YCTextureManager* textureManager, YCIUITag* host);

	//
	// ������Build_TMB3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// Ŀ�ģ�Top, Middle, Bottom ��������
	//       Middle��Y�����ظ�
	//
	// ע�ͣ�
	//       ��Ч�����飺myTopLeftTexture
	//                  myMiddleLeftTexture    => y�����ظ�
	//                  myBottomLeftTexture
	//
	static YCIBackground *Build_TMB3_TileBG(YCTextureManager* textureManager, YCIUITag* host);

	//
	// ������Build_LMR3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
	//
	// Ŀ�ģ�Left, Middle, Right ��������
	//       Middle��X�����ظ�
	//
	// ע�ͣ�
	//       ��Ч�����飺myTopLeftTexture
	//                  myTopTexture    => x�����ظ�
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
	// ������draw()
	//
	// Ŀ�ģ���ָ��host.(myPositionX, myPositionY)��
	//      ��͸����alpha����host.(myWidth, myHeight)�ı���
	//
	// ע�ͣ�
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
	// ������draw9Tiles(float alpha, float left, float top, float width, float height)
	//
	// Ŀ�ģ���9��ͼ������
	//
	void draw9Tiles(float alpha, float left, float top, float width, float height);

	//
	// ������drawTMBTiles(float alpha, float left, float top, float width, float height)
	//
	// Ŀ�ģ�������������ͼ������
	//
	void drawTMBTiles(float alpha, float left, float top, float width, float height);

	//
	// ������drawLMRTiles(float alpha, float left, float top, float width, float height)
	//
	// Ŀ�ģ�������������ͼ������
	//
	void drawLMRTiles(float alpha, float left, float top, float width, float height);

private:

	E_SPLIT_MODE mySplitMode;

	// ������
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

