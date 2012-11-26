#ifndef _INCLUDE_YCISHELLCOMMAND_H_
#define _INCLUDE_YCISHELLCOMMAND_H_

class YCShell;

/*
 * YCIShellCommand : Shell命令处理器接口类
 */
class YCAPI YCIShellCommand
{
public:

	YCIShellCommand();

	virtual ~YCIShellCommand(void);

	//
	// 注释：处理器名称 如 cd, rm, copy, touch ...
	//
	virtual const char* name() = 0;

	//
	// 注释：绑定到Shell环境
	//
	virtual void bind(YCShell* shell) = 0;

	//
	// 注释：处理Shell转发的命令
	//
	virtual bool handle(const char* cmd, int len) = 0;

protected:

	//
	// 属性：所属Shell环境
	//
	YCShell * myShell;
};

#endif

