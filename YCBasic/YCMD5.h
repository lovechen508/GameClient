#ifndef _INCLUDE_YCMD5_H_
#define _INCLUDE_YCMD5_H_

/*
 * YCMD5 : MD5 Class. 
 *
 * 说明：
 *
 *		YCMD5 md5;
 *		char final_string[16];
 *		md5.MD5Update(password.c_str(), password.Length());
 *		md5.MD5Final(final_string);
 *		return BCD_to_AnsiString(final_string,16);   //将MD5加密的验证码用16进制显示
 * 
 */
class YCAPI YCMD5 
{
public:
	YCMD5();
	virtual ~YCMD5();
	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);

private:

	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);

private:

	unsigned long int state[4];     /* state (ABCD) */
	unsigned long int count[2];     /* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */
};

#endif

