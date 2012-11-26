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

private:

	PNGDecoder * myDecoder;
};

#endif


