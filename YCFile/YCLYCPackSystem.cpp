#include "YCLYCPackSystem.h"

#include "YCLYCDefine.h"
#include "YCLYCFactory.h"
#include "YCLYCFilePack.h"
#include "YCLYCFileBuilder1.h"
#include "YCLYCFileDirectory.h"

#include "YCBasic\YCIAbstractFile.h"
#include "YCBasic\YCIFileNormal.h"
#include "YCBasic\YCIFilePack.h"
#include "YCBasic\YCIFileDirectory.h"

#include "YCBasic\YCDlist.h"
#include "YCBasic\YCLogger.h"
#include "YCBasic\YCAssert.h"
#include "YCBasic\YCFileUtil.h"
#include "YCBasic\YCException.h"
#include "YCBasic\YCScopeGuard.h"
#include "YCBasic\YCPathWalker.h"
#include "YCBasic\YCStringHelper.h"
#include "YCBasic\YCPackException.h"

#pragma region handle
//////////////////////////////////////////////////////
// YCFileUtil::HandleFileInDir 辅助函数
//////////////////////////////////////////////////////

static void* mountPackFile(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "mountPackFile未设置YCLYCFileSystem句柄");
	}

	char fullname[MAX_PATH] = {0};
	sprintf(fullname, "%s\\%s", path, filename);
	YCLYCPackSystem* fileSystem = (YCLYCPackSystem*)customData;
	fileSystem->open(fullname);

	return NULL;
}

static void* umountPackFile(const char* path, const char* filename, void* customData)
{
	SMART_ASSERT(customData != NULL);
	if (customData == NULL)
	{
		throw YCPackException(1002, "mountPackFile未设置YCLYCFileSystem句柄");
	}

	char fullname[MAX_PATH] = {0};
	sprintf(fullname, "%s\\%s", path, filename);
	YCLYCPackSystem* fileSystem = (YCLYCPackSystem*)customData;
	fileSystem->close(fullname);

	return NULL;
}

#pragma endregion handle

YCLYCPackSystem::YCLYCPackSystem(void)
	: myFactory(NULL)
	, myMounted(NULL)
{
}


YCLYCPackSystem::~YCLYCPackSystem(void)
{
	SAFE_DELETE(myFactory);
	SAFE_DELETE(myMounted);
}

//
// 注释：初始化文件系统
//
bool YCLYCPackSystem::initialize()
{
	// 创建构建器
	SMART_ASSERT(myFactory == NULL);
	if (myFactory != NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::initialize重复初始化");
	}

	myFactory = new YCLYCFactory();
	myFactory->regist(new YCLYCFileBuilder1());

	return true;
}

//
// 注释：添加文件搜索路径
//
bool YCLYCPackSystem::mount(const char* regulePath)
{
	SMART_ASSERT(regulePath != NULL);
	if (regulePath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::mount路径为空");
	}

	if (!YCFileUtil::IsFileExist(regulePath))
	{
		throw YCPackException(1002, "YCLYCFileSystem::mount路径不存在");
	}

	std::string pathWithLYCFilter(regulePath);
	pathWithLYCFilter += LYC_FILE_EXTENSION;
	YCFileUtil::HandleFileInDir(pathWithLYCFilter.c_str(), mountPackFile, this);

	return true;
}

//
// 注释：取消文件搜索路径
//
bool YCLYCPackSystem::umount(const char* regulePath)
{
	SMART_ASSERT(regulePath != NULL);
	if (regulePath == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::umount路径为空");
	}

	if (!YCFileUtil::IsFileExist(regulePath))
	{
		throw YCPackException(1002, "YCLYCFileSystem::umount路径不存在");
	}

	std::string pathWithLYCFilter(regulePath);
	pathWithLYCFilter += LYC_FILE_EXTENSION;
	YCFileUtil::HandleFileInDir(pathWithLYCFilter.c_str(), umountPackFile, this);

	return true;
}

//
// 注释：打开一个压缩包文件
//
//  内部函数，用于mount时读取，不对外提供op
//
bool YCLYCPackSystem::open(const char * packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open打开文件为空");
	}

	// 是否已加载
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCIFilePack* thePack = (YCIFilePack*)myPackFiles->payload(item);
		if (stricmp(thePack->filename(), packFile) == 0)
		{
			return true;
		}
	}

	LYC_PACK_HEAD head;
	FILE* fd = fopen(packFile, "r");
	SCOPE_GUARD(fd_guard, fclose(fd));
	if (fd == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open读取文件失败", packFile);
	}

	if (1 != fread(&head, sizeof(LYC_PACK_HEAD), 1, fd))
	{
		throw YCPackException(1002, "YCLYCFileSystem::open读取全局文件头失败", packFile);
	}

	YCIPackFileBuilder* builder = myFactory->getBuilder(head.version);
	if (builder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename获取构建器失败",packFile);
	}

	YCIFilePack* result = builder->loadPack(packFile);
	if (result == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename创建压缩包文件失败", packFile);
	}

	myPackFiles->append(result, NULL);

	return true;
}

