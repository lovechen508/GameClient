#ifndef _INCLUDE_YCISHELLCOMMAND_H_
#define _INCLUDE_YCISHELLCOMMAND_H_

class YCShell;

/*
 * YCIShellCommand : Shell��������ӿ���
 */
class YCAPI YCIShellCommand
{
public:

	YCIShellCommand();

	virtual ~YCIShellCommand(void);

	//
	// ע�ͣ����������� �� cd, rm, copy, touch ...
	//
	virtual const char* name() = 0;

	//
	// ע�ͣ��󶨵�Shell����
	//
	virtual void bind(YCShell* shell) = 0;

	//
	// ע�ͣ�����Shellת��������
	//
	virtual bool handle(const char* cmd, int len) = 0;

protected:

	//
	// ���ԣ�����Shell����
	//
	YCShell * myShell;
};

#endif

