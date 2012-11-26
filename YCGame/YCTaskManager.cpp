#include "YCTaskManager.h"

#include "YCConfig\YCConfig.h"

YCTaskManager::YCTaskManager(void)
{
}


YCTaskManager::~YCTaskManager(void)
{
}

//
// 函数：initialize(YCConfig& config)
//
// 目的：初始化任务管理器
//
bool YCTaskManager::initialize(YCConfig& config)
{
	return true;
}

//
// 函数：recv(const NET_TASK_MSG_NTF_T* msg)
//
// 目的：消息到达
//
void YCTaskManager::recv(const NET_TASK_MSG_NTF_T* msg)
{

}

//
// 函数：finalize()
//
// 目的：释放资源
//
void YCTaskManager::finalize()
{

}
