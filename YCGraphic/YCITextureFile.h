#ifndef _INCLUDE_YCITEXTUREFILE_H_
#define _INCLUDE_YCITEXTUREFILE_H_

#include "YCBasic\YCDef.h"

/*
 * YCITextureFile : ��Ӧһ������λ�õ�ͼƬ
 *                 ����ͼƬ��������Ϣ
 */
class YCITextureFile
{
public:

	//
	// ������
	//
	// Ŀ�ģ�
	//
	YCITextureFile(const char* filename);

	~YCITextureFile();

	//
	// ������load(void* pd3d9Device)
	//
	// Ŀ�ģ����������ڴ�
	//
	virtual bool load(void* pd3d9Device) = 0;

	//
	// ������width()
	//
	// Ŀ�ģ�ȡ��ͼ����
	//
	unsigned int width();
	
	//
	// ������height()
	//
	// Ŀ�ģ�ȡ��ͼ��߶�
	//
	unsigned int height();

	//
	// ������depth()
	//
	// Ŀ�ģ�ȡ��ͼ�����
	//
	unsigned int depth();
	
	//
	// ������format()
	//
	// Ŀ�ģ�ȡ��ͼ���ʽ
	//
	unsigned int format();
	
	//
	// ������raw()
	//
	// Ŀ�ģ�ȡ��ԭʼָ������
	//
	void* raw();
	
	//
	// ������isTransparent(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ�����ĳ�����Ƿ�͸��
	//
	virtual bool isTransparent(unsigned int x, unsigned int y) = 0;
	//
	// ������
	//
	// Ŀ�ģ�
	//
	
	//
	// ������
	//
	// Ŀ�ģ�
	//
	
	//
	// ������
	//
	// Ŀ�ģ�
	//
	
	//
	// ������
	//
	// Ŀ�ģ�
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