//
// 注释：关闭一个压缩包文件
//
//  内部函数，用于umount时读取，不对外提供op
//
bool YCLYCPackSystem::close(const char * packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::open打开文件为空");
	}

	YCIFilePack* found = NULL;
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCIFilePack* exist = (YCIFilePack*)myPackFiles->payload(item);
		if (stricmp(packFile, exist->getPackName()) == 0)
		{
			found = exist;
			myPackFiles->remove(item);
			break;
		}
	}
	
	SAFE_DELETE(found);

	return true;
}

//
// 注释：创建一个压缩包文件
//      
//       op : touch version xxx.ycp
//
YCIFilePack* YCLYCPackSystem::create(const char * filename, unsigned int version)
{
	SMART_ASSERT(filename != NULL);
	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename文件名为空");
	}

	if (YCFileUtil::IsFileExist(filename))
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename文件已存在");
	}

	YCIPackFileBuilder* builder = myFactory->getBuilder(version);
	if (builder == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename获取构建器失败");
	}

	YCIFilePack* result = builder->createPack(filename);
	if (result == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::filename创建压缩包文件失败");
	}

	myPackFiles->append(result, NULL);

	return result;
}

//
// 注释：删除一个压缩包文件
//       如果当前路径在该压缩包文件中，删除失败
//
//       op : rm xxx.ycp
//
void YCLYCPackSystem::remove(const char* packFile, bool force)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::remove删除压缩包文件为空");
	}

	if (!YCFileUtil::IsFileExist(packFile))
	{
		throw YCPackException(1002, "YCLYCFileSystem::remove删除压缩包文件文件不存在");
	}

	bool found = false;
	for (Item_List_T* item = myPackFiles->begin();
		 item != myPackFiles->end();
		 item = myPackFiles->next(item))
	{
		YCLYCFilePack* thePack = (YCLYCFilePack*)myPackFiles->payload(item);
		if (stricmp(packFile, thePack->getPackName()) == 0)
		{
			if (myCurrent->getPack() == thePack)
			{
				throw YCPackException(1002, "YCLYCFileSystem::remove删除压缩包文件在使用中");
			}
			found = true;
			myPackFiles->remove(item);
			break;
		}
	}

	if (found)
	{
		YCFileUtil::RemoveFile(packFile);
	}
}

//
// 注释：尝试碎片整理
//
//  op ：defrag pathInPack
//
void YCLYCPackSystem::defrag(const char* pathInPack)
{

}

//
// 注释：添加一个常规磁盘reguleFile文件到pathInPack路径对应的压缩包文件中
//       对应路径下有同名文件则看overwrite是否为true
//       是则覆盖，否则抛出YCPackException异常
//
//     op : copy -f c:\pic\xxx.png .
//
bool YCLYCPackSystem::copy( const char* reguleFile, const char* pathInPack, bool overwrite )
{
	SMART_ASSERT(reguleFile != NULL);
	if (reguleFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::copy源文件为NULL");
	}

	if (pathInPack == NULL || strlen(pathInPack) == 0)
	{
		throw YCPackException(1002, "YCLYCFileSystem::copy目标位置为空");
	}

	if (!YCFileUtil::IsFileExist(reguleFile))
	{
		throw YCPackException(1002, "YCLYCFileSystem::copy源文件不存在");
	}

	if (YCFileUtil::IsDir(reguleFile))
	{
		//递归检查文件数是否大于FILES_IN_DIR
	}
	//else
	//{
	//	if (YCFileUtil::GetFileLength(reguleFile) == 0)
	//	{
	//		throw YCPackException(1002, "YCLYCFileSystem::copy源文件为空");
	//	}
	//}
	

	YCLYCFilePack* thePack = NULL;
	YCIFileDirectory* position = NULL;
	const char* remain = NULL;

	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		position = myCurrent;
		remain = pathInPack;
	}
	else
	{
		YCPathWalker walker(pathInPack);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		const char* first = walker.next(length, type);

		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			thePack = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(thePack->getPackName(), first, length) == 0)
			{
				position = thePack;
			}
		}

		if (position == NULL)
		{
			throw YCPackException(1002, "YCLYCFileSystem::copy目录未找到", first);
		}

		remain = walker.remain();
	}

	// 沿remain递归进入子目录，如果不存在则创建
	if (remain != NULL)
	{
		if (!position->has(remain))
		{
			position->mkdir(remain, true);
		}

		position = (YCIFileDirectory*)position->get(remain);
		if (position == NULL)
		{
			return false;
		}
	}	
	
	if (!position->add(reguleFile, overwrite))
	{
		return false;
	}

	thePack->flush();
	return true;
}

