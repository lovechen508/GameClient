#ifndef _INCLUDE_YCNETWORKFILELOADLISTENER_H_
#define _INCLUDE_YCNETWORKFILELOADLISTENER_H_

/*
 * YCNetworkFileLoadListener : ��������֪ͨ��
 */
class YCNetworkFileLoadListener
{
public:

	YCNetworkFileLoadListener() {};

	virtual ~YCNetworkFileLoadListener() {}

	//
	// ������notify(const char* filename, bool result)
	//
	// Ŀ�ģ������ļ����֪ͨ
	//
	// ������
	//       filename : ���ص��ļ���
	//       result   �����ؽ��
	//
	virtual void notify(const char* filename, bool result) = 0;
};

#endif