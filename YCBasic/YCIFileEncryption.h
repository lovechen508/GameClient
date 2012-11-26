#ifndef _INCLUDE_YCIFILEENCRYPTION_H_
#define _INCLUDE_YCIFILEENCRYPTION_H_

 /*
  * YCIFileEncryption : �ļ��ӽ��ܽӿ���
  *
  * ע�ͣ���Ϊ��DLL�����ӿ��в���Ҫ��STL
  *       ����Ŀǰֻ��֧�ֻ����ļ����ļӽ���
  */
class YCAPI YCIFileEncryption
{
public:

	YCIFileEncryption(void);

	virtual ~YCIFileEncryption(void);

	//
	// ������encoder(const char *inFile, const char *outFile)
	//
	// Ŀ�ģ������ļ����ļ���
	//
	virtual bool encode(const char *inFile, const char *outFile) = 0;

	//
	// ������decoder(const char *inFile, const char *outFile)
	//
	// Ŀ�ģ������ļ����Ľ���
	//
	virtual bool decode(const char *inFile, const char *outFile) = 0;

};

#endif

