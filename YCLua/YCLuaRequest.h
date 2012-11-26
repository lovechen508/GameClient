#ifndef _INCLUDE_YCLUA_REQUEST_H_
#define _INCLUDE_YCLUA_REQUEST_H_

class YCLuaRequestHolder;

/*
 * YCLuaRequest : 从Lua环境向C++传递请求消息
 *                一般发生在用户点击某个按钮后
 *                通过onClick监听器获取数据并构造
 *                YCLuaRequest到对应的YCIController
 */
class YCAPI YCLuaRequest
{
public:

	YCLuaRequest(void);

	~YCLuaRequest(void);

	bool hasParameter(const char* key);

	int intValue(const char* key);

	const char* strValue(const char* key);

	float floatValue(const char* key);

private:

	YCLuaRequestHolder *myHolder;
};

#endif

