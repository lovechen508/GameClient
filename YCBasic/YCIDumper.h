#ifndef _INCLUDE_YCIDUMPER_H_
#define _INCLUDE_YCIDUMPER_H_

/*
 * YCIDumper : Dump ���ڲ�״̬��html
 */
class YCAPI YCIDumper
{
public:

	YCIDumper() {}

	virtual ~YCIDumper() {}

	//
	// ������dump()
	//
	// Ŀ�ģ�dump���ڲ�״̬ΪHtmlƬ��
	//
	virtual const char* dump() = 0;
};
#endif