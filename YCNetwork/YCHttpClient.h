#ifndef _INCLUDE_YCHTTPCLIENT_H_
#define _INCLUDE_YCHTTPCLIENT_H_

class YCIHttpAsyncActionListener;

class YCHttpClient
{

public:
	
	YCHttpClient();
	
	~YCHttpClient(void);

	//
	// 函数：const char* get(const char *url)
	//
	// 目的：同步下载文件
	//
	const char* get(const char *url);

private:

};

#endif

