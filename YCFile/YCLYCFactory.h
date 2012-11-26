#ifndef _INCLUDE_YCLYCFACTORY_H_
#define _INCLUDE_YCLYCFACTORY_H_

class YCDList;
class YCIPackFileBuilder;

/*
 * YCLYCFactory : YCIPackFileBuilder����������
 */
class YCLYCFactory
{
public:

	YCLYCFactory();

	virtual ~YCLYCFactory();

	//
	// ע�ͣ�ע�Ṥ����
	//
	void regist(YCIPackFileBuilder* builder);

	//
	// ע�ͣ��ӵ�ǰ�ļ����ҹ�����
	//
	YCIPackFileBuilder* getBuilder(const char* packFile);

	//
	// ע�ͣ��Ӱ汾�Ų��ҹ�����
	//
	YCIPackFileBuilder* getBuilder(unsigned int version);

	//
	// ע�ͣ��ͷ����й�����
	//
	void finalize();

private:

	YCDList* myBuilders;
};

#endif