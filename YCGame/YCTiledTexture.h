#ifndef _INCLUDE_YCTILEDTEXTURE_H_
#define _INCLUDE_YCTILEDTEXTURE_H_

#include "YCGraphic\YCITexture.h"
#include "YCGraphic\YCIMapObject.h"

class YCTileSet;

/*
 * YCTiledTexture : �ֿ��ͼ����
 *
 * ˵����YCTexture ������ģʽ
 *      ���ͼƬ�Ĵ�С����һ����ͼ�飬����YCTexture�ҽӵ���ͼ��
 *      ���ͼƬ�Ĵ�С����һ����ͼ�飬����YCTiledTexture�ҽӵ���ͼ��
 *      YCTiledTextureֻ���ṩ���ʵ�UVֵ
 */
class YCTiledTexture 
	: public YCITexture
	, public YCIMapObject
{
public:

	YCTiledTexture(YCTileSet* tileSet, YCITexture* texture);

	virtual ~YCTiledTexture(void);

	//
	// ������draw()
	//
	// Ŀ�ģ��ڵ�ͼ���������Ļ���
	//
	virtual void draw();

	//
	// ������rebindTexture(YCITexture* texture)
	//
	// Ŀ�ģ����°�����
	//
	void rebindTexture(YCITexture* texture);

	//
	// ������setRect(unsigned int left, unsigned int top)
	//
	// Ŀ�ģ����õ�ͼ���Ӧλ��
	//
	void setRect(unsigned int left, unsigned int top);

	//
	// ������raw()
	//
	// Ŀ�ģ�������ʵ����LPDIRECT3DTEXTURE9ָ�룬��YCGraphicʹ��
	//
	virtual void* raw();	

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
	virtual unsigned int color();

private:

	//
	// ���ԣ�myTileSet
	//
	// Ŀ�ģ�������Դ�ĵ�ͼ��
	//
	YCTileSet * myTileSet;

	//
	// ���ԣ�myTexture
	//
	// Ŀ�ģ��������ʵ������������������
	//
	YCITexture * myTexture;

	//
	// ���ԣ�myLeft, myTop
	//
	// Ŀ�ģ����Ͻǣ������ɺ궨�����
	//
	unsigned int myLeft;
	unsigned int myTop;

};

#endif

