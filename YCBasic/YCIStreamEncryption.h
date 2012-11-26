#ifndef _INCLUDE_YCISTREAMENCRYPTION_H_
#define _INCLUDE_YCISTREAMENCRYPTION_H_

/*
 * YCIStreamEncryption : �ڴ����ݼӽ��ܽӿ���
 *
 *   �Լ������ļ��������ڴ��еļӽ���
 */
class YCAPI YCIStreamEncryption
{
public:

	YCIStreamEncryption(void);

	virtual ~YCIStreamEncryption(void);

	//
	// ������encoder(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen)
	//
	// Ŀ�ģ������ڴ�ӳ��ļ���
	//
	virtual bool encode(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen) = 0;

	//
	// ������decoder(const char *plain, unsigned int inLen, char * cipher, unsigned int& outLen)
	//
	// Ŀ�ģ������ڴ�ӳ��Ľ���
	//
	virtual bool decode(const char *cipher, unsigned int inLen, char * plain, unsigned int& outLen) = 0;
};

#endif

