#ifndef _INCLUDE_YCHTTPCLIENT_H_
#define _INCLUDE_YCHTTPCLIENT_H_

class YCIHttpAsyncActionListener;

class YCHttpClient
{

public:
	
	YCHttpClient();
	
	~YCHttpClient(void);

	//
	// ������const char* get(const char *url)
	//
	// Ŀ�ģ�ͬ�������ļ�
	//
	const char* get(const char *url);

private:

};

#endif

