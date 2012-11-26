#include "YCFreeTypeSprite.h"

#include <map>
#include <vector>
#include <memory>

#include "d3d9.h"
#include "d3dx9tex.h"
#include "DxErr.h"

#include "YCITexture.h"

#include "YCBasic\YCDef.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCRegistry.h"
#include "YCBasic\YCException.h"

#include "ft2build.h"
#include FT_FREETYPE_H

// 加快启动速度

#define FREETYPE_TEXTURE_WIDTH 256
#define FREETYPE_TEXTURE_HEIGHT 256
#define TEXTUREASPECT 1.0f       // 字体文理宽高比例
#define PIXELBYTES  2
#define FREETYPE_TEXTURE_BUFFER (FREETYPE_TEXTURE_WIDTH * FREETYPE_TEXTURE_HEIGHT * PIXELBYTES)

const unsigned char FONT_MASK_SELECT = 0x88;
const unsigned char FONT_MASK_SELECT_DEACTIVE = 0x60;
const unsigned char FONT_MASK_SPACE = 0x00;
const unsigned char FONT_MASK_CHAR = 0xFF;    
const size_t FONT_TEXTURE_WIDTH = 1024;

const unsigned int FT_TTF_RESOLUTION = 96;

enum constCodePoints
{
    FONT_CODE_SELECT		= 0,
    FONT_CODE_TAB			= 0x0009,
    FONT_CODE_LF			= 0x000A,
    FONT_CODE_CR			= 0x000D,
    FONT_CODE_SPACE			= 0x0020,
    FONT_CODE_LATIN_START	= 0x0021,
    FONT_CODE_NEL			= 0x0085,
    FONT_CODE_LATIN_END		= 0x00A6,
};

typedef unsigned int CodePoint;

/////////////////////////////////
// CodePoint迭代器 内部类
/////////////////////////////////
#pragma region CodePointIterator

class CodePointIterator
{
public:
	CodePointIterator(const char* info)
		: myInfo(info)
		, myPosition(info)
	{
	}
	
	~CodePointIterator()
	{
	}

	CodePoint next()
	{
		if ( (myInfo == NULL) || (myPosition == '\0') )
		{
			return 0;
		}

		BYTE c1 = (BYTE)(*myPosition);
		BYTE c2 = (BYTE)(*(myPosition+1));
		if ( (c1 >= 0x81 && c1 <= 0xFE) && (c2 >= 0x40 && c2 <= 0xFE) )
		{
			WORD codePoint;
			::MultiByteToWideChar(CP_ACP, 0, myPosition, 2, (LPWSTR)&codePoint, 1);
			myPosition += 2;
			return codePoint;
		}
		return *myPosition++;
	}

private:
		
	const char* myInfo;
	const char* myPosition;
};

#pragma endregion CodePointIterator

/////////////////////////////////
// YCFreeTypeSpriteImpl 内部类
/////////////////////////////////
#pragma region YCFreeTypeSpriteImpl

class YCFreeTypeSpriteImpl
{
	struct TTF_INFO_T {
		TTF_INFO_T(unsigned int h, unsigned int w, unsigned int y)
			: maxHeight(h)
			, maxWidth(w)
			, bearingY(y)
		{
		}

		unsigned int maxHeight;
		unsigned int maxWidth;
		unsigned int bearingY;
	};

	union FreeType_Key_T {
		struct {
			unsigned int reserved : 25;
			unsigned int size : 5;
			unsigned int stoker : 1;
			unsigned int wchar : 1;
		} composite;
		unsigned int id;
	};

public:

	//
	// 函数：key(int size, bool stoker, CodePoint codePoint)
	//
	// 目的：
	//      确定该字符所在的纹理池ID
	//		汉字区间：4E00 ~ 9FA5
	//		英文字符：  33 ~  166
	//
	static unsigned int key(int size, bool stoker, CodePoint codePoint)
	{
		FreeType_Key_T key;
		key.composite.reserved = 0;
		key.composite.size = size;
		key.composite.stoker = stoker ? 1 : 0;
		key.composite.wchar = isAChar(codePoint) ? 0 : 1;

		return key.id;
	}

	//
	// 函数：isAChar(CodePoint codePoint);
	//
	// 目的：是否单字节字符
	//
	static bool isAChar(CodePoint codePoint)
	{
		return (codePoint >= 33 && codePoint <= 166);
	}

public:

	// Single Texture info 
	struct FreeTypeTexture
	{
		LPDIRECT3DTEXTURE9 texture;

		BYTE*       mImageData;
		
		int			mTtfMaxBearingY;
		UINT		mLeftBlankNum;
		       
        int			mMaxHeight;
        int			mMaxWidth;
        
		UINT        mImage_l;
		UINT        mImage_m;
	};

	// Information about the position and size of a glyph in a texture
    class YCGlyphInfo : public YCITexture
	{
    public:

        YCGlyphInfo(CodePoint _code, FreeTypeTexture* texture, bool mono) 
            : codePoint(_code)
			, myMono(mono)
			, myTexture(texture)
			, stokerColor(0)
			, aspectRatio(TEXTUREASPECT)
        {
        }

		virtual ~YCGlyphInfo()
		{
		}
		
		//
		// 函数：raw()
		//
		// 目的：返回真实纹理LPDIRECT3DTEXTURE9指针，供YCGraphic使用
		//
		virtual void* raw()
		{
			return myTexture->texture;
		}

