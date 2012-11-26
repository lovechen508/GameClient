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
	else throw YCException(2002, "YCD3DXFontSprite��Ч�������ͣ�");

	LPDIRECT3DDEVICE9 g_pd3dDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
	if (g_pd3dDevice == NULL)
	{
		throw YCException(2002, "YCD3DXFontSprite�޷�ȡ��ע��D3DDevice!");
	}

	if (FAILED(D3DXCreateFontIndirect(g_pd3dDevice, &df, &myFont)))
	{
		throw YCException(2002, "YCD3DXFontSprite��������ʧ��!");
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
// ������getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
//
// Ŀ�ģ�ȡ���ض������С����ߺͿ�
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
// ������draw(const char* info, unsigned int left, unsigned int top)
//
// Ŀ�ģ�����ַ����Ļ���
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
