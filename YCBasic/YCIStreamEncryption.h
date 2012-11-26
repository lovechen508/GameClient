#ifndef _INCLUDE_YCISTREAMENCRYPTION_H_
#define _INCLUDE_YCISTREAMENCRYPTION_H_

/*
 * YCIStreamEncryption : 内存数据加解密接口类
 *
 *   以及基于文件加载在内存中的加解密
 */
class YCAPI YCIStreamEncryption
{
public:

	YCIStreamEncryption(void);

	virtual ~YCIStreamEncryption(void);

	//
	// 函数：encoder(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen)
	//
	// 目的：基于内存映像的加密
	//
	virtual bool encode(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen) = 0;

	//
	// 函数：decoder(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen)
	//
	// 目的：基于内存映像的解密
	//
	virtual bool decode(const char *cipher, unsigned int inLen, char * plain, unsigned int& outLen) = 0;
};

#endif

