#include "YCDataHolder.h"

#include <stdlib.h>
#include <winsock2.h>
#include <string>

#include "YCBasic\YCAssert.h"

static inline unsigned long long htonll(unsigned long long number)
{
	return ( htonl((unsigned long)((number >> 32) & 0xFFFFFFFF)) |
		((unsigned long long)(htonl((unsigned long)(number & 0xFFFFFFFF))) << 32 ));
}

static inline unsigned long long ntohll(unsigned long long number)
{
	return ( ntohl((unsigned long)((number >> 32) & 0xFFFFFFFF)) |
		((unsigned long long)(ntohl((unsigned long)(number & 0xFFFFFFFF))) << 32 ));
}

static inline int myStrlen(const char *pszString, int iSize)
{
	const char *ptr = pszString;
	for (int i = 0; i < iSize; i++, ptr++)
	{
		if ('\0' == *ptr) return i;
	}
	return iSize;
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

YCDataHolder::YCDataHolder(char *buf, int len)
	: myBuf(buf), myLen(len), myPos(0)
{
	SMART_ASSERT(myBuf != NULL);
	SMART_ASSERT(len > 0);
}

YCDataHolder::~YCDataHolder()
{
}

void YCDataHolder::reset()
{
	myPos = 0;
}

const char* YCDataHolder::data()
{
	return myBuf;
}

int YCDataHolder::length()
{
	return myLen;
}

int YCDataHolder::addByte(unsigned char byByte)
{
	if (myPos + (int)sizeof(byByte) > myLen)
	{
		return -1;
	}

	*(unsigned char*)(myBuf+myPos) = byByte;
	myPos += sizeof(byByte);
	return myPos;
}

int YCDataHolder::delByte(unsigned char* byByte)
{
	if (myPos + (int)sizeof(unsigned char) > myLen)
	{
		return -1;
	}
	*byByte = *(unsigned char*)(myBuf+myPos);
	myPos += sizeof(byByte);
	return myPos;
}

int YCDataHolder::addChar(char chChar)
{
	if (myPos + (int)sizeof(chChar) > myLen)
	{
		return -1;
	}

	*(char*)(myBuf+myPos) = chChar;
	myPos += sizeof(chChar);
	return myPos;
}

int YCDataHolder::delChar(char *chChar)
{
	if (myPos + (int)sizeof(char) > myLen)
	{
		return -1;
	}
	*chChar = *(char*)(myBuf+myPos);
	myPos += sizeof(char);
	return myPos;
}

int YCDataHolder::addWord(unsigned short wWord)
{
	if (myPos + (int)sizeof(unsigned short) > myLen)
	{
		return -1;
	}

	*(unsigned short*)(myBuf+myPos) = htons(wWord);
	myPos += sizeof(unsigned short);
	return myPos;
}

int YCDataHolder::delWord(unsigned short *wWord)
{
	if (myPos + (int)sizeof(unsigned short) > myLen)
	{
		return -1;
	}

	*wWord = ntohs(*(unsigned short*)(myBuf+myPos));
	myPos += sizeof(unsigned short);
	return myPos;
}

int YCDataHolder::addShort(short shShort)
{
	if (myPos + (int)sizeof(short) > myLen)
	{
		return -1;
	}

	*(short*)(myBuf+myPos) = htons(shShort);
	myPos += sizeof(short);
	return myPos;
}

int YCDataHolder::delShort(short *shShort)
{
	if (myPos + (int)sizeof(short) > myLen)
	{
		return -1;
	}

	*shShort = ntohs(*(short*)(myBuf+myPos));
	myPos += sizeof(short);
	return myPos;    
}

int YCDataHolder::addDword(unsigned int dwWord)
{
	if (myPos + (int)sizeof(unsigned int) > myLen)
	{
		return -1;
	}
	*(unsigned int*)(myBuf+myPos) = htonl(dwWord);
	myPos += sizeof(unsigned int);
	return myPos;
}

int YCDataHolder::delDword(unsigned int *dwWord)
{
	if (myPos + (int)sizeof(unsigned int) > myLen)
	{
		return -1;
	}

	*dwWord = ntohl(*(unsigned int*)(myBuf+myPos));
	myPos += sizeof(unsigned int);
	return myPos;
}

int YCDataHolder::addInt(int iInt)
{
	if (myPos + (int)sizeof(int) > myLen)
	{
		return -1;
	}

	*(int*)(myBuf+myPos) = htonl(iInt);
	myPos += sizeof(int);
	return myPos; 
}

int YCDataHolder::delInt(int *iInt)
{
	if (myPos + (int)sizeof(int) > myLen)
	{
		return -1;
	}

	*iInt = *(int*)(myBuf+myPos);
	myPos += sizeof(int);
	return myPos;
}

int YCDataHolder::addUint64(unsigned long long qwUint64)
{
	if (myPos + (int)sizeof(unsigned long long) > myLen)
	{
		return -1;
	}

	*(unsigned long long*)(myBuf+myPos) = htonll(qwUint64);
	myPos += sizeof(unsigned long long);
	return myPos;
}

int YCDataHolder::delUint64(unsigned long long *qwUint64)
{
	if (myPos + (int)sizeof(unsigned long long) > myLen)
	{
		return -1;
	}

	*qwUint64 = ntohll(*(unsigned long long*)(myBuf+myPos));
	myPos += sizeof(unsigned long long);
	return myPos;
}

int YCDataHolder::addInt64(long long llInt64)
{
	if (myPos + (int)sizeof(long long) > myLen)
	{
		return -1;
	}

	*(long long*)(myBuf+myPos) = htonll(llInt64);
	myPos += sizeof(llInt64);
	return myPos;
}

int YCDataHolder::delInt64(long long *llInt64)
{
	if (myPos + (int)sizeof(long long) > myLen)
	{
		return -1;
	}

	*llInt64 = ntohll(*(long long*)(myBuf+myPos));
	myPos += sizeof(llInt64);
	return myPos;
}

int YCDataHolder::addString(const char*pszString, int length)
{
	int iLen = myStrlen(pszString, length - 1);

	if (myPos + (int)sizeof(unsigned short) + iLen > myLen)
	{
		return -1;
	}

	if (-1 == addWord(iLen))
	{
		return -1;
	}

	memcpy(myBuf+myPos, pszString, iLen);
	myPos += iLen;
	return myPos;
}

int YCDataHolder::delString(char* pszOut, int bufSize)
{
	unsigned short wLen = 0;
	if (-1 == delWord(&wLen))
	{
		return -1;
	}

	if (myPos + (int)wLen > myLen)
	{
		return -1;
	}

	if ((int)(wLen+1) > bufSize)
	{
		return -1;
	}

	memcpy(pszOut, myBuf+myPos, wLen);
	pszOut[wLen] = '\0';
	myPos += wLen;
	return myPos;
}

