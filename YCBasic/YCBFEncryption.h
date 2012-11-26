#ifndef _INCLUDE_YCBFENCRYPTION_H_
#define _INCLUDE_YCBFENCRYPTION_H_

#include "YCDef.h"

#include "YCIFileEncryption.h"
#include "YCIStreamEncryption.h"

class YCAPI YCBFEncryption 
	: public YCIFileEncryption
	, public YCIStreamEncryption
{
public:

	YCBFEncryption(const char *pwd);
	
	virtual ~YCBFEncryption(void);

	//
	// 函数：encoder(const char *inFile, const char *outFile)
	//
	// 目的：基于文件名的加密
	//
	virtual bool encode(const char *inFile, const char *outFile);

	//
	// 函数：decoder(const char *inFile, const char *outFile)
	//
	// 目的：基于文件名的解密
	//
	virtual bool decode(const char *inFile, const char *outFile);

	//
	// 函数：encoder(const char *inBuf, int inLen, char * outBuf, int& outLen)
	//
	// 目的：基于内存映像的加密
	//      如果inBuf == outBuf，执行就地加密
	//      Encode pIntput into pOutput.  Input length in lSize.  Returned value
	//		is length of output which will be even MOD 8 bytes.  Input buffer and
	//		output buffer can be the same, but be sure buffer length is even MOD8.
	//
	virtual bool encode(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen);

	//
	// 函数：decoder(const char *inBuf, unsigned int inLen, char * outBuf, unsigned int& outLen)
	//
	// 目的：基于内存映像的解密
	//      如果inBuf == outBuf，执行就地解密
	//		Decode pIntput into pOutput.  Input length in lSize.  Input buffer and
	//		output buffer can be the same, but be sure buffer length is even MOD8.
	//
	virtual bool decode(const char *cipher, unsigned int inLen, char * plain, unsigned int& outLen);

private:

	char _encryptionKey[100];

	DWORD getOutputLength(DWORD lInputLong);

	//DWORD 	*PArray;
	//DWORD	(*SBoxes)[256];

	DWORD	PArray[18];
	DWORD	SBoxes[4][256];

	void 	Enc_encipher(DWORD *xl, DWORD *xr);
	void 	Enc_decipher(DWORD *xl, DWORD *xr);
};

union aword {
	DWORD dword;
	BYTE byte[4];
	struct {
		DWORD byte3:8;
		DWORD byte2:8;
		DWORD byte1:8;
		DWORD byte0:8;
	} w;
};

#endif

