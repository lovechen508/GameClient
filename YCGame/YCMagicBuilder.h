#ifndef _INCLUDE_YCMAGICBUILDER_H_
#define _INCLUDE_YCMAGICBUILDER_H_

class YCMagic;
struct YCIPackageResponse;

/*
 * YCMagicBuilder : ���ܹ���������
 */
struct YCMagicBuilder
{
	//
	// ������build(YCIPackageResponse* response)
	//
	// Ŀ�ģ��ӷ��������ع�������
	//
	static YCMagic* build(YCIPackageResponse* response);
};

#endif

