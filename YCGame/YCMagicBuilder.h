#ifndef _INCLUDE_YCMAGICBUILDER_H_
#define _INCLUDE_YCMAGICBUILDER_H_

class YCMagic;

/*
 * YCMagicBuilder : ���ܹ���������
 */
struct YCMagicBuilder
{
	//
	// ������build(void* response)
	//
	// Ŀ�ģ��ӷ��������ع�������
	//
	static YCMagic* build(void* response);
};

#endif

