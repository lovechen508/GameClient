#include "YCTGATextureFile.h"


YCTGATextureFile::YCTGATextureFile(const char* filename)
	:YCITextureFile(filename)
{
}


YCTGATextureFile::~YCTGATextureFile(void)
{
}

//
// 函数：isTransparent(unsigned int x, unsigned int y)
//
// 目的：测试某个点是否透明
//
bool YCTGATextureFile::isTransparent(unsigned int x, unsigned int y)
{
	return true;
}

//
// 函数：load(void* pd3d9Device)
//
// 目的：加载纹理到内存
//
bool YCTGATextureFile::load(void* pd3d9Device)
{
	return true;
}
