#ifndef _INCLUDE_YCSTATICASSERT_H_
#define _INCLUDE_YCSTATICASSERT_H_

#ifdef __cplusplus

#define JOIN( X, Y ) JOIN2(X,Y)
#define JOIN2( X, Y ) X##Y

namespace STATIC_ASSERT
{
	template <bool> struct STATIC_ASSERT_FAILURE;
	template <> struct STATIC_ASSERT_FAILURE<true> { enum { value = 1 }; };

	template<int x> struct static_assert_test{};
}

#define COMPILE_ASSERT(x) \
	typedef ::STATIC_ASSERT::static_assert_test<\
	sizeof(::STATIC_ASSERT::STATIC_ASSERT_FAILURE< (bool)( x ) >)>\
	JOIN(_static_assert_typedef, __LINE__)

#else // __cplusplus

#define COMPILE_ASSERT(x) extern int __dummy[(int)x]

#endif // __cplusplus

#define VERIFY_EXPLICIT_CAST(from, to) COMPILE_ASSERT(sizeof(from) == sizeof(to)) 

#endif // _INCLUDE_YCSTATICASSERT_H_
