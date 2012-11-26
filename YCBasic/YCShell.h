#ifndef _INCLUDE_YCSHELL_H_
#define _INCLUDE_YCSHELL_H_

class YCShellImpl;
class YCIShellCommand;
class YCIShellInterceptor;

/*
 * YCShell : Shell������װ��
 *           ���ɼ̳�Final��
 *
 * ˵����
 */
class YCAPI YCShell
{
public:

	YCShell(const char* prompt);

	~YCShell(void);

	//
	// ע�ͣ�ע���������������������ڵ�¼���ǳ�����
	//
	YCShell& registInterceptor(YCIShellInterceptor* interceptor);

	//
	// ע�ͣ�ע���������
	//
	YCShell& registCommander(YCIShellCommand* commander);

	//
	// ע�ͣ�ѭ������֪���û��ر�CMD����
	//
	void run();

	//
	// ע�ͣ��˳�Shell
	//
	void quit();

	//
	// ע�ͣ���������
	//
	void handle(const char* cmd, int len);

	//
	// ע�ͣ���ȡ��ʾ��
	//
	const char* getPrompt() const;

	//
	// ע�ͣ�������ʾ��
	//
	void setPrompt(const char* prompt);

private:

	YCShellImpl * myImpl;
};

#endif