//
// 注释：从压缩包中删除一个文件，删除失败或文件不存在抛出YCPackException异常
//     
//   op : rm -f /pathInPack/xxx.png
//
void YCLYCPackSystem::erase(const char* fileInPack)
{
	SMART_ASSERT(fileInPack != NULL);
	if (fileInPack == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::erase删除文件名为空");
	}

	YCIFileDirectory* position = NULL;

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(fileInPack) && myCurrent != NULL)
	{
		myCurrent->remove(fileInPack);
		myCurrent->getPack()->flush();
	}
	else
	{
		YCPathWalker walker(fileInPack);
		int length;
		YCPathWalker::E_FILE_TYPE type;
		const char* first = walker.next(length, type);
		if (first == NULL || type == YCPathWalker::DISK)
		{
			throw YCPackException(1002, "YCLYCFileSystem::erase包路径有误");
		}

		YCIFileDirectory * position = NULL;
		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(exists->getPackName(), first, length) == 0)
			{
				position = exists;
			}
		}

		if (NULL == position)
		{
			throw YCPackException(1002, "YCLYCFileSystem::erase包文件不存在");
		}

		const char* remain = walker.remain(false);

		//移除包文件
		if (remain == NULL) 
		{
			throw YCPackException(1002, "YCLYCPackSystem::erase移除包文件请使用rm -f");
		}

		// 移除包文件下的所有文件
		if (strlen(remain) == 1 && *remain == '\\')
		{
			position->clear();
		}
		else
		{
			position->remove(remain);
		}

		position->getPack()->flush();
	}
}

//
// 注释：添加内存文件到包文件中
//       pathInPack可以使相对路径，也可以使绝对路径
//       length > 0 必须大于零
//
bool YCLYCPackSystem::add(const char* filename, const char* pathInPack, const char* content, unsigned int length, bool force)
{
	SMART_ASSERT(filename != NULL);
	SMART_ASSERT(pathInPack != NULL);
	SMART_ASSERT(content != NULL && length > 0);

	if (filename == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add内存文件名为空");
	}

	if (pathInPack == NULL)
	{
		throw YCPackException(1002, "YCLYCFilePack::add包路径为空");
	}

	if (content == NULL || length == 0)
	{
		throw YCPackException(1002, "YCLYCFilePack::add内存文件为空");
	}

	YCIFileDirectory* position = NULL;

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		position = myCurrent; 
	}
	else
	{
		YCPathWalker walker(pathInPack);
		int pathLength;
		YCPathWalker::E_FILE_TYPE type;
		const char* first = walker.next(pathLength, type);
		if (first == NULL || type == YCPathWalker::DISK)
		{
			throw YCPackException(1002, "YCLYCFileSystem::add包路径有误");
		}

		YCIFileDirectory * position = NULL;
		for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
		{
			YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
			if (strnicmp(exists->getPackName(), first, pathLength) == 0)
			{
				position = exists;
			}
		}

		if (NULL == position)
		{
			throw YCPackException(1002, "YCLYCFileSystem::add包文件不存在");
		}
	}

	position->add(filename, pathInPack, content, length, force);
	position->getPack()->flush();

	return false;
}

