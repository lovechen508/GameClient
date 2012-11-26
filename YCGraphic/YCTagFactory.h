#ifndef _INCLUDE_YCTAGFACTORY_H_
#define _INCLUDE_YCTAGFACTORY_H_

class YCDList;
class YCIUITag;
class YCConfig;
class YCTagBuilder;
class YCUITagMeta;

class YCTagFactory
{
public:

	YCTagFactory(void);

	~YCTagFactory(void);

	//
	// ������initialize(YCConfig& config)
	//
	// Ŀ�ģ���ʼ����ǩ����
	//
	bool initialize(YCConfig& config);
		
	//
	// ������registerTag(YCUITagMeta* tag)
	//
	// Ŀ�ģ�ע���ǩ
	//
	void registerTag(YCUITagMeta* tagMeta);

	//
	// ������create(const char* tagName)
	//
	// Ŀ�ģ����ݱ�ǩ��������ǩ
	//
	YCIUITag* create(const char* tagName);

	//
	// ������finalize()
	//
	// Ŀ�ģ�����YCTagFactory
	//
	void finalize();

private:

	YCTagBuilder *myTagBuilder;

	YCDList *myTagRepository;
};

#endif

