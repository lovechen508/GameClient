#ifndef _INCLUDE_YCBUFFMANAGER_H_
#define _INCLUDE_YCBUFFMANAGER_H_

#include "YCGraphic\YCIModel.h"

class YDList;
class YCConfig;

//
// Buff 管理器
//
class YCBuffManager : public YCIModel
{
public:

	YCBuffManager(void);

	virtual ~YCBuffManager(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：初始化消息管理器
	//
	bool initialize(YCConfig& config);



	//
	// 函数：finalize()
	//
	// 目的：释放资源
	//
	void finalize();

private:

	YDList* myBuffMetas;
};

#endif

