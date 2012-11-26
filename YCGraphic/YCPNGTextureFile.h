#ifndef _INCLUDE_YCPNGTEXTUREFILE_H_
#define _INCLUDE_YCPNGTEXTUREFILE_H_

#include "ycitexturefile.h"

class PNGDecoder;

class YCPNGTextureFile :
	public YCITextureFile
{

public:

	YCPNGTextureFile(const char* filename);

	virtual ~YCPNGTextureFile(void);

	//
	// 函数：isTransparent(unsigned int x, unsigned int y)
	//
	// 目的：测试某个点是否透明
	//
	virtual bool isTransparent(unsigned int x, unsigned int y);

	//
	// 函数：load(void* pd3d9Device)
	//
	// 目的：加载纹理到内存
	//
	virtual bool load(void* pd3d9Device);

private:

	PNGDecoder * myDecoder;
};

#endif


