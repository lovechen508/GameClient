#ifndef _INCLUDE_YCIPACKPERSIST_H_
#define _INCLUDE_YCIPACKPERSIST_H_

//
// YCILYCPersist : �ӿ��� ��д�ļ��Ĵ���ѹ�������ӽ���
//
//    ���سɹ���
//         out : �ڴ����ڲ����䣬�ͻ������ͷ� SAFE_DELETE_ARRAY
//    
//    ����ʧ�ܣ�
//         out : �����ڴ�
//
class YCIPackPersist
{
public:

	virtual ~YCIPackPersist() {}

	//
	// д���ļ�ǰ�Ĵ������ܣ�ѹ����
	//
	virtual bool encode(const char* in, unsigned int ilen, char** out, unsigned int& olen) = 0;

	//
	// �����ļ��Ĵ�����ѹ�����ܵ�
	//
	virtual bool decode(const char* in, unsigned int ilen, char** out, unsigned int& olen) = 0;

};

#endif

