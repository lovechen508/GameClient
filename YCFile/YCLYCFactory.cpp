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
// 注释：注册工厂类
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
			throw YCPackException(1002, "YCLYCFactory::regist相同版本的创建器已存在：", version);
		}
	}

	myBuilders->append(builder, NULL);
}

//
// 注释：从当前文件查找工厂类
//
YCIPackFileBuilder* YCLYCFactory::getBuilder(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);

	if (!YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFactory::getBuilder文件不存在", packFile);
	}

	if (!YCFileUtil::IsDir(packFile))
	{
		throw YCPackException(1002, "YCLYCFactory::getBuilder文件是目录", packFile);
	}

	FILE * fd = fopen(packFile, "rt+");
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFactory::build读取文件失败", packFile);
	}

	LYC_PACK_HEAD head;

	// 尝试读入文件头
	size_t result = fread(&head, sizeof(LYC_PACK_HEAD), 1, fd);
	if (result != 1)
	{
		fclose(fd);
		throw YCPackException(1002, "YCLYCFactory::build读入文件头失败，可能文件已损坏", packFile);
	}

	fclose(fd);

	return getBuilder(head.version);
}

//
// 注释：从版本号查找工厂类
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
// 注释：释放所有工厂类
//
void YCLYCFactory::finalize()
{
	try 
	{
		myBuilders->clear(YCDListClearHelper<YCIPackFileBuilder>);
	}
	catch (YCException& e)
	{
		LOG_ERROR("YCLYCFactory::finalize释放工厂失败：" << e.what());
	}
}