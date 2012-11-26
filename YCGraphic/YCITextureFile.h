#ifndef _INCLUDE_YCITEXTUREFILE_H_
#define _INCLUDE_YCITEXTUREFILE_H_

#include "YCBasic\YCDef.h"

/*
 * YCITextureFile : 对应一张物理位置的图片
 *                 包含图片的所有信息
 */
class YCITextureFile
{
public:

	//
	// 函数：
	//
	// 目的：
	//
	YCITextureFile(const char* filename);

	~YCITextureFile();

	//
	// 函数：load(void* pd3d9Device)
	//
	// 目的：加载纹理到内存
	//
	virtual bool load(void* pd3d9Device) = 0;

	//
	// 函数：width()
	//
	// 目的：取得图像宽度
	//
	unsigned int width();
	
	//
	// 函数：height()
	//
	// 目的：取得图像高度
	//
	unsigned int height();

	//
	// 函数：depth()
	//
	// 目的：取得图像深度
	//
	unsigned int depth();
	
	//
	// 函数：format()
	//
	// 目的：取得图像格式
	//
	unsigned int format();
	
	//
	// 函数：raw()
	//
	// 目的：取得原始指针纹理
	//
	void* raw();
	
	//
	// 函数：isTransparent(unsigned int x, unsigned int y)
	//
	// 目的：测试某个点是否透明
	//
	virtual bool isTransparent(unsigned int x, unsigned int y) = 0;
	//
	// 函数：
	//
	// 目的：
	//
	
	//
	// 函数：
	//
	// 目的：
	//
	
	//
	// 函数：
	//
	// 目的：
	//
	
	//
	// 函数：
	//
	// 目的：
	//
	
	
protected:

	void * myLPTexture;

	char myFilename[MAX_PATH];

	unsigned int myWidth;
	unsigned int myHeight;
	unsigned int myDepth;
	unsigned int myFormat;
	unsigned int myResourceType;
	unsigned int myImageFileFormat;
};

#endif