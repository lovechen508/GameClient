#ifndef _INCLUDE_YCRECT_H_
#define _INCLUDE_YCRECT_H_

template<typename T> 
struct YCAPI TRect
{
	T left, top, right, bottom;
	TRect() {}
	TRect(T const & l, T const & t, T const & r, T const & b)
		: left(l), top(t), right(r), bottom(b)
	{
	}
	TRect(TRect const & o)
		: left(o.left), top(o.top), right(o.right), bottom(o.bottom)
	{
	}
	TRect & operator=(TRect const & o)
	{
		left = o.left;
		top = o.top;
		right = o.right;
		bottom = o.bottom;
		return *this;
	}
	T width() const
	{
		return right - left;
	}
	T height() const
	{
		return bottom - top;
	}
};

typedef TRect<float> FloatRect;
typedef TRect<int> IntRect;

#endif