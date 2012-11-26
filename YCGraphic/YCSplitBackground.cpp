#include "YCSplitBackground.h"

#include <memory>

#include "YCTexture.h"
#include "YCTextureManager.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

#pragma region background_builder

//
// 函数：Build_9_TileBG(YCTextureManager* textureManager, YCIComponent* host)
//
// 目的：全部九块创建背景
//
YCIBackground * YCSplitBackground::Build_9_TileBG(YCTextureManager* textureManager, YCIUITag* host)
{
	SMART_ASSERT(host != NULL);

	if (host->hasProperty(UITAG_BACKGROUND_TOP_LEFT) 
	 && host->hasProperty(UITAG_BACKGROUND_TOP) 
	 && host->hasProperty(UITAG_BACKGROUND_TOP_RIGHT)
		      
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE_LEFT) 
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE) 
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE_RIGHT)
	     
	 && host->hasProperty(UITAG_BACKGROUND_BOTTOM_LEFT) 
	 && host->hasProperty(UITAG_BACKGROUND_BOTTOM) 
	 && host->hasProperty(UITAG_BACKGROUND_BOTTOM_RIGHT))
	{
		char background[MAX_PATH] ={0};
		YCTexture *texture = NULL;
		host->getProperty(UITAG_BACKGROUND_TOP_LEFT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> topLeft(texture);
		
		host->getProperty(UITAG_BACKGROUND_TOP, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> top(texture);

		host->getProperty(UITAG_BACKGROUND_TOP_RIGHT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> topRight(texture);

		host->getProperty(UITAG_BACKGROUND_MIDDLE_LEFT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middleLeft(texture);
		
		host->getProperty(UITAG_BACKGROUND_MIDDLE, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middle(texture);

		host->getProperty(UITAG_BACKGROUND_MIDDLE_RIGHT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middleRight(texture);

		host->getProperty(UITAG_BACKGROUND_BOTTOM_LEFT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> bottomLeft(texture);
		
		host->getProperty(UITAG_BACKGROUND_BOTTOM, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> bottom(texture);

		host->getProperty(UITAG_BACKGROUND_BOTTOM_RIGHT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> bottomRight(texture);

		return new YCSplitBackground(host, FULL_9_TILES,
			      topLeft.release(),    top.release(),    topRight.release(),
				  middleLeft.release(), middle.release(), middleRight.release(),
				  bottomLeft.release(), bottom.release(), bottomRight.release());
	}

	return NULL;
}

//
// 函数：Build_TMB3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
//
// 目的：Top, Middle, Bottom 创建背景
//       Middle在Y方向重复
//
YCIBackground * YCSplitBackground::Build_TMB3_TileBG(YCTextureManager* textureManager, YCIUITag* host)
{
	SMART_ASSERT(host != NULL);

	if (host->hasProperty(UITAG_BACKGROUND_TOP) 
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE) 
	 && host->hasProperty(UITAG_BACKGROUND_BOTTOM))
	{
		char background[MAX_PATH] ={0};
		YCTexture *texture = NULL;
				
		host->getProperty(UITAG_BACKGROUND_TOP, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> top(texture);
		
		host->getProperty(UITAG_BACKGROUND_MIDDLE, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middle(texture);
		
		host->getProperty(UITAG_BACKGROUND_BOTTOM, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> bottom(texture);


		return new YCSplitBackground(host, TMB_3_TILES,
			      NULL, top.release(),    NULL,
				  NULL, middle.release(), NULL,
				  NULL, bottom.release(), NULL);
	}

	return NULL;
}

//
// 函数：Build_LMR3_TileBG(YCTextureManager* textureManager, YCIComponent* host)
//
// 目的：Left, Middle, Right 创建背景
//       Middle在X方向重复
//
YCIBackground * YCSplitBackground::Build_LMR3_TileBG(YCTextureManager* textureManager, YCIUITag* host)
{
	SMART_ASSERT(host != NULL);

	if (host->hasProperty(UITAG_BACKGROUND_MIDDLE_LEFT) 
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE) 
	 && host->hasProperty(UITAG_BACKGROUND_MIDDLE_RIGHT))
	{
		char background[MAX_PATH] ={0};
		YCTexture *texture = NULL;
		
		host->getProperty(UITAG_BACKGROUND_MIDDLE_LEFT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middleLeft(texture);
		
		host->getProperty(UITAG_BACKGROUND_MIDDLE, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middle(texture);

		host->getProperty(UITAG_BACKGROUND_MIDDLE_RIGHT, background, MAX_PATH);
		texture = textureManager->fetch(TEXTURE_UI, background);
		if (texture == NULL)
		{
			return NULL;
		}
		std::auto_ptr<YCTexture> middleRight(texture);

		return new YCSplitBackground(host, FULL_9_TILES,
			      NULL,                 NULL,             NULL,
				  middleLeft.release(), middle.release(), middleRight.release(),
				  NULL,                 NULL,             NULL);
	}
	return NULL;
}

#pragma endregion background_builder

YCSplitBackground::YCSplitBackground(YCIUITag *host, 
		              E_SPLIT_MODE mode,
					  YCTexture * topLeft,
	                  YCTexture * top,
					  YCTexture * topRight,
					  YCTexture * middleLeft,
	                  YCTexture * middle,
					  YCTexture * middleRight,
					  YCTexture * bottomLeft,
	                  YCTexture * bottom,
					  YCTexture * bottomRight)
	: YCIBackground(host)
	, mySplitMode(mode)
	, myDrawer(NULL)
	, myTopLeftTexture(topLeft) 
	, myTopTexture(top)       
	, myTopRightTexture(topRight)  
	
	, myMiddleLeftTexture(middleLeft)
	, myMiddleTexture(middle)
	, myMiddleRightTexture(middleRight)

	, myBottomLeftTexture(bottomLeft)
	, myBottomTexture(bottom)
	, myBottomRightTexture(bottomRight)
{
	YCGraphic* graphic = (YCGraphic*)YCRegistry::get("graphic");
	if (graphic == NULL)
	{
		throw YCException(2002, "YCSplitBackground无法获取YCGraphic句柄！");
	}
	myDrawer = graphic->createD3DSprite();
}

YCSplitBackground::~YCSplitBackground(void)
{
	SAFE_DELETE(myTopLeftTexture);   
	SAFE_DELETE(myTopTexture);       
	SAFE_DELETE(myTopRightTexture);  

	SAFE_DELETE(myMiddleLeftTexture);
	SAFE_DELETE(myMiddleTexture);    
	SAFE_DELETE(myMiddleRightTexture);

	SAFE_DELETE(myBottomLeftTexture);
	SAFE_DELETE(myBottomTexture);    
	SAFE_DELETE(myBottomRightTexture);
}

//
// 函数：draw()
//
// 目的：在指定host.(myPositionX, myPositionY)处
//      以透明度alpha绘制host.(myWidth, myHeight)的背景
//
void YCSplitBackground::draw(float alpha, int left, int top, int width, int height)
{
	if (width == 0 || height == 0 || abs(alpha) < EPSINON)
	{
		return;
	}

	if      (mySplitMode == FULL_9_TILES)
	{
		draw9Tiles(alpha, (float)left, (float)top, (float)width, (float)height);
	}
	else if (mySplitMode == TMB_3_TILES)
	{
		drawTMBTiles(alpha, (float)left, (float)top, (float)width, (float)height);
	}
	else if (mySplitMode == LMR_3_TILES)
	{
		drawLMRTiles(alpha, (float)left, (float)top, (float)width, (float)height);
	}
}

//
// 函数：draw9Tiles(float alpha, float left, float top, float width, float height)
//
// 目的：用9块图画背景
//
void YCSplitBackground::draw9Tiles(float alpha, float left, float top, float width, float height)
{
	const FloatRect* rtl = myTopLeftTexture->rect();
	const FloatRect* rt = myTopTexture->rect();
	const FloatRect* rtr = myTopRightTexture->rect();

	const FloatRect* rml = myMiddleLeftTexture->rect();
	const FloatRect* rm = myMiddleTexture->rect();
	const FloatRect* rmr = myMiddleRightTexture->rect();

	const FloatRect* rbl = myBottomLeftTexture->rect();
	const FloatRect* rb = myBottomTexture->rect();
	const FloatRect* rbr = myBottomRightTexture->rect();

	if (width < (rtl->right-rtl->left + rtr->right-rtr->left))
	{
		float half_width_L = (float)width/2;
		float half_width_R = (float)width - half_width_L;

		if (height < (rtl->bottom-rtl->top + rbl->bottom-rbl->top))
		{
			//             |
			//   top_left  | top_right
			//             |
			//   ---------------------
			//             |
			// bottom_left | bottom_right
			//             |
		
			float half_height_T = (float)height/2;
			float half_height_B = (float)height - half_height_T;

			myDrawer->draw(myTopLeftTexture, left, top, half_width_L/(rtl->right-rtl->left), half_height_T/(rtl->bottom-rbl->top), alpha, 0)
					->draw(myTopRightTexture, left+half_width_L, top, half_width_R/(rtr->right-rtr->left), half_height_T/(rtr->bottom-rbr->top), alpha, 0)
					->draw(myBottomLeftTexture, left, top+half_height_T, half_width_L/(rbl->right-rbl->left), half_height_B/(rbl->bottom-rbl->top), alpha, 0)
					->draw(myBottomRightTexture, left+half_width_L, top+half_height_T, half_width_R/(rbr->right-rbr->left), half_height_B/(rbr->bottom-rbr->top), alpha, 0);
		}
		else
		{
			//             |
			//   top_left  | top_right
			//             |
			//   ---------------------
			//             |
			// middle_left | middle_right
			//             |
			//   ---------------------
			//             |
			// bottom_left | bottom_right
			//             |
			float top_height = (float)rtl->bottom - rtl->top;
			float middle_height = (float)(height 
				                - (rtl->bottom-rtl->top)
								- (rbl->bottom-rbl->top));

			myDrawer->draw(myTopLeftTexture, left, top, half_width_L/(rtl->right-rtl->left), 1.0f, alpha, 0)
					->draw(myTopRightTexture, left+half_width_L, top, half_width_R/(rtr->right-rtr->left), 1.0f, alpha, 0)
					->draw(myMiddleLeftTexture, left, top+top_height, half_width_L/(rml->right-rml->left), middle_height/(rml->bottom-rml->top), alpha, 0)
					->draw(myMiddleRightTexture, left+half_width_L, top+top_height, half_width_R/(rmr->right-rmr->left), middle_height/(rmr->bottom-rmr->top), alpha, 0)
					->draw(myBottomLeftTexture, left, top+top_height+middle_height, half_width_L/(rbl->right-rbl->left), 1.0f, alpha, 0)
					->draw(myBottomRightTexture, left+half_width_L, top+top_height+middle_height, half_width_R/(rbr->right-rbr->left), 1.0f, alpha, 0);
		}
	}
	else
	{
		if (height < (rtl->bottom-rtl->top + rbl->bottom-rbl->top))
		{
			//             |        |
			//   top_left  |  top   | top_right
			//             |        |
			//   --------------------------------
			//             |        |
			// bottom_left | bottom | bottom_right
			//             |        |
			
			float half_height_T = (float)height/2;
			float half_height_B = (float)height - half_height_T;
			
			float left_width = (float)rtl->right - rtl->left;
			float right_width = (float)rtr->right - rtr->left;
			float middle_width = (float)width - left_width - right_width;

			myDrawer->draw(myTopLeftTexture, left, top, 1.0f, half_height_T/(rtl->bottom-rtl->top), alpha, 0)
					->draw(myTopTexture, left+left_width, top, middle_width/(rt->right-rt->left), half_height_T/(rt->bottom-rt->top), alpha, 0)
					->draw(myTopRightTexture, left+left_width+middle_width, top, 1.0f, half_height_T/(rtr->bottom-rtr->top), alpha, 0)
					->draw(myBottomLeftTexture, left, top+half_height_T, 1.0f, half_height_B/(rbl->bottom-rbr->top), alpha, 0)
					->draw(myBottomTexture, left+left_width, top+half_height_T, middle_width/(rb->right-rb->left), half_height_B/(rb->bottom-rb->top), alpha, 0)
					->draw(myBottomRightTexture, left+left_width+middle_width, top+half_height_T, 1.0f, half_height_B/(rbr->bottom-rbr->top), alpha, 0);

		}
		else
		{
			//             |        |
			//   top_left  |  top   | top_right
			//             |        |
			//   --------------------------------
			//             |        |
			// middle_left | middle | middle_right
			//             |        |
			//  ---------------------------------
			//             |        |
			// bottom_left | bottom | bottom_right
			//             |        |
			float left_width = (float)(rtl->right - rtl->left);
			float right_width = (float)(rtr->right - rtr->left);
			float middle_width = (float)(width - left_width - right_width);

			float top_height = (float)(rtl->bottom - rtl->top);			
			float bottom_height = (float)(rbl->bottom - rbl->top);
			float middle_height = (float)(height - top_height - bottom_height);

			myDrawer->draw(myTopLeftTexture, left, top, 1.0f, 1.0f, alpha, 0)
					->draw(myTopTexture, left+left_width, top, middle_width/(rt->right-rt->left), 1.0f, alpha, 0)
					->draw(myTopRightTexture, left+left_width+middle_width, top, 1.0f, 1.0f, alpha, 0)
					->draw(myMiddleLeftTexture, left, top+top_height, 1.0f, middle_height/(rml->bottom-rml->top), alpha, 0)
					->draw(myMiddleTexture, left+left_width, top+top_height, middle_width/(rm->right-rm->left), middle_height/(rm->bottom-rm->top), alpha, 0)
					->draw(myMiddleRightTexture, left+left_width+middle_width, top+top_height, 1.0f, middle_height/(rmr->bottom-rmr->top), alpha, 0)
					->draw(myBottomLeftTexture, left, top+top_height+middle_height, 1.0f, 1.0f, alpha, 0)
					->draw(myBottomTexture, left+left_width, top+top_height+middle_height, middle_width/(rb->right-rb->left), 1.0f, alpha, 0)
					->draw(myBottomRightTexture, left+left_width+middle_width, top+top_height+middle_height, 1.0f, 1.0f, alpha, 0);
		}
	}
}

//
// 函数：drawTMBTiles(float alpha, float left, float top, float width, float height)
//
// 目的：用上中下三块图画背景
//
void YCSplitBackground::drawTMBTiles(float alpha, float left, float top, float width, float height)
{

}

//
// 函数：drawLMRTiles(float alpha, int left, int top, int width, int height)
//
// 目的：用左中右三块图画背景
//
void YCSplitBackground::drawLMRTiles(float alpha, float left, float top, float width, float height)
{

}

