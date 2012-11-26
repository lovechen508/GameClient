#include "YCLYCFactory.h"

#include "YCLYCDefine.h"

#include "YCBasic\YCDList.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCIPackFileBuilder.h"
#include "YCBasic\YCPackException.h"

YCLYCFactory::YCLYCFactory() : myBuilders(new YCDList())
{
}

YCLYCFactory::~YCLYCFactory()
{
	SAFE_DELETE(myBuilders);
}

//
// ע�ͣ�ע�Ṥ����
//
void YCLYCFactory::regist(YCIPackFileBuilder* builder)
{
	SMART_ASSERT(builder != NULL);
	unsigned int version = builder->version();
	for (Item_List_T* item = myBuilders->begin();
		item != myBuilders->end();
		item = myBuilders->next(item))
	{
		YCIPackFileBuilder* exist = (YCIPackFileBuilder*)myBuilders->payload(item);
		if (exist->version() == version)
		{
			throw YCPackException(1002, "YCLYCFactory::regist��ͬ�汾�Ĵ������Ѵ��ڣ�", version);
		}
	}

	myBuilders->append(builder, NULL);
}

//
// ע�ͣ��ӵ�ǰ�ļ����ҹ�����
//
YCIPackFileBuilder* YCLYCFactory::getBuilder(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);

	if (!YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFactory::getBuilder�ļ�������", packFile);
	}

	if (!YCFileUtil::IsDir(packFile))
	{
		throw YCPackException(1002, "YCLYCFactory::getBuilder�ļ���Ŀ¼", packFile);
	}

	FILE * fd = fopen(packFile, "rt+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFactory::build��ȡ�ļ�ʧ��", packFile);
	}

	LYC_PACK_HEAD head;

	// ���Զ����ļ�ͷ
	size_t result = fread(&head, sizeof(LYC_PACK_HEAD), 1, fd);
	if (result != 1)
	{
		fclose(fd);
		throw YCPackException(1002, "YCLYCFactory::build�����ļ�ͷʧ�ܣ������ļ�����", packFile);
	}

	fclose(fd);

	return getBuilder(head.version);
}

//
// ע�ͣ��Ӱ汾�Ų��ҹ�����
//
YCIPackFileBuilder* YCLYCFactory::getBuilder(unsigned int version)
{
	YCIPackFileBuilder* found = NULL;
	for (Item_List_T* item = myBuilders->begin();
		item != myBuilders->end() && !found;
		item = myBuilders->next(item))
	{
		YCIPackFileBuilder* current = (YCIPackFileBuilder*)myBuilders->payload(item);
		if (current->version() == version)
		{
			found = current;
		}
	}
	return found;
}

//
// ע�ͣ��ͷ����й�����
//
void YCLYCFactory::finalize()
{
	try 
	{
		myBuilders->clear(YCDListClearHelper<YCIPackFileBuilder>);
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCLYCFactory::finalize�ͷŹ���ʧ�ܣ�" << e.what());
	}
}