#include "YCITexture.h"

YCITexture::YCITexture(void)
{
}


YCITexture::~YCITexture(void)
{
}

//
// ������fetchUV()
//
// Ŀ�ģ��õ�UVֵ
//
void YCITexture::fetchUV(float& leftTopU,     float& leftTopV,      // V0         V0 ------ V1
						float& rightTopU,	 float& rightTopV,      // V1           |      |
						float& leftBottomU,	 float& leftBottomV,    // V2           |      |
						float& rightBottomU, float& rightBottomV)   // V3         V2 ------ V3
{
	leftTopU = myUV[0];      // V0
	leftTopV = myUV[1];

	rightTopU = myUV[2];     // V1 
	rightTopV = myUV[3];

	leftBottomU = myUV[4];   // V2
	leftBottomV = myUV[5];

	rightBottomU = myUV[6];  // V3
	rightBottomV = myUV[7];
}

//
// ������rect()
//
// Ŀ�ģ�����������������
//       ����NULL��ʾ����Ϊ��������
//
const FloatRect* YCITexture::rect() const
{
	return &myRect;
}