		//
		// 函数：setGlyphTexCoords(UINT u1Pixel, UINT v1Pixel, UINT u2Pixel, UINT v2Pixel)
		//
		// 目的：
		//
		void setGlyphTexCoords(UINT u1Pixel, UINT v1Pixel, UINT u2Pixel, UINT v2Pixel)
		{
			myRect.left = (float)u1Pixel;
			myRect.top = (float)v1Pixel;
			myRect.right = (float)u2Pixel;
			myRect.bottom = (float)v2Pixel;
			
			float u1 = (float)u1Pixel / (float)FREETYPE_TEXTURE_WIDTH;
			float v1 = (float)v1Pixel / (float)FREETYPE_TEXTURE_HEIGHT;
			float u2 = (float)u2Pixel / (float)FREETYPE_TEXTURE_WIDTH;
			float v2 = (float)v2Pixel / (float)FREETYPE_TEXTURE_HEIGHT;

			myUV[0] = u1;
			myUV[1] = v1;
			myUV[2] = u2;
			myUV[3] = v1;
			myUV[4] = u1;
			myUV[5] = v2;
			myUV[6] = u2;
			myUV[7] = v2;

			aspectRatio = (myRect.right-myRect.left)/(myRect.bottom-myRect.top);
		}


		//
		// 函数：freeTypeTexture()
		//
		// 目的：
		//
		FreeTypeTexture* freeTypeTexture()
		{
			return myTexture;
		}

		//
		// 函数：globalWidth()
		// 
		// 目的：整张字符纹理的宽度
		//
		virtual unsigned int globalWidth()
		{
			return FREETYPE_TEXTURE_WIDTH;
		}

		//
		// 函数：globalHeight()
		// 
		// 目的：整张Altas纹理的高度
		//
		virtual unsigned int globalHeight()
		{
			return FREETYPE_TEXTURE_HEIGHT;
		}

		//
		// 函数：color()
		//
		// 目的：返回透明色
		//
		virtual unsigned int color()
		{
			return 0x000000;   //black
		}

		//
		// 函数：getLastAccess()
		//
		// 目的：
		//
		time_t getLastAccess()
		{
			return lastAccess;
		}

		//
		// 函数：setLastAccess(time_t t)
		//
		// 目的：
		//
		void setLastAccess(time_t t)
		{
			lastAccess = t;
		}

		//
		// 函数：
		//
		// 目的：
		//
		void getLM(unsigned int &l, unsigned int &m)
		{
			l = myL;
			m = myM;
		}

	private:

        CodePoint	     codePoint;        //字符的unicode码
		bool             myMono;           //是否黑白字符pixel_mode
		FreeTypeTexture* myTexture;        //纹理指针
		unsigned int	 stokerColor;      //描边色
        FloatRect	     uvRect;           //纹理区域
        float		     aspectRatio;
        time_t		     lastAccess;       //字符的最后访问
        UINT		     myL;
        UINT		     myM;
    };

	// A range of code points, inclusive on both ends
    typedef std::pair<CodePoint, CodePoint> CodePointRange;
    typedef std::vector<CodePointRange> CodePointRangeList;

	// Map from unicode code point to texture coordinates
	typedef std::map<CodePoint, YCGlyphInfo*> CodePointMap;

public:
	
