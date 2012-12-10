#ifndef _INCLUDE_YCTASKMANAGER_H_
#define _INCLUDE_YCTASKMANAGER_H_

class YCConfig;

struct NET_TASK_MSG_NTF_T;

#include "YCGraphic/YCIModel.h"

/*
 * YCTaskManager：任务管理器
 *
 */
class YCAPI YCTaskManager : public YCIModel
{

public:

	YCTaskManager(void);

	virtual ~YCTaskManager(void);

	//
	// 函数：initialize(YCConfig& config)
	//
	// 目的：初始化任务管理器
	//
	bool initialize(YCConfig& config);

	//
	// 函数：recv(const NET_TASK_MSG_NTF_T* msg)
	//
	// 目的：消息到达
	//
	void recv(const NET_TASK_MSG_NTF_T* msg);

	//
	// 函数：finalize()
	//
	// 目的：释放资源
	//
	void finalize();

private:

    //
    //
    //

};

#endif

