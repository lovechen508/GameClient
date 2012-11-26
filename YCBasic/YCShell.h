#ifndef _INCLUDE_YCSHELL_H_
#define _INCLUDE_YCSHELL_H_

class YCShellImpl;
class YCIShellCommand;
class YCIShellInterceptor;

/*
 * YCShell : Shell交互包装器
 *           不可继承Final类
 *
 * 说明：
 */
class YCAPI YCShell
{
public:

	YCShell(const char* prompt);

	~YCShell(void);

	//
	// 注释：注册命令拦截器，可以用于登录，登出检查等
	//
	YCShell& registInterceptor(YCIShellInterceptor* interceptor);

	//
	// 注释：注册命令处理器
	//
	YCShell& registCommander(YCIShellCommand* commander);

	//
	// 注释：循环运行知道用户关闭CMD窗口
	//
	void run();

	//
	// 注释：退出Shell
	//
	void quit();

	//
	// 注释：处理命令
	//
	void handle(const char* cmd, int len);

	//
	// 注释：获取提示符
	//
	const char* getPrompt() const;

	//
	// 注释：设置提示符
	//
	void setPrompt(const char* prompt);

private:

	YCShellImpl * myImpl;
};

#endif

