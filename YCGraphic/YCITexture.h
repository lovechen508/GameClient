#ifndef _INCLUDE_YCITEXTURE_H_
#define _INCLUDE_YCITEXTURE_H_

#include "YCBasic\YCDef.h"

#include "YCRect.h"

/*
 * YCITexture : ���������
 */
class YCAPI YCITexture
{
public:

	YCITexture(void);

	virtual ~YCITexture(void);

	//
	// ������fetchUV()
	//
	// Ŀ�ģ��õ�UVֵ
	//
	void fetchUV(float& leftTopU,     float& leftTopV,          // V0         V0 ------ V1
		         float& rightTopU,    float& rightTopV,         // V1           |      |
		         float& leftBottomU,  float& leftBottomV,       // V2           |      |
		         float& rightBottomU, float& rightBottomV);     // V3         V2 ------ V3

	//
	// ������rect()
	//
	// Ŀ�ģ�����������������
	//
	const FloatRect* rect() const;

	//
	// ������raw()
	//
	// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
	//
	virtual void* raw() = 0;	

	//
	// ������globalWidth()
	// 
	// Ŀ�ģ�����Altas����Ŀ��
	//
	virtual unsigned int globalWidth() = 0;

	//
	// ������globalHeight()
	// 
	// Ŀ�ģ�����Altas����ĸ߶�
	//
	virtual unsigned int globalHeight() = 0;

	//
	// ������color()
	//
	// Ŀ�ģ�����͸��ɫ
	//
	virtual unsigned int color() = 0;

protected:	
	
	FloatRect myRect; //ͼƬ����

	float myUV[8]; // UVֵ  V0,V1,V2,V3
};

#endif