	//
	// 函数：createLibrary()
	//
	// 目的：获取FT_Library
	//
	static bool createLibrary()
	{
		if (gLibrary == NULL)
		{
			FT_Error error = FT_Init_FreeType(&gLibrary);
			if (error)
			{
				throw YCException(2002, "YCFreeTypeSprite初始化FT_Library失败!");
			}
		}

		LPDIRECT3DDEVICE9 g_pd3dDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
		if (g_pd3dDevice == NULL)
		{
			throw YCException(2002, "YCD3DXFontSprite无法取得注册D3DDevice!");
		}

#define MAKE_TTF_INFO_KEY(type, range, size)   ((type<<16)+((range)<<8)+size)

		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 10)] = new TTF_INFO_T(16, 14, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 10)] = new TTF_INFO_T(16, 20, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 12)] = new TTF_INFO_T(20, 17, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 12)] = new TTF_INFO_T(20, 24, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 14)] = new TTF_INFO_T(23, 21, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 14)] = new TTF_INFO_T(23, 29, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 16)] = new TTF_INFO_T(25, 23, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 16)] = new TTF_INFO_T(25, 32, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 18)] = new TTF_INFO_T(29, 26, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 18)] = new TTF_INFO_T(29, 37, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 20)] = new TTF_INFO_T(32, 29, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 20)] = new TTF_INFO_T(32, 41, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 22)] = new TTF_INFO_T(33, 32, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 22)] = new TTF_INFO_T(33, 44, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 24)] = new TTF_INFO_T(38, 35, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 24)] = new TTF_INFO_T(38, 49, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 26)] = new TTF_INFO_T(41, 38, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 26)] = new TTF_INFO_T(41, 54, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 28)] = new TTF_INFO_T(42, 41, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 28)] = new TTF_INFO_T(43, 57, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 30)] = new TTF_INFO_T(47, 44, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 30)] = new TTF_INFO_T(47, 62, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 32)] = new TTF_INFO_T(49, 47, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 32)] = new TTF_INFO_T(49, 66, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 34)] = new TTF_INFO_T(51, 49, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 34)] = new TTF_INFO_T(52, 69, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 36)] = new TTF_INFO_T(56, 53, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 36)] = new TTF_INFO_T(56, 74, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 38)] = new TTF_INFO_T(58, 56, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 38)] = new TTF_INFO_T(58, 79, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 40)] = new TTF_INFO_T(62, 58, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 40)] = new TTF_INFO_T(62, 82, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 42)] = new TTF_INFO_T(65, 62, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 42)] = new TTF_INFO_T(65, 86, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 44)] = new TTF_INFO_T(67, 65, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 44)] = new TTF_INFO_T(67, 91, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 46)] = new TTF_INFO_T(71, 67, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 46)] = new TTF_INFO_T(71, 94, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 0, 48)] = new TTF_INFO_T(73, 71, 55);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_SONG, 1, 48)] = new TTF_INFO_T(73, 99, 55);

		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 10)] = new TTF_INFO_T(17, 14, 11);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 10)] = new TTF_INFO_T(19, 19, 13);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 12)] = new TTF_INFO_T(21, 18, 13);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 12)] = new TTF_INFO_T(22, 24, 15);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 14)] = new TTF_INFO_T(26, 22, 16);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 14)] = new TTF_INFO_T(26, 28, 18);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 16)] = new TTF_INFO_T(28, 24, 18);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 16)] = new TTF_INFO_T(29, 31, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 18)] = new TTF_INFO_T(32, 27, 20);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 18)] = new TTF_INFO_T(33, 36, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 20)] = new TTF_INFO_T(36, 30, 23);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 20)] = new TTF_INFO_T(36, 40, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 22)] = new TTF_INFO_T(38, 33, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 22)] = new TTF_INFO_T(38, 43, 26);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 24)] = new TTF_INFO_T(43, 36, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 24)] = new TTF_INFO_T(43, 48, 29);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 26)] = new TTF_INFO_T(47, 39, 30);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 26)] = new TTF_INFO_T(47, 53, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 28)] = new TTF_INFO_T(49, 41, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 28)] = new TTF_INFO_T(49, 56, 33);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 30)] = new TTF_INFO_T(53, 45, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 30)] = new TTF_INFO_T(53, 60, 36);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 32)] = new TTF_INFO_T(58, 48, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 32)] = new TTF_INFO_T(58, 65, 38);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 34)] = new TTF_INFO_T(60, 50, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 34)] = new TTF_INFO_T(60, 68, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 36)] = new TTF_INFO_T(64, 54, 41);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 36)] = new TTF_INFO_T(64, 72, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 38)] = new TTF_INFO_T(68, 58, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 38)] = new TTF_INFO_T(68, 77, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 40)] = new TTF_INFO_T(70, 60, 45);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 40)] = new TTF_INFO_T(70, 80, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 42)] = new TTF_INFO_T(76, 63, 47);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 42)] = new TTF_INFO_T(76, 84, 48);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 44)] = new TTF_INFO_T(81, 66, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 44)] = new TTF_INFO_T(81, 89, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 46)] = new TTF_INFO_T(82, 68, 52);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 46)] = new TTF_INFO_T(82, 92, 52);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 0, 48)] = new TTF_INFO_T(87, 72, 54);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_YAHEI, 1, 48)] = new TTF_INFO_T(87, 97, 55);

		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 10)] = new TTF_INFO_T(16, 14, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 10)] = new TTF_INFO_T(16, 19, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 12)] = new TTF_INFO_T(20, 17, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 12)] = new TTF_INFO_T(20, 24, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 14)] = new TTF_INFO_T(23, 21, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 14)] = new TTF_INFO_T(23, 28, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 16)] = new TTF_INFO_T(25, 23, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 16)] = new TTF_INFO_T(25, 31, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 18)] = new TTF_INFO_T(29, 26, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 18)] = new TTF_INFO_T(29, 36, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 20)] = new TTF_INFO_T(32, 29, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 20)] = new TTF_INFO_T(32, 40, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 22)] = new TTF_INFO_T(33, 32, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 22)] = new TTF_INFO_T(33, 43, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 24)] = new TTF_INFO_T(38, 35, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 24)] = new TTF_INFO_T(38, 48, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 26)] = new TTF_INFO_T(41, 38, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 26)] = new TTF_INFO_T(41, 52, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 28)] = new TTF_INFO_T(42, 41, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 28)] = new TTF_INFO_T(42, 55, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 30)] = new TTF_INFO_T(47, 44, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 30)] = new TTF_INFO_T(47, 60, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 32)] = new TTF_INFO_T(49, 47, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 32)] = new TTF_INFO_T(49, 64, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 34)] = new TTF_INFO_T(51, 49, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 34)] = new TTF_INFO_T(51, 67, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 36)] = new TTF_INFO_T(56, 53, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 36)] = new TTF_INFO_T(56, 72, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 38)] = new TTF_INFO_T(58, 56, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 38)] = new TTF_INFO_T(58, 76, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 40)] = new TTF_INFO_T(62, 58, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 40)] = new TTF_INFO_T(62, 79, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 42)] = new TTF_INFO_T(65, 62, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 42)] = new TTF_INFO_T(65, 84, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 44)] = new TTF_INFO_T(67, 65, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 44)] = new TTF_INFO_T(67, 88, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 46)] = new TTF_INFO_T(71, 67, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 46)] = new TTF_INFO_T(71, 91, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 0, 48)] = new TTF_INFO_T(73, 71, 55);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_HEI, 1, 48)] = new TTF_INFO_T(73, 96, 55);

		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 10)] = new TTF_INFO_T(15, 13, 11);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 10)] = new TTF_INFO_T(15, 18, 11);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 12)] = new TTF_INFO_T(18, 17, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 12)] = new TTF_INFO_T(18, 22, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 14)] = new TTF_INFO_T(22, 20, 16);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 14)] = new TTF_INFO_T(22, 26, 16);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 16)] = new TTF_INFO_T(24, 22, 18);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 16)] = new TTF_INFO_T(24, 29, 18);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 18)] = new TTF_INFO_T(27, 25, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 18)] = new TTF_INFO_T(27, 33, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 20)] = new TTF_INFO_T(31, 28, 23);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 20)] = new TTF_INFO_T(31, 38, 23);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 22)] = new TTF_INFO_T(33, 30, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 22)] = new TTF_INFO_T(33, 41, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 24)] = new TTF_INFO_T(36, 34, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 24)] = new TTF_INFO_T(36, 45, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 26)] = new TTF_INFO_T(40, 37, 30);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 26)] = new TTF_INFO_T(40, 49, 30);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 28)] = new TTF_INFO_T(42, 39, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 28)] = new TTF_INFO_T(42, 52, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 30)] = new TTF_INFO_T(44, 42, 34);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 30)] = new TTF_INFO_T(44, 56, 34);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 32)] = new TTF_INFO_T(49, 45, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 32)] = new TTF_INFO_T(49, 60, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 34)] = new TTF_INFO_T(51, 47, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 34)] = new TTF_INFO_T(51, 63, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 36)] = new TTF_INFO_T(55, 51, 41);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 36)] = new TTF_INFO_T(55, 67, 41);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 38)] = new TTF_INFO_T(58, 54, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 38)] = new TTF_INFO_T(58, 71, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 40)] = new TTF_INFO_T(60, 56, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 40)] = new TTF_INFO_T(60, 75, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 42)] = new TTF_INFO_T(64, 59, 48);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 42)] = new TTF_INFO_T(64, 79, 48);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 44)] = new TTF_INFO_T(66, 62, 50);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 44)] = new TTF_INFO_T(66, 83, 50);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 46)] = new TTF_INFO_T(69, 64, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 46)] = new TTF_INFO_T(69, 86, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 0, 48)] = new TTF_INFO_T(73, 68, 55);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_LISHU, 1, 48)] = new TTF_INFO_T(73, 90, 55);

		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 10)] = new TTF_INFO_T(16, 14, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 10)] = new TTF_INFO_T(16, 20, 12);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 12)] = new TTF_INFO_T(20, 17, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 12)] = new TTF_INFO_T(20, 25, 14);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 14)] = new TTF_INFO_T(23, 21, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 14)] = new TTF_INFO_T(23, 29, 17);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 16)] = new TTF_INFO_T(25, 23, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 16)] = new TTF_INFO_T(26, 32, 19);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 18)] = new TTF_INFO_T(29, 26, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 18)] = new TTF_INFO_T(29, 37, 21);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 20)] = new TTF_INFO_T(32, 29, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 20)] = new TTF_INFO_T(32, 42, 24);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 22)] = new TTF_INFO_T(33, 32, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 22)] = new TTF_INFO_T(35, 45, 25);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 24)] = new TTF_INFO_T(38, 35, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 24)] = new TTF_INFO_T(38, 50, 28);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 26)] = new TTF_INFO_T(41, 38, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 26)] = new TTF_INFO_T(42, 54, 31);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 28)] = new TTF_INFO_T(42, 41, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 28)] = new TTF_INFO_T(44, 57, 32);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 30)] = new TTF_INFO_T(47, 44, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 30)] = new TTF_INFO_T(47, 62, 35);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 32)] = new TTF_INFO_T(49, 47, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 32)] = new TTF_INFO_T(51, 67, 37);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 34)] = new TTF_INFO_T(51, 49, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 34)] = new TTF_INFO_T(53, 70, 39);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 36)] = new TTF_INFO_T(56, 53, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 36)] = new TTF_INFO_T(56, 75, 42);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 38)] = new TTF_INFO_T(58, 56, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 38)] = new TTF_INFO_T(60, 79, 44);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 40)] = new TTF_INFO_T(62, 58, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 40)] = new TTF_INFO_T(62, 82, 46);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 42)] = new TTF_INFO_T(65, 62, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 42)] = new TTF_INFO_T(66, 87, 49);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 44)] = new TTF_INFO_T(67, 65, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 44)] = new TTF_INFO_T(69, 92, 51);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 46)] = new TTF_INFO_T(71, 67, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 46)] = new TTF_INFO_T(71, 95, 53);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 0, 48)] = new TTF_INFO_T(73, 71, 55);
		gTtfInfo[MAKE_TTF_INFO_KEY(FONT_KAI, 1, 48)] = new TTF_INFO_T(75, 100, 55);

		return true;
	}

	static bool getWidthAndHeight(unsigned int key, int& width, int& height)
	{
		std::map<unsigned int, TTF_INFO_T*>::iterator at = gTtfInfo.find(key);
		if (at == gTtfInfo.end())
		{
			return false;
		}

		const TTF_INFO_T* ttfInfo = at->second;
		width = ttfInfo->maxWidth;
		height = ttfInfo->maxHeight;

		return true;
	}

	static bool getWidthAndHeightA(E_FONT_TYPE type, unsigned int size, int& width, int& height)
	{
		return getWidthAndHeight(MAKE_TTF_INFO_KEY(type, 0, size), width, height);
		
	}

	static int getWidthAndHeightW(E_FONT_TYPE type, unsigned int size, int& width, int& height)
	{
		return getWidthAndHeight(MAKE_TTF_INFO_KEY(type, 1, size), width, height);
	}

	//
	// 函数：releaseLibrary()
	//
	// 目的：释放FT_Library
	//
	static void releaseLibrary()
	{
		if (gLibrary != NULL)
		{
			FT_Error error = FT_Done_FreeType(gLibrary);
			if (error)
			{
				throw YCException(2002, "YCFreeTypeSprite释放FT_Library失败!");
			}
			gLibrary = NULL;
		}

		for (std::map<unsigned int, TTF_INFO_T*>::iterator it = gTtfInfo.begin(); it != gTtfInfo.end(); ++it)
		{
			SAFE_DELETE(it->second);
		}
		gTtfInfo.clear();
	}

