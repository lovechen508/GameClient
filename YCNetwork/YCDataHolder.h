#ifndef _INCLUDE_YCDATAHOLDER_H_
#define _INCLUDE_YCDATAHOLDER_H_

/*
 * YCDataHolder : 数据装卸辅助类
 *
 * 说明：
 *      Add 类函数：从Host往NetData填数据
 *      Del 类函数：从NetData往Host填数据 
 */
class YCDataHolder
{
public:
	YCDataHolder(char* buf, int len);
	
	~YCDataHolder(void);

	void reset();

	const char* data();
	int length();

	int addByte(unsigned char byByte);
	int delByte(unsigned char* byByte);

	int addChar(char chChar);
	int delChar(char *chChar);

	int addWord(unsigned short wWord);
	int delWord(unsigned short *wWord);

	int addShort(short shShort);
	int delShort(short *shShort);

	int addDword(unsigned int dwWord);
	int delDword(unsigned int *dwWord);

	int addInt(int iInt);
	int delInt(int *iInt);

	int addUint64(unsigned long long qwUint64);
	int delUint64(unsigned long long *qwUint64);

	int addInt64(long long llInt64);
	int delInt64(long long *llInt64);

	int addString(const char*pszString, int length);
	int delString(char* pszOut, int bufSize);

private:
	char * myBuf;
	int myLen;
	int myPos;
};

#endif

