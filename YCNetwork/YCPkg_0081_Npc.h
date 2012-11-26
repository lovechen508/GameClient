#ifndef _INCLUDE_YCPKG_0081_TASK_H_
#define _INCLUDE_YCPKG_0081_TASK_H_

/*
 *  任务消息请求结构体
 */
struct NET_TASK_MSG_REQ_T
{
	unsigned char	type;
	unsigned char	self;
};

/*
 *  任务消息响应结构体
 */
struct NET_TASK_MSG_NTF_T
{
	unsigned int	count;
	char			message[2048];
};

#endif