//
// 注释：在压缩包中查找文件，pathInPack为查找路径
//       如果pathInPack为空，则在当前路径下查找
//       如果pathInPack 为绝对路径，则从根目录下查找
//       返回查找到的第一个文件，如果没有找到，返回NULL
// 
//  op : find 0001.png /ui/0/    <= 绝对路径
//       find 0001.png           <= 当前路径下
//
YCIAbstractFile* YCLYCPackSystem::find(const char* packFile, const char* pathInPack)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find参数有误");
	}

	char nextPath[MAX_PATH] = {0};
	if (!YCPathWalker::isAbsolutePath(pathInPack) && myCurrent != NULL)
	{
		sprintf(nextPath, "%s\\%s", pathInPack, packFile);
		return myCurrent->get(nextPath);
	}

	YCPathWalker walker(pathInPack);
	int length;
	YCPathWalker::E_FILE_TYPE type;
	const char* first = walker.next(length, type);
	if (first == NULL || type == YCPathWalker::DISK)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find包路径有误");
	}

	YCIFileDirectory * position = NULL;
	for (Item_List_T* item = myPackFiles->begin();
			item != myPackFiles->end() && !position;
			item = myPackFiles->next(item))
	{
		YCLYCFilePack* exists = (YCLYCFilePack*)myPackFiles->payload(item);
		if (strnicmp(exists->getPackName(), first, length) == 0)
		{
			position = exists;
		}
	}

	if (NULL == position)
	{
		throw YCPackException(1002, "YCLYCFileSystem::find包文件不存在");
	}

	sprintf(nextPath, "%s\\%s", walker.remain(), packFile);
	return position->get(nextPath);
}

//
// 注释：查看一个文件的详细信息
// 
//   op : file /pathInPack/xxx.png
//
YCIAbstractFile* YCLYCPackSystem::file(const char* packFile)
{
	SMART_ASSERT(packFile != NULL);
	if (packFile == NULL || strlen(packFile) == 0)
	{
		throw YCPackException(1002, "YCLYCFileSystem::file无文件名参数");
	}

	YCIAbstractFile* found = NULL;

	try
	{
		// 绝对路径 or 相对路径
		YCIFileDirectory* position = (packFile[0] == '\\') ? NULL : myCurrent;

		std::vector<std::string> dirs;
		YCStringHelper::split(packFile, "\\", &dirs);
		for (unsigned int i = 0; i < dirs.size(); ++i)
		{
			const std::string& dir = dirs[i];
			if (position == NULL) 
			{
				for (Item_List_T* item = myPackFiles->begin();
					item != myPackFiles->end();
					item = myPackFiles->next(item))
				{
					YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
					if (stricmp(pack->getPackName(), dir.c_str()) == 0)
					{
						position = pack;
					}
				}

				if (position == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::file目录未找到", dir.c_str());
				}
			}
			else
			{

			}
		}
	}
	catch (YCPackException& e)
	{
		LOG_ERROR("YCLYCFileSystem::file查找文件" << packFile << "出错：" << e.what());
	}

	return found;
}

//
// 注释：dump一个压缩packFile文件到pathInRegult路径对应的磁盘文件系统中
//       对应路径下有同名文件则看overwrite是否为true
//       是则覆盖，否则抛出YCPackException异常
//
//   op : dump -f /pathInPack/xxx.png c:/pic/
//
void YCLYCPackSystem::dump(const char* packFile, const char* pathInRegule, bool overwrite)
{
	SMART_ASSERT(packFile != NULL);
	SMART_ASSERT(pathInRegule != NULL);

	if (packFile == NULL)
	{
		throw YCPackException(1002, "YCLYCPackSystem::dump源文件为空");
	}

	if (pathInRegule == NULL)
	{
		throw YCPackException(1002, "YCLYCPackSystem::dump目标文件夹为空");
	}

	if (!YCFileUtil::IsDir(pathInRegule))
	{
		throw YCPackException(1002, "YCLYCPackSystem::dump目标文件夹不存在");
	}

	YCPathWalker walker(packFile);
	int length;
	YCPathWalker::E_FILE_TYPE type;

	YCIFileDirectory* position = NULL;
	if (!YCPathWalker::isAbsolutePath(packFile) && myCurrent != NULL)
	{
		position = myCurrent; //当前路径下查找
	}
		
	const char* first = NULL;
	while ((first = walker.next(length, type)) != NULL) //递归进入子目录
	{
		std::string current(first, length);

		//从包文件根进入
		if (position == NULL)
		{
			for (Item_List_T* item = myPackFiles->begin();
				item != myPackFiles->end() && !position;
				item = myPackFiles->next(item))
			{
				YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
				if (stricmp(pack->getPackName(), current.c_str()) == 0)
				{
					position = pack;
				}
			}

			if (position == NULL)
			{
				throw YCPackException(1002, "YCLYCFileSystem::dump目录未找到", current.c_str());
			}
		}
		else
		{
			YCIAbstractFile* abstractFile = position->get(current.c_str());
			if (type == YCPathWalker::DIR) //必须是文件夹
			{
				if (abstractFile->type() != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCPackSystem::dump文件类型不一致", current.c_str());
				}

				position = (YCIFileDirectory*)abstractFile;				
			}
		}
	}

	//遍历结束，可能是文件也可能是文件夹
	if (position != NULL)
	{
		position->dump(pathInRegule, overwrite);
	}
}

