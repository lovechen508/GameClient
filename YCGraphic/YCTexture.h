#ifndef _INCLUDE_YCTEXTURE_H_
#define _INCLUDE_YCTEXTURE_H_

#include "YCITexture.h"

class YCITexture2D;

/*
 * YCTexture : ����Ԫ������
 *             ���ɼ̳�
 */
class YCAPI YCTexture : public YCITexture
{
public:

	//
	// ������YCITexture()
	//
	// Ŀ�ģ����캯����ͬʱYCITexture2D�������ü�����1
	//
	YCTexture(unsigned int uniqueId);

	//
	// ������~YCTexture(void)
	//
	// Ŀ�ģ��ͷ�������YCITexture2D�������ü�����1
	//
	virtual ~YCTexture(void);

	//
	// ������bind(YCITexture2D* texture)
	//
	// Ŀ�ģ�����ʵ������
	//
	void bind(YCITexture2D* texture);

	//
	// ������isTransparent(unsigned int x, unsigned int y)
	//
	// Ŀ�ģ�����ĳ�����Ƿ�͸��
	//
	bool isTransparent(unsigned int x, unsigned int y);
			
	//
	// ������id()
	//
	// Ŀ�ģ���������ΨһID
	//
	unsigned int unique();

	//
	// ������raw()
	//
	// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
	//
	virtual void* raw();

	//
	// ������draw()
	//
	// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
	//
	void draw();
	
	//
	// ������globalWidth()
	// 
	// Ŀ�ģ�����Altas����Ŀ��
	//
	virtual unsigned int globalWidth();

	//
	// ������globalHeight()
	// 
	// Ŀ�ģ�����Altas����ĸ߶�
	//
	virtual unsigned int globalHeight();

	//
	// ������color()
	//
	// Ŀ�ģ�����͸��ɫ
	//
	virtual unsigned int color() {return 0xFFFFFFFF;};

protected:

	unsigned int myUniqueId;
	
	//λͼ���ε���������װ
	YCITexture2D* myLPTexture; 
};

#endif