#include "YCTGATextureFile.h"


YCTGATextureFile::YCTGATextureFile(const char* filename)
	:YCITextureFile(filename)
{
}


YCTGATextureFile::~YCTGATextureFile(void)
{
}

//
// ������isTransparent(unsigned int x, unsigned int y)
//
// Ŀ�ģ�����ĳ�����Ƿ�͸��
//
bool YCTGATextureFile::isTransparent(unsigned int x, unsigned int y)
{
	return true;
}

//
// ������load(void* pd3d9Device)
//
// Ŀ�ģ����������ڴ�
//
bool YCTGATextureFile::load(void* pd3d9Device)
{
	return true;
}
