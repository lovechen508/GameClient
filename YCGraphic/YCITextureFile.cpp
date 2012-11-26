#include "YCITextureFile.h"

#include "YCBasic\YCAssert.h"

#include "d3d9.h"
#include "d3dx9tex.h"

YCITextureFile::YCITextureFile(const char* filename)
	: myLPTexture(NULL)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(strlen(filename) < MAX_PATH);
	memset(myFilename, 0, MAX_PATH);
	memcpy(myFilename, filename, strlen(filename));
}

YCITextureFile::~YCITextureFile()
{	
	if (myLPTexture)
	{		
		((LPDIRECT3DTEXTURE9)myLPTexture)->Release();
		myLPTexture = NULL;
	}
}

//
// 函数：width()
//
// 目的：取得图像宽度
//
unsigned int YCITextureFile::width()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myWidth;
}
	
//
// 函数：height()
//
// 目的：取得图像高度
//
unsigned int YCITextureFile::height()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myHeight;
}

//
// 函数：depth()
//
// 目的：取得图像深度
//
unsigned int YCITextureFile::depth()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myDepth;
}
	
//
// 函数：format()
//
// 目的：取得图像格式
//
unsigned int YCITextureFile::format()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myFormat;
}

//
// 函数：raw()
//
// 目的：取得原始指针纹理
//
void* YCITextureFile::raw()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myLPTexture;
}