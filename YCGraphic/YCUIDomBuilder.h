#ifndef _INCLUDE_YCUIDOMBUILDER_H_
#define _INCLUDE_YCUIDOMBUILDER_H_

class YCUIDOM;

/*
 * YCUIDomBuilder : UI ���ڹ�����
 */

class YCUIDomBuilder 
{

public:

	//
	// ������build(const char* xmlFile)
	//
	// Ŀ�ģ���UIXml�����ļ��д���DOM
	//
	static YCUIDOM* build(const char* xmlFile);

	//
	// ������Parse(const char* strValue);
	//
	// Ŀ�ģ���strValue�ַ����д���DOM
	//
	static YCUIDOM* Parse(const char* strValue);
};

#endif