#ifndef _INCLUDE_YCALTASTEXTURE_H_
#define _INCLUDE_YCALTASTEXTURE_H_

class YCDList;

#include "YCITexture2D.h"

struct ATLAS_TEXTURE_INFO_T {
	unsigned int id;  // uniqueId
	int x;   // x����
	int y;   // y����
	int w;   // width
	int h;   // height
};

/*
 * YCAltasTexture : ����ͼƬѹ����һ�����������
 *
 * ע�ͣ��������е�ĳһ��ͼƬ
 */
class YCAtlasTexture2D :
	public YCITexture2D
{
public:

	YCAtlasTexture2D(YCDList *idList, YCITextureFile *texture, int status);

	virtual ~YCAtlasTexture2D(void);
	
	//
	// ������hasTexture(unsigned int uniqueId)
	//
	// Ŀ�ģ������������Ƿ����ͼƬ
	//
	// ע�ͣ�
	//     1. YCSingleTexture2D ֻ����һ��ͼƬ
	//     2. YCAtlasTexture2D ���ܰ���1..n��ͼƬ
	//
	virtual bool hasTexture(unsigned int uniqueId) const;

	//
	// ������getRect(unsigned int uniqueId, FloatRect& rect)
	//
	// Ŀ�ģ�����������������
	//
	virtual void getRect(unsigned int uniqueId, FloatRect& rect) const;

private:

	YCDList* myIdList;
};

#endif


