#ifndef _INCLUDE_YCITEMBUILDER_H_
#define _INCLUDE_YCITEMBUILDER_H_

class YCItem;
struct YCIPackageResponse;

struct YCItemBuilder
{
	//
	// ������build(YCIPackageResponse* response)
	//
	// Ŀ�ģ��ӷ��������ع�����Ʒ
	//
	static YCItem* build(YCIPackageResponse* response);
};

#endif