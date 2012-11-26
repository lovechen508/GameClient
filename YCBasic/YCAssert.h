#ifndef _INCLUDE_YCASSERT_H_
#define _INCLUDE_YCASSERT_H_

#include "YCDef.h"

enum E_ASSERTLEVEL{
	LVL_WARN,		// just logging
	LVL_ERROR,		// throw exception
	LVL_FATAL,		// full logging and aborting
	LVL_TOTAL  
};

class YCAssertContext;

/*
 * YCAssert : 增强的Assert类
 *
 * 注释：
 *      Helper for unrolling complex SMART_ASSERT macroses
 */
class YCAPI YCAssert
{
public:
        YCAssert(const char* expr);
        ~YCAssert();
        
        YCAssert& setContext(const char* file, const unsigned int line);
        YCAssert& setExpr(const char*);  

		YCAssert& addValue(const char* name, int value);
		YCAssert& addValue(const char* name, float value);
		YCAssert& addValue(const char* name, double value);
        YCAssert& addValue(const char* name, const char* value);
        
        YCAssert& warn(const char*);
        YCAssert& error(const char*);
        YCAssert& fatal(const char*);

        YCAssert& SMART_ASSERT_A;
        YCAssert& SMART_ASSERT_B;  

        // NULL as callback will disable handling specified level of assertions  
        typedef void (*HandlerCallback_t)(const YCAssertContext&); 
        static void setHandler(E_ASSERTLEVEL, HandlerCallback_t);
                
private:
        static void defaultWarnHandler(const YCAssertContext&);
        static void defaultErrorHandler(const YCAssertContext&);
        static void defaultFatalHandler(const YCAssertContext&); 

        YCAssertContext * m_context;
        static HandlerCallback_t m_callbacks[LVL_TOTAL];

        // forbidden
        YCAssert(const YCAssert&);
        YCAssert& operator=(const YCAssert&);
        YCAssert* operator*(const YCAssert&);

}; // class Assert_c

////////////////////////////////////////////////////////////////////////////////
// These macroses will help us to unfold complex assertions, like
// SMART_ASSERT(a >= 0)(a).warn("A must be >=0");
#define SMART_ASSERT_A(x) SMART_ASSERT_OP(x, B)
#define SMART_ASSERT_B(x) SMART_ASSERT_OP(x, A)

#define SMART_ASSERT_OP(x, next) \
    SMART_ASSERT_A.addValue(#x, (x)).SMART_ASSERT_ ## next

////////////////////////////////////////////////////////////////////////////////   
#ifndef NDEBUG
#define SMART_ASSERT(expr) \
        if (expr) \
                { ; } \
        else \
                YCAssert(#expr).setContext(__FILE__, __LINE__).SMART_ASSERT_A

#define SMART_VERIFY(expr) \
        if (expr) \
                { ; } \
        else \
        YCAssert(#expr).setContext(__FILE__, __LINE__).SMART_ASSERT_A

#define SMART_THROW() \
        YCAssert("").setContext(__FILE__, __LINE__).SMART_ASSERT_A
                        
#else
#define SMART_ASSERT(expr) \
        if (true) \
            {;} \
        else \
            YCAssert("").SMART_ASSERT_A

#define SMART_VERIFY(expr) \
        if (true) \
                { (void)(expr); } \
        else \
        YCAssert("").SMART_ASSERT_A
                
#define SMART_THROW() \
        YCAssert("").setContext(__FILE__, __LINE__).SMART_ASSERT_A

#endif // ifndef NDEBUG

#endif

