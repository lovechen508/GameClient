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
// ������width()
//
// Ŀ�ģ�ȡ��ͼ����
//
unsigned int YCITextureFile::width()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myWidth;
}
	
//
// ������height()
//
// Ŀ�ģ�ȡ��ͼ��߶�
//
unsigned int YCITextureFile::height()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myHeight;
}

//
// ������depth()
//
// Ŀ�ģ�ȡ��ͼ�����
//
unsigned int YCITextureFile::depth()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myDepth;
}
	
//
// ������format()
//
// Ŀ�ģ�ȡ��ͼ���ʽ
//
unsigned int YCITextureFile::format()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myFormat;
}

//
// ������raw()
//
// Ŀ�ģ�ȡ��ԭʼָ������
//
void* YCITextureFile::raw()
{
	SMART_ASSERT(myLPTexture != NULL);
	return myLPTexture;
}