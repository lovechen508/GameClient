#include "YCITexture.h"

YCITexture::YCITexture(void)
{
}


YCITexture::~YCITexture(void)
{
}

//
// 函数：fetchUV()
//
// 目的：得到UV值
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
// 函数：rect()
//
// 目的：返回纹理所在区域
//       返回NULL表示区域为整张纹理
//
const FloatRect* YCITexture::rect() const
{
	return &myRect;
}

