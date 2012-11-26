#ifndef _INCLUDE_YCIFILEENCRYPTION_H_
#define _INCLUDE_YCIFILEENCRYPTION_H_

 /*
  * YCIFileEncryption : 文件加解密接口类
  *
  * 注释：因为从DLL导出接口中不能要有STL
  *       所以目前只能支持基于文件名的加解密
  */
class YCAPI YCIFileEncryption
{
public:

	YCIFileEncryption(void);

	virtual ~YCIFileEncryption(void);

	//
	// 函数：encoder(const char *inFile, const char *outFile)
	//
	// 目的：基于文件名的加密
	//
	virtual bool encode(const char *inFile, const char *outFile) = 0;

	//
	// 函数：decoder(const char *inFile, const char *outFile)
	//
	// 目的：基于文件名的解密
	//
	virtual bool decode(const char *inFile, const char *outFile) = 0;

};

#endif

