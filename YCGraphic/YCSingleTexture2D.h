#ifndef _INCLUDE_YCSINGLETEXTURE2D_H_
#define _INCLUDE_YCSINGLETEXTURE2D_H_

#include "YCITexture2D.h"

/*
 * YCSingleTexture2D : 2D ��������ֻ��һ��ͼƬ��
 */

class YCSingleTexture2D : public YCITexture2D
{
public:

	//
	// ������YCSingleTexture2D(void)
	//
	// Ŀ�ģ����캯����ת������
	//
	YCSingleTexture2D(YCITextureFile *texture, int status, int uniqueId);

	//
	// ������~YCSingleTexture2D(void)
	//
	// Ŀ�ģ������������ͷ�����
	//
	virtual ~YCSingleTexture2D(void);

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

	unsigned int myUniqueId;
};

#endif