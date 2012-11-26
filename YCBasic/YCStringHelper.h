#ifndef _INCLUDE_YCSTRINGHELPER_H_
#define _INCLUDE_YCSTRINGHELPER_H_

#include <regex>
#include <string>
#include <vector>

/*
 * YCStringHelper : std::string辅助函数
 */
struct YCStringHelper
{
	enum E_NUMBER_TYPE {
		NUMBER           = 1,
		POSITIVE_NUMBER  = 2,
		INTEGER          = 3,
		POSITIVE_INTEGER = 4
	};

	static std::string trimLeft(const std::string& str) {
		std::string t = str;
		for (std::string::iterator i = t.begin(); i != t.end(); i++) {
			if (!isspace(*i)) {
				t.erase(t.begin(), i);
				break;
			}
		}

		return t;
	}

	static std::string trimRight(const std::string& str) {
		if (str.begin() == str.end()) {
			return str;
		}

		std::string t = str;
		for (std::string::iterator i = t.end() - 1; i != t.begin(); i--) {
			if (!isspace(*i)) {
				t.erase(i + 1, t.end());
				break;
			}
		}
		return t;
	}

	static inline bool istab(int c) 
	{ 
		return (c == '\t'); 
	}

	static inline char* strltrim(char* str)
	{
		while (str != '\0')
		{
			if (!isspace(*str) && !istab(*str))
			{
				break;
			}
			++str;
		}
		return str;
	}

	static inline char* strrtrim(char* str)
	{
		for (int len = strlen(str) - 1; len >= 0; --len)
		{
			if (!isspace(str[len]) && !istab(str[len]))
			{
				break;
			}
			str[len] = '\0';
		}
		return str;
	} 

	static const char* trim(char* str) {
		return strrtrim(strltrim(str));
	}
		
	static void split(const char* buf, const char* delim, std::vector<std::string>* ret)
	{
		size_t last = 0;
		std::string s(buf);
		size_t index = s.find_first_of(delim, last);
		while (index != std::string::npos)
		{
			ret->push_back(s.substr(last, index-last));
			last = index + 1;
			index = s.find_first_of(delim, last);
		}
	
		if (index-last > 0)
		{
			ret->push_back(s.substr(last, index - last));
		}
	} 

	static unsigned long hash(char* buf, int len)
	{
		unsigned long hash = 5381;

		for (int i = 0; i < len; i++)
		{
            hash = ((hash<<5) + hash) + buf[i]; /* hash * 33 + c */
		}
        return hash;
	}

	////////////////////////////////////////////////
	// Number Check
	////////////////////////////////////////////////

	//
	// 函数： isType(const char *tocheck)
	//
	// 目的：
	//
	//       -?[0-9]+([.][0-9]+)?
	//       [0-9]+([.][0-9]+)?
	//       -?[0-9]+
	//       [0-9]+              
	//
	static bool isType(const char *tocheck, E_NUMBER_TYPE type)
	{
		if      (type == NUMBER)           return std::regex_match(tocheck, std::regex("-?[0-9]+([.][0-9]+)?", std::regex::icase));
		else if (type == POSITIVE_NUMBER)  return std::regex_match(tocheck, std::regex("[0-9]+([.][0-9]+)?",   std::regex::icase));
		else if (type == INTEGER)          return std::regex_match(tocheck, std::regex("-?[0-9]+",             std::regex::icase));
		else if (type == POSITIVE_INTEGER) return std::regex_match(tocheck, std::regex("[0-9]+",               std::regex::icase));
		return false;
	}

	//
	// 函数：parseColor(const char * tocheck)
	//
	// 目的：解析color配置 0xAACCBB
	//
	static bool parseColor(const char * tocheck, unsigned int& color)
	{
		color = 0;

		if (strlen(tocheck) != 8)
		{
			return false;
		}

		if (tocheck[0] != '0' && (tocheck[1] != 'x' || tocheck[1] != 'X'))
		{
			return false;
		}

		for (int i = 7; i > 1; --i)
		{
			int value = 0;
			switch (tocheck[i])
			{
			case '0': value =  0; break;
			case '1': value =  1; break;
			case '2': value =  2; break;
			case '3': value =  3; break;
			case '4': value =  4; break;
			case '5': value =  5; break;
			case '6': value =  6; break;
			case '7': value =  7; break;
			case '8': value =  8; break;
			case '9': value =  9; break;
			case 'a': value = 10; break;
			case 'A': value = 10; break;
			case 'b': value = 11; break;
			case 'B': value = 11; break;
			case 'c': value = 12; break;
			case 'C': value = 12; break;
			case 'd': value = 13; break;
			case 'D': value = 13; break;
			case 'e': value = 14; break;
			case 'E': value = 14; break;
			case 'f': value = 15; break;
			case 'F': value = 15; break;
			default: return false;
			}

			color += (value << ((7-i)*4));
		}

		return true;
	}
};
#endif