public:

	YCFreeTypeSpriteImpl(E_FONT_TYPE type)
		: myScalable(false)
		, myAntialiasColour(false)
		, myType(type)
		, myFace(NULL)
	{
	}

	~YCFreeTypeSpriteImpl()
	{		
		if (myFace != NULL)
		{
			FT_Done_Face(myFace);
			myFace = NULL;
		}
	}

	void load(const char* ttfFile)
	{		
		SMART_ASSERT(gLibrary != NULL);
		
		FT_Error error = FT_New_Face(gLibrary, ttfFile, 0, &myFace);
		if (error)
		{
			if (error == FT_Err_Unknown_File_Format)
			{
				throw YCException(2002, "YCFreeTypeSprite可以打开和读这个文件，但不支持它的字体格式");
			}
			else
			{
				throw YCException(2002, "YCFreeTypeSprite这个字体文件不能打开和读，或者简单的说它损坏了");
			}
		}

		//字符是否可伸缩
		myScalable = FT_IS_SCALABLE(myFace);
		
		if(myCodePointRangeList.empty())
        {
            myCodePointRangeList.push_back(CodePointRange(33, 166));
            myCodePointRangeList.push_back(CodePointRange(19968, 40869));
        }

		UINT maxFaceNum = myFace->num_faces;
		
		// buildDebugInfo();
	}

    void unLoad()
	{
		for (std::map<unsigned int, CodePointMap>::iterator it = myCodePointMaps.begin();
			 it != myCodePointMaps.end();
			 ++it)
		{
			for (std::map<CodePoint, YCGlyphInfo*>::iterator it2 = it->second.begin();
				 it2 != it->second.end();
				 ++it2)
			{
				SAFE_DELETE(it2->second);
			}
			it->second.clear();
		}
		myCodePointMaps.clear();

		for (std::map<unsigned int, FreeTypeTexture*>::iterator it = myTextureMap.begin();
			 it != myTextureMap.end();
			 ++it)
		{
			FreeTypeTexture* textureInfo = it->second;
			SAFE_RELEASE(textureInfo->texture);
			SAFE_DELETE_ARRAY(textureInfo->mImageData);
			SAFE_DELETE(textureInfo);
		}
		myTextureMap.clear();
	}

	YCGlyphInfo* getTexture(int size, CodePoint codePoint)
	{
		unsigned int key = YCFreeTypeSpriteImpl::key(size, true, codePoint);
		FreeTypeTexture* textureInfo = getTextureImpl(size, key);
		if (textureInfo == NULL)
		{
			throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker获取纹理失败");
		}

		// 纹理中是否包含该codePoint
		YCGlyphInfo* glyphInfo = NULL;
		CodePointMap& codePointMap = myCodePointMaps[key];
		CodePointMap::iterator at = codePointMap.find(codePoint);
		if (at == codePointMap.end())
		{
			glyphInfo = NULL; //creation
			if (!hasBlankInTexture(textureInfo))
			{
				removeLeastUseGlyph(codePointMap, textureInfo);
			}
			glyphInfo = renderGlyphIntoTexture(codePoint, textureInfo);
			if (glyphInfo != NULL)
			{
				codePointMap[codePoint] = glyphInfo;
			}
		}
		else
		{
			glyphInfo = at->second;
		}

		return glyphInfo;
	}

	YCGlyphInfo* getTextureWithStoker(int size, unsigned int stokercolor, CodePoint codePoint)
	{
		unsigned int key = YCFreeTypeSpriteImpl::key(size, true, codePoint);
		FreeTypeTexture* textureInfo = getTextureImpl(size, key);
		if (textureInfo == NULL)
		{
			throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker获取纹理失败");
		}

		// 纹理中是否包含该codePoint
		YCGlyphInfo* glyphInfo = NULL;
		CodePointMap& codePointMap = myCodePointMaps[key];
		CodePointMap::iterator at = codePointMap.find(codePoint);
		if (at == codePointMap.end())
		{
			glyphInfo = NULL; //creation
			if (!hasBlankInTexture(textureInfo))
			{
				removeLeastUseGlyph(codePointMap, textureInfo);
			}
			glyphInfo = renderGlyphIntoTexture(codePoint, textureInfo);
			if (glyphInfo != NULL)
			{
				codePointMap[codePoint] = glyphInfo;
			}
		}
		else
		{
			glyphInfo = at->second;
		}

		return glyphInfo;
	}

