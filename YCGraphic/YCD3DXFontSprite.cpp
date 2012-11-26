#include "YCD3DXFontSprite.h"

#include "d3d9.h"
#include "d3dx9core.h"
#include "d3dx9math.h"
#include "d3dx9shader.h"
#include "DxErr.h"

#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

YCD3DXFontSprite::YCD3DXFontSprite(E_FONT_TYPE type, int width, int height)
	: YCIFontSprite(type)
{
	D3DXFONT_DESC df;

	ZeroMemory(&df, sizeof(D3DXFONT_DESC));
	df.Height = height;
	df.Width = width;
	df.Weight = 500;
	df.MipLevels = D3DX_DEFAULT;
	df.Italic = false;
	df.CharSet = DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality = 0;
	df.PitchAndFamily = 0;

	if      (type == FONT_SONG)  strcpy_s(df.FaceName, STR_FONT_SONG);
	else if (type == FONT_HEI)   strcpy_s(df.FaceName, STR_FONT_HEI);
	else if (type == FONT_YAHEI) strcpy_s(df.FaceName, STR_FONT_YAHEI);
	else if (type == FONT_LISHU) strcpy_s(df.FaceName, STR_FONT_LISHU);
	else if (type == FONT_KAI)   strcpy_s(df.FaceName, STR_FONT_KAI);
	else throw YCException(2002, "YCD3DXFontSprite无效字体类型！");

	LPDIRECT3DDEVICE9 g_pd3dDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
	if (g_pd3dDevice == NULL)
	{
		throw YCException(2002, "YCD3DXFontSprite无法取得注册D3DDevice!");
	}

	if (FAILED(D3DXCreateFontIndirect(g_pd3dDevice, &df, &myFont)))
	{
		throw YCException(2002, "YCD3DXFontSprite创建字体失败!");
	}
}


YCD3DXFontSprite::~YCD3DXFontSprite(void)
{
	if (myFont != NULL)
	{
		myFont->Release();
	}
}

//
// 函数：getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
//
// 目的：取得特定字体大小下面高和宽
//
bool YCD3DXFontSprite::getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
{
	if (direction == FONT_HORIZONTAL)
	{
		width = size * strlen(strInfo);
		height = size;
	}
	else
	{
		width = size;
		height = size * strlen(strInfo);
	}
	return true;
}

//
// 函数：draw(const char* info, unsigned int left, unsigned int top)
//
// 目的：完成字符串的绘制
//
void YCD3DXFontSprite::draw(const char* info, unsigned int left, unsigned int top)
{
	//RECT rect;
	//rect.left = x;
	//rect.top = y;
	//rect.right = x + width;
	//rect.bottom = y + height;
	//myFontImpl->draw(rect, color, text);
	//font->DrawText(NULL, text, -1, &rect, DT_TOP|DT_LEFT, color);
}
