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
	// ������encoder(const char *inFile, const char *outFile)
	//
	// Ŀ�ģ������ļ����ļ���
	//
	virtual bool encode(const char *inFile, const char *outFile);

	//
	// ������decoder(const char *inFile, const char *outFile)
	//
	// Ŀ�ģ������ļ����Ľ���
	//
	virtual bool decode(const char *inFile, const char *outFile);

	//
	// ������encoder(const char *inBuf, int inLen, char * outBuf, int& outLen)
	//
	// Ŀ�ģ������ڴ�ӳ��ļ���
	//      ���inBuf == outBuf��ִ�о͵ؼ���
	//      Encode pIntput into pOutput.  Input length in lSize.  Returned value
	//		is length of output which will be even MOD 8 bytes.  Input buffer and
	//		output buffer can be the same, but be sure buffer length is even MOD8.
	//
	virtual bool encode(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen);

	//
	// ������decoder(const char *inBuf, unsigned int inLen, char * outBuf, unsigned int& outLen)
	//
	// Ŀ�ģ������ڴ�ӳ��Ľ���
	//      ���inBuf == outBuf��ִ�о͵ؽ���
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

