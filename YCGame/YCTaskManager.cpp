#include "YCTaskManager.h"

#include "YCConfig\YCConfig.h"

YCTaskManager::YCTaskManager(void)
{
}


YCTaskManager::~YCTaskManager(void)
{
}

//
// ������initialize(YCConfig& config)
//
// Ŀ�ģ���ʼ�����������
//
bool YCTaskManager::initialize(YCConfig& config)
{
	return true;
}

//
// ������recv(const NET_TASK_MSG_NTF_T* msg)
//
// Ŀ�ģ���Ϣ����
//
void YCTaskManager::recv(const NET_TASK_MSG_NTF_T* msg)
{

}

//
// ������finalize()
//
// Ŀ�ģ��ͷ���Դ
//
void YCTaskManager::finalize()
{

}