//
// 注释：cd 进入文件目录
//       \开头为绝对路径
//
//   op : cd /packFileWithoutExtension/pathInPack/  根目录进入
//        cd pathInPack     相对路径进入
//
bool YCLYCPackSystem::cd(const char* path)
{
	if (path == NULL || strlen(path) == 0)
	{
		throw YCPackException(2002, "YCLYCFileSystem::cd命令必须提供路径");
	}

	try
	{
		YCPathWalker walker(path);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		YCIFileDirectory* position = NULL;
		if (!YCPathWalker::isAbsolutePath(path) && myCurrent != NULL)
		{
			position = myCurrent;
		}

		for (const char* first = walker.next(length, type); first != NULL; first = walker.next(length, type))
		{
			std::string current(first, length);
			if (position == NULL) //根
			{
				for (Item_List_T* item = myPackFiles->begin();
					item != myPackFiles->end();
					item = myPackFiles->next(item))
				{
					YCLYCFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
					if (stricmp(pack->getPackName(), current.c_str()) == 0)
					{
						position = pack;
					}
				}

				if (position == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cd目录未找到", current.c_str());
				}
			}
			else
			{
				YCIAbstractFile* file = position->get(current.c_str());
				if (file == NULL)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cd目录未找到", current.c_str());
				}

				if (file->type() != YCIAbstractFile::FILE_DIR)
				{
					throw YCPackException(1002, "YCLYCFileSystem::cd不是目录文件", current.c_str());
				}
				position = (YCIFileDirectory*)file;
			}
		}
		
		myCurrent = position;
	}
	catch (YCPackException& e)
	{
		LOG_ERROR("YCLYCFileSystem:cd错误：" << e.what());
		return false;
	}	

	return true;
}

//
// 注释：创建子目录
//
// op ：mkdir /packFileWithoutExtension/pathInPack/newDir  根目录进入
//      mkdir pathUnderCurrentPath/newDir 相对路径进入
//
bool YCLYCPackSystem::mkdir(const char* dirname, bool recursive)
{
	if (dirname == NULL || strlen(dirname) == 0)
	{
		throw YCPackException(2002, "YCLYCFileSystem::mkdir命令必须提供路径");
	}

	bool result = false;
	try
	{
		YCPathWalker walker(dirname);
		int length;
		YCPathWalker::E_FILE_TYPE type;

		YCIFileDirectory* position = NULL;
		if (YCPathWalker::isAbsolutePath(dirname) && myCurrent != NULL)
		{
			position = myCurrent;
		}

		if (position == NULL) //包
		{
			const char* first = walker.next(length, type);
			if (first == NULL)
			{
				throw YCPackException(2002, "YCLYCFileSystem::mkdir命令路径有误", dirname);
			}

			for (Item_List_T* item = myPackFiles->begin();
				item != myPackFiles->end() && !position;
				item = myPackFiles->next(item))
			{
				YCIFilePack* pack = (YCLYCFilePack*)myPackFiles->payload(item);
				if (strnicmp(pack->getPackName(), first, length) == 0)
				{
					position = pack;
				}
			}

			// 根目录无法创建，实际上是包文件
			if (position == NULL)
			{
				throw YCPackException(1002, "YCLYCFileSystem::mkdir目录未找到", first);
			}

			const char* remain = walker.remain();
			if (remain != NULL)
			{
				result = position->mkdir(remain, recursive);
			}
			else
			{
				result = true; // 什么都不做
			}
		}
		else  //当前目录
		{
			result = position->mkdir(dirname, recursive);
		}
	}
	catch (YCPackException& e)
	{
		UNREFERENCED_PARAMETER(e);
	}

	return result;
}


//
// 注释：释放文件系统
//
void YCLYCPackSystem::finalize()
{
	if (myPackFiles->size() > 0)
	{
		// 刷新到磁盘
		for (Item_List_T* item = myPackFiles->begin();
			 item != myPackFiles->end();
			 item = myPackFiles->next(item))
		{
			YCLYCFilePack* thePack = (YCLYCFilePack*)myPackFiles->payload(item);
			thePack->flush();
		}

		myPackFiles->clear(YCDListClearHelper<YCIFilePack>);
	}

	if (myFactory != NULL)
	{
		myFactory->finalize();
	}
}