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
	// ������isTransparent(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ�����ĳ�����Ƿ�͸��
	//
	virtual bool isTransparent(unsigned int x, unsigned int y);

	//
	// ������load(void* pd3d9Device)
	//
	// Ŀ�ģ����������ڴ�
	//
	virtual bool load(void* pd3d9Device);
};

#endif

