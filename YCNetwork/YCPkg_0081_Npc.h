#ifndef _INCLUDE_YCPKG_0081_TASK_H_
#define _INCLUDE_YCPKG_0081_TASK_H_

/*
 *  ������Ϣ����ṹ��
 */
struct NET_TASK_MSG_REQ_T
{
	unsigned char	type;
	unsigned char	self;
};

/*
 *  ������Ϣ��Ӧ�ṹ��
 */
struct NET_TASK_MSG_NTF_T
{
	unsigned int	count;
	char			message[2048];
};

#endif