private:

	//
	// 函数：buildDebugInfo()
	//
	// 目的：构建Debug信息
	//
	void buildDebugInfo()
	{				
		std::vector<std::string> ttfVector;
		ttfVector.push_back("c:\\windows\\Fonts\\SIMSUN.ttc");
		ttfVector.push_back("c:\\windows\\Fonts\\MSYH.ttf");
		ttfVector.push_back("c:\\windows\\Fonts\\SIMHEI.ttf");
		ttfVector.push_back("c:\\windows\\Fonts\\SIMLI.ttf");
		ttfVector.push_back("c:\\windows\\Fonts\\SIMKAI.ttf");

		std::vector<int> ttfSizeVector;
		for (int m = 10; m <= 48; m += 2)
		{
			ttfSizeVector.push_back(m);
		}

		FT_Face face;
		for (unsigned int i = 0; i < ttfVector.size(); ++i)
		{			
			FT_Error error = FT_New_Face(gLibrary, ttfVector[i].c_str(), 0, &face);
			if (error)
			{
				if (error == FT_Err_Unknown_File_Format)
				{
					throw YCException(2002, "YCFreeTypeSprite可以打开和读这个文件，但不支持它的字体格式");
				}
				else
				{
					throw YCException(2002, "YCFreeTypeSprite这个字体文件不能打开和读，或者简单的说它损坏了");
				}
			}

			for (unsigned int j = 0; j < ttfSizeVector.size(); ++j)
			{
				FT_F26Dot6 ftSize = (FT_F26Dot6)(ttfSizeVector[j] * (1 << 6));

				if(FT_Set_Char_Size(face, ftSize, 0, FT_TTF_RESOLUTION, FT_TTF_RESOLUTION))
				{
					throw YCException(2002, "YCFreeTypeSprite::createTextureInfo无法设置字体大小!");
				}

				int mMaxHeight = 0;
				int mMaxWidth = 0;
				int mTtfMaxBearingY = 0;

				int rangeIndex = 0;
				for (CodePointRangeList::const_iterator r = myCodePointRangeList.begin();
					r != myCodePointRangeList.end(); ++r)
				{
					const CodePointRange& range = *r;
					for(CodePoint cp = range.first; cp <= range.second; ++cp)
					{
						if (FT_Load_Char(face, cp, FT_LOAD_RENDER))
						{
							throw YCException(2002, "YCFreeTypeSprite::createTextureInfo无法加载字符!");
						}

						if (NULL == face->glyph->bitmap.buffer) continue;

						if ( (2*(face->glyph->bitmap.rows<<6) - face->glyph->metrics.horiBearingY) > mMaxHeight)
						{
							mMaxHeight = (2*(face->glyph->bitmap.rows<<6) - face->glyph->metrics.horiBearingY);
						}

						if (face->glyph->metrics.horiBearingY > mTtfMaxBearingY)
						{
							mTtfMaxBearingY = face->glyph->metrics.horiBearingY;
						}

						if ((face->glyph->advance.x>>6) + (face->glyph->metrics.horiBearingX>>6) > mMaxWidth)
						{
							mMaxWidth = (face->glyph->advance.x>>6) + (face->glyph->metrics.horiBearingX>>6);
						}
					}

					char buf[20] = {0};
					sprintf_s(buf, "= new TTF_INFO_T(%d, %d, %d);", (mMaxHeight>>6), mMaxWidth, (mTtfMaxBearingY>>6));
					LOG_INFO("type " << ttfVector[i] << ", range = " << rangeIndex++ << " SIZE : " << ttfSizeVector[j] << buf);
				}
			}

			FT_Done_Face(face);
		}
	}

	//
	// 函数：initialConstCodePoints(int size, CodePointMap& codePointMap, FreeTypeTexture* textureInfo)
	//
	// 目的：构建常量codePoint
	//
	void initialConstCodePoints(int size, CodePointMap& codePointMap, FreeTypeTexture* textureInfo)
	{
		
	}

	//
	// 函数：createTextureInfo(int size, LPDIRECT3DTEXTURE9 texture)
	//
	// 目的：构造FreeTypeTexture
	//
	FreeTypeTexture* createTextureInfo(int size, LPDIRECT3DTEXTURE9 texture)
	{
		int iFontSize = size;

		if (myScalable)
		{
			if (FT_Set_Pixel_Sizes(myFace, iFontSize, 0))
			{
				throw YCException(2002, "YCFreeTypeSprite::createTextureInfo无法设置Scalable字体PixelSizes!");
			}
		}
		else
		{
			if (iFontSize > myFace->num_fixed_sizes)
			{
				iFontSize = myFace->num_fixed_sizes - 1;
			}

			if (FT_Set_Pixel_Sizes(myFace, myFace->available_sizes[iFontSize].width, myFace->available_sizes[iFontSize].height) != 0)
			{
				throw YCException(2002, "YCFreeTypeSprite::createTextureInfo无法设置Noscalable字体PixelSizes!");
			}
		}

        FT_F26Dot6 ftSize = (FT_F26Dot6)(iFontSize * (1 << 6));

        if(FT_Set_Char_Size( myFace, ftSize, 0, FT_TTF_RESOLUTION, FT_TTF_RESOLUTION))
        {
			throw YCException(2002, "YCFreeTypeSprite::createTextureInfo无法设置字体大小!");
		}
			
		std::auto_ptr<FreeTypeTexture> textureInfo(new FreeTypeTexture());

		textureInfo->texture = texture;
		textureInfo->mTtfMaxBearingY = 0;
		textureInfo->mImage_l = 0;
		textureInfo->mImage_m = 0;

		std::map<unsigned int, TTF_INFO_T*>::iterator at = gTtfInfo.find(MAKE_TTF_INFO_KEY(myType, 1, size));
		if (at == gTtfInfo.end())
		{
			throw YCException(2002, "YCFreeTypeSprite::createTextureInfo字体信息未缓存！");
		}

		const TTF_INFO_T& ttfInfo = *at->second;
		textureInfo->mMaxHeight = ttfInfo.maxHeight;
		textureInfo->mMaxWidth = ttfInfo.maxWidth;
		textureInfo->mTtfMaxBearingY = ttfInfo.bearingY;
		
		textureInfo->mImageData = new BYTE[FREETYPE_TEXTURE_BUFFER];
		memset(textureInfo->mImageData, 0, FREETYPE_TEXTURE_BUFFER);

		textureInfo->mLeftBlankNum = (FREETYPE_TEXTURE_WIDTH * PIXELBYTES / textureInfo->mMaxWidth)
			                       * (FREETYPE_TEXTURE_HEIGHT / textureInfo->mMaxHeight);

		return textureInfo.release();
	}

	//
	// 函数：getTextureImpl(unsigned int key)
	//
	// 目的：检查纹理是否存在
	//
	FreeTypeTexture* getTextureImpl(int size, unsigned int key)
	{
		FreeTypeTexture* found = NULL;

		std::map<unsigned int, FreeTypeTexture*>::iterator at = myTextureMap.find(key);
		if (at == myTextureMap.end())
		{
			LPDIRECT3DDEVICE9 g_pd3dDevice = (LPDIRECT3DDEVICE9)YCRegistry::get("D3DDevice");
			if (g_pd3dDevice == NULL)
			{
				throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker无法找到DIRECTD3DDEVICE9句柄!");
			}
			LPDIRECT3DTEXTURE9 texture = NULL;
			HRESULT hr = D3DXCreateTexture(g_pd3dDevice, 
				                   FREETYPE_TEXTURE_WIDTH,
								   FREETYPE_TEXTURE_HEIGHT,
								   1,
								   0,
								   D3DFMT_A8L8,
								   D3DPOOL_MANAGED,
								   &texture);
			if (FAILED(hr))
			{
				throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker创建纹理失败", DXGetErrorDescription(hr));
			}

			if (texture == NULL)
			{
				throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker创建纹理失败");
			}
			
			D3DSURFACE_DESC textureDesc;
			texture->GetLevelDesc(0, &textureDesc);
			if (textureDesc.Format != D3DFMT_A8L8) 
			{
				throw YCException(2002, "YCFreeTypeSprite::getTextureWithStoker创建纹理格式D3DFMT_A8L8");
			}

			found = createTextureInfo(size, texture);
			myTextureMap[key] = found;
		}
		else
		{
			found = at->second;
		}

		return found;
	}

	//
	// 函数：hasBlankInTexture(FreeTypeTexture* textureInfo)
	//
	// 目的：
	//
	bool hasBlankInTexture(FreeTypeTexture* textureInfo)
	{
		return (textureInfo->mLeftBlankNum > 0); 
	}

	//
	// 函数：
	//
	// 目的：
	//
	void removeLeastUseGlyph(CodePointMap& codePointMap, FreeTypeTexture* textureInfo)
	{
		YCGlyphInfo* leastUse = NULL;
		CodePointMap::iterator lru = codePointMap.end();

		for (CodePointMap::iterator it = codePointMap.begin(); it != codePointMap.end(); ++it)
		{
			YCGlyphInfo* glyphInfo = (YCGlyphInfo*)it->second;
			if (glyphInfo->freeTypeTexture() == textureInfo)
			{
				if ( (leastUse == NULL) || (leastUse->getLastAccess() > glyphInfo->getLastAccess()) )
				{
					lru = it;
					leastUse = glyphInfo;
				}
			}
		}

		if (lru == codePointMap.end())
		{
			throw YCException(2002, "YCFreeTypeSprite::removeLeastUseGlyph失败！");
		}
		
		codePointMap.erase(lru);
		leastUse->getLM(textureInfo->mImage_l,
			            textureInfo->mImage_m);
		SAFE_DELETE(leastUse);
		textureInfo->mLeftBlankNum += 1;
	}

	//
	// 函数：renderGlyphIntoTexture(CodePoint codePoint, FreeTypeTexture* textureInfo)
	//
	// 目的：渲染到纹理
	//
	YCGlyphInfo* renderGlyphIntoTexture(CodePoint codePoint, FreeTypeTexture* textureInfo)
	{
		if (FT_Load_Char(myFace, codePoint, FT_LOAD_DEFAULT))
		{
			LOG_WARNING("YCFreeTypeSprite::renderGlyphIntoTexture 无法加载 CodePoint : " << codePoint);
            throw YCException(2002, "YCFreeTypeSprite::renderGlyphIntoTexture 无法加载 CodePoint");
		}

		if (FT_Render_Glyph(myFace->glyph, FT_RENDER_MODE_NORMAL)) //FT_RENDER_MODE_NORMAL, FT_RENDER_MODE_MONO
		{
			LOG_WARNING("YCFreeTypeSprite::renderGlyphIntoTexture 无法加载 CodePoint : " << codePoint);
            throw YCException(2002, "YCFreeTypeSprite::renderGlyphIntoTexture 无法加载 CodePoint");
		}

		FT_Bitmap& bitmap = myFace->glyph->bitmap;
		unsigned char* buffer = myFace->glyph->bitmap.buffer;
		if (buffer == NULL)
        {
           LOG_WARNING("YCFreeTypeSprite::renderGlyphIntoTexture 加载为NULL CodePoint : " << codePoint);
           throw YCException(2002, "YCFreeTypeSprite::renderGlyphIntoTexture 无法加载 CodePoint");
        }

        FT_Int advance = (myFace->glyph->advance.x >> 6) + (myFace->glyph->metrics.horiBearingX >> 6);

		int rows = bitmap.rows;
		int width = bitmap.width;
		int pitch = bitmap.pitch;
		int pixel_mode = bitmap.pixel_mode;
        int y_bearing = textureInfo->mTtfMaxBearingY - (myFace->glyph->metrics.horiBearingY>>6);
        int x_bearing = myFace->glyph->metrics.horiBearingX >> 6;
		
		if (pixel_mode == FT_PIXEL_MODE_MONO)
		{				
			for (int i = 0; i < rows; ++i)
			{
				int row = i + textureInfo->mImage_m + y_bearing;
				BYTE* dest = &textureInfo->mImageData[row * FREETYPE_TEXTURE_WIDTH * PIXELBYTES] + textureInfo->mImage_l * PIXELBYTES;

				for (int j = 0; j < width; ++j)
				{	
					if(buffer[i*pitch+(j>>3)] & (0x80>>(j&7)))
					{
						*dest++ = 255;
						*dest++ = 255;
					}
					else
					{
						*dest++ = 0;
						*dest++ = 0;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < rows; ++i)
			{
				int row = i + textureInfo->mImage_m + y_bearing;
				BYTE* dest = &textureInfo->mImageData[row * FREETYPE_TEXTURE_WIDTH * PIXELBYTES] + textureInfo->mImage_l * PIXELBYTES;

				for (int j = 0; j < width; ++j)
				{				
					if (myAntialiasColour)
					{
						// Use the same greyscale pixel for all components RGBA
						*dest++ = *buffer;
					}
					else
					{
						// Always white whether 'on' or 'off' pixel, since alpha
						// will turn off
						*dest++ = FONT_MASK_CHAR;
					}
					// Always use the greyscale value for alpha
					*dest++ = *buffer++;
				}
			}
        }

        D3DLOCKED_RECT lockedRect;
        textureInfo->texture->LockRect(0, &lockedRect,0, 0);         

		YCGlyphInfo * glyphInfo = NULL;

        // 使用类型要对应(unsigned short --- D3DFMT_A8L8)
		BYTE* imageData2 = (BYTE*)lockedRect.pBits;
		for(int i = 0; i < FREETYPE_TEXTURE_WIDTH; i++){
			for(int j = 0; j < FREETYPE_TEXTURE_HEIGHT; j++){
				//Pitch数据的总长度
				int index = i * lockedRect.Pitch / PIXELBYTES + j;
				imageData2[index] = textureInfo->mImageData[index];
				imageData2[index+1] = textureInfo->mImageData[index+1];  // ? imageData2[index-1]
			}
		}
		textureInfo->texture->UnlockRect(0);
			
		glyphInfo = new YCGlyphInfo(codePoint, textureInfo, 
			(myFace->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO));
		glyphInfo->setGlyphTexCoords(textureInfo->mImage_l,  // u1
										textureInfo->mImage_m,  // v1
										textureInfo->mImage_l + (myFace->glyph->advance.x>>6), // u2
										textureInfo->mImage_m + textureInfo->mMaxHeight); // v2

		--textureInfo->mLeftBlankNum;
					
		// Advance a column
		textureInfo->mImage_l += advance;

		// If at end of row
		if( (FREETYPE_TEXTURE_WIDTH - 1) < (textureInfo->mImage_l + advance) )
		{
			textureInfo->mImage_m += (textureInfo->mMaxHeight>>6);
			textureInfo->mImage_l = 0;
		}

		//D3DXSaveTextureToFileA("c://www//test.jpg",D3DXIFF_JPG, textureInfo->texture, 0);
		
		return glyphInfo;
	}

private:
	
	//
	// 属性：全局FT_LIBRARY句柄
	//
	static FT_Library gLibrary;

	//
	// 属性：
	//
	static std::map<unsigned int, TTF_INFO_T*> gTtfInfo;

	//
	// 属性：
	//
	FT_Face myFace;

	//
	// 属性：
	//
	const E_FONT_TYPE myType;

	//
	// 属性：
	//
	bool myAntialiasColour;
	
	//
	// 属性：是否固定大小,如果字符可伸缩，值为0
	//
	bool myScalable;

	//
	// 属性：
	//
	std::map<unsigned int, CodePointMap> myCodePointMaps;
	
	//
	// 属性：Range of code points to generate glyphs for (truetype only)
	//
	CodePointRangeList myCodePointRangeList;

	//
	// 属性：
	//
	std::map<unsigned int, FreeTypeTexture*> myTextureMap;
};

#pragma endregion YCFreeTypeSpriteImpl

std::map<unsigned int, YCFreeTypeSpriteImpl::TTF_INFO_T*> YCFreeTypeSpriteImpl::gTtfInfo;
FT_Library YCFreeTypeSpriteImpl::gLibrary = NULL;

#pragma region static_init

//
// 函数：createLibrary()
//
// 目的：获取FT_Library
//
bool YCFreeTypeSprite::createLibrary()
{
	return YCFreeTypeSpriteImpl::createLibrary();
}

//
// 函数：releaseLibrary()
//
// 目的：释放FT_Library
//
void YCFreeTypeSprite::releaseLibrary()
{
	YCFreeTypeSpriteImpl::releaseLibrary();
}

#pragma endregion static_init

YCFreeTypeSprite::YCFreeTypeSprite(YCGraphic::YCD3DSprite* sprite, E_FONT_TYPE type)
	: YCIFontSprite(type)
	, mySprite(sprite)
{
	char filename[MAX_PATH] = {0};
	if (FAILED(::GetWindowsDirectory(filename, MAX_PATH)))
	{
		throw YCException(2002, "YCFreeTypeSprite无法取得GetWindowsDirectory!");
	}

	switch (type)
	{
	case FONT_SONG :
		strcat_s(filename, "\\Fonts\\SIMSUN.ttc");
		break;
	case FONT_YAHEI	:
		strcat_s(filename, "\\Fonts\\MSYH.ttf");
		break;
	case FONT_HEI	:
		strcat_s(filename, "\\Fonts\\SIMHEI.ttf");
		break;
	case FONT_LISHU	:
		strcat_s(filename, "\\Fonts\\SIMLI.ttf");
		break;
	case FONT_KAI	:
		strcat_s(filename, "\\Fonts\\SIMKAI.ttf");
		break;
	default :
		throw YCException(2002, "YCFreeTypeSprite无效字体类型！");
	};
	
	myImpl = new YCFreeTypeSpriteImpl(myType);
	myImpl->load(filename);
}

YCFreeTypeSprite::~YCFreeTypeSprite(void)
{
	myImpl->unLoad();
}

//
// 函数：getFontWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
//
// 目的：取得特定字体大小下面高和宽
//
bool YCFreeTypeSprite::getWidthAndHeight(unsigned int size, E_FONT_DIRECTION direction, const char* strInfo, int& width, int& height)
{
	int tmpWidth = 0;
	int tmpHeight = 0;

	CodePointIterator it(strInfo);
	for (CodePoint codePoint = it.next(); codePoint != 0; codePoint = it.next())
	{
		// 检查特殊字符：临时方案为跳过
		if (codePoint == FONT_CODE_TAB   
			||	codePoint == FONT_CODE_LF 
			||	codePoint == FONT_CODE_CR
			|| codePoint == FONT_CODE_SPACE)
		{
			continue;
		}

		YCFreeTypeSpriteImpl::YCGlyphInfo* glyphInfo 
			= myImpl->getTexture(mySize, codePoint);
		if (glyphInfo == NULL)
		{
			return false;
		}

		const FloatRect* rect = glyphInfo->rect();

		//初始化
		if (direction == FONT_HORIZONTAL)
		{
			tmpHeight = max(tmpHeight, rect->height());
		}
		else
		{
			tmpWidth += max(tmpWidth, rect->width());
		}

		//累计
		if (direction == FONT_HORIZONTAL)
		{
			tmpWidth += rect->width();
		}
		else
		{
			tmpHeight += rect->height();
		}
	}

	width = tmpWidth;
	height = tmpHeight;
	
	return true;
}

//
// 函数：draw(const char* info, unsigned int left, unsigned int top)
//
// 目的：完成字符串的绘制
//
void YCFreeTypeSprite::draw(const char* info, unsigned int left, unsigned int top)
{
	if (info == NULL) return;

	CodePointIterator it(info);
	for (CodePoint codePoint = it.next(); codePoint != 0; codePoint = it.next())
	{
		// 检查特殊字符：临时方案为跳过
		if (codePoint == FONT_CODE_TAB   
		 ||	codePoint == FONT_CODE_LF 
		 ||	codePoint == FONT_CODE_CR
		 || codePoint == FONT_CODE_SPACE)
		{
			continue;
		}

		YCFreeTypeSpriteImpl::YCGlyphInfo* glyphInfo 
			= myImpl->getTexture(mySize, codePoint);
		if (glyphInfo != NULL)
		{
			mySprite->draw(glyphInfo, (float)left, (float)top, 1.0f, 1.0f, 1.0f, myColor); 
			const FloatRect* rect = glyphInfo->rect();
			if (myVertical)
			{
				top += (unsigned int)(rect->height() + mySpacer);
			}
			else
			{
				//left += (codePoint > 0x4E00) ? 2 * mySize : mySize;
				left += (unsigned int)(rect->width() + mySpacer);
			}
		}
	}
}