#ifndef _INCLUDE_YCTASKPARSER_H_
#define _INCLUDE_YCTASKPARSER_H_

struct NET_TASK_MSG_NTF_T;
class YCTaskMessage;

/*
 * YCChatParser : ������Ϣ������
 *
 * ��Ϣʵ��(Python)��
 *
 *      msg = """
 *			[start]
 *		talk ="<?xml version='1.0' encoding='GB2312'?>
 *			<p>
 *			<t color='ffff0000'>���鳾�����������������ٵ�ʱ��˭���޷����ܣ�
 *			</p>"
 *			1 = 'link' '�ָ��' 'dlg1'
 *			2 = 'server_cmd' '���ͻ��ͼ' '1'
 *			3 = 'link' '��ɱ���ѯ' 'dlg2'
 *			4 = 'link' '��ȡ�����' 'dlg3'
 *			5 = 'server_cmd' '�뿪' '2'
 *			[dlg1]
 *		talk ="�ָ��"
 *			%s
 *			2 = 'link' '���񿼺���' 'dlg12'
 *			3 = 'link' '����' 'start'
 *			[dlg11]
 *		talk =" ϵͳ���ڻ�ڼ��Զ���¼���н�ɫ��Ļ�ɱ��¼"
 *			1 = 'link' '����' 'start'
 *			[dlg12]
 *		talk =" ϵͳ���ڻ�ڼ��Զ���¼"
 *			1 = 'link' '����' 'start'
 *			[dlg2]
 *		talk =" ���а�"
 *			%s
 *			2 = 'server_cmd' '���񿼺���' '22'
 *			3 = 'link' '����' 'start'
 *			[dlg3]
 *		talk =" �����"
 *			%s
 *			2 = 'server_cmd' '���񿼺���' '32'
 *			3 = 'link' '����' 'start'
 *			"""%(GetFormatString(1), GetFormatString(2), GetFormatString(3))
 *
 */
struct YCAPI YCTaskParser
{
	//
	// ������parse(const NET_TASK_MSG_T* msg)
	//
	// Ŀ�ģ����������ݰ������ڽ���Ϣ��Ԫ
	//
	static YCTaskMessage* parse(const NET_TASK_MSG_NTF_T* msg);
};

#endif

