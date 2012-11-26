#ifndef _INCLUDE_YCNETWORKFILELOADLISTENER_H_
#define _INCLUDE_YCNETWORKFILELOADLISTENER_H_

/*
 * YCNetworkFileLoadListener : 网络下载通知类
 */
class YCNetworkFileLoadListener
{
public:

	YCNetworkFileLoadListener() {};

	virtual ~YCNetworkFileLoadListener() {}

	//
	// 函数：notify(const char* filename, bool result)
	//
	// 目的：下载文件结果通知
	//
	// 参数：
	//       filename : 下载的文件名
	//       result   ：下载结果
	//
	virtual void notify(const char* filename, bool result) = 0;
};

#endif