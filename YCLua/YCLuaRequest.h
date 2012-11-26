#ifndef _INCLUDE_YCLUA_REQUEST_H_
#define _INCLUDE_YCLUA_REQUEST_H_

class YCLuaRequestHolder;

/*
 * YCLuaRequest : ��Lua������C++����������Ϣ
 *                һ�㷢�����û����ĳ����ť��
 *                ͨ��onClick��������ȡ���ݲ�����
 *                YCLuaRequest����Ӧ��YCIController
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

