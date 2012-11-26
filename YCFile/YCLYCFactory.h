#ifndef _INCLUDE_YCLYCFACTORY_H_
#define _INCLUDE_YCLYCFACTORY_H_

class YCDList;
class YCIPackFileBuilder;

/*
 * YCLYCFactory : YCIPackFileBuilder构建工厂类
 */
class YCLYCFactory
{
public:

	YCLYCFactory();

	virtual ~YCLYCFactory();

	//
	// 注释：注册工厂类
	//
	void regist(YCIPackFileBuilder* builder);

	//
	// 注释：从当前文件查找工厂类
	//
	YCIPackFileBuilder* getBuilder(const char* packFile);

	//
	// 注释：从版本号查找工厂类
	//
	YCIPackFileBuilder* getBuilder(unsigned int version);

	//
	// 注释：释放所有工厂类
	//
	void finalize();

private:

	YCDList* myBuilders;
};

#endif