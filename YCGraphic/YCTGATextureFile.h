#ifndef _INCLUDE_YCTGATEXTUREFILE_H_
#define _INCLUDE_YCTGATEXTUREFILE_H_

#include "ycitexturefile.h"

class YCTGATextureFile :
	public YCITextureFile
{

public:

	YCTGATextureFile(const char* filename);
	
	virtual ~YCTGATextureFile(void);

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
};

#endif

