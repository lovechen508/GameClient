#ifndef _INCLUDE_YCSHAREDPTR_H_
#define _INCLUDE_YCSHAREDPTR_H_

#include<memory>

class sharedptr_count_base
{
public:
	explicit sharedptr_count_base():usecount(1)
	{
	}

	void addref()
	{
		++usecount;
	}

	void release()
	{
		if(--usecount==0){
			dispose();
			delete this;
		}
	}

	long use_count()
	{
		return usecount;
	}

	virtual void dispose()=0;

	virtual ~sharedptr_count_base(){};

private:

	int usecount; 
};

template<typename T>
class sharedptr_count_impl 
	: public sharedptr_count_base
{
public:
	explicit sharedptr_count_impl(T* p):px(p)
	{
	}

	virtual void dispose()
	{
		if(px)
			delete px;
	}

private:
	T* px;
};

template<typename T, typename D>
class sharedptr_count_impld
	: public sharedptr_count_base
{
public:
	explicit sharedptr_count_impld(T* p, D d):px(p),del(d)
	{
	}

	virtual void dispose()
	{
		del(px);
	}

private:
	T* px;
	D  del;
};


class sharedptr_count
{
private:

	sharedptr_count_base * pi_;

public:

	explicit sharedptr_count():pi_(0)
	{
	}

	template<typename Y>
	explicit sharedptr_count(Y *p)
	{
		pi_ = new sharedptr_count_impl<Y>(p);
	}

	template<typename Y, typename D>
	explicit sharedptr_count(Y *p, D d)
	{
		pi_ = new sharedptr_count_impld<Y, D>(p, d);
	} 

	~sharedptr_count()
	{
		if( pi_ != 0 ) pi_->release();
	}

	sharedptr_count& operator=(sharedptr_count const &other)
	{
		if( pi_!=other.pi_){
			if( pi_)
				pi_->release();
			pi_ = other.pi_;
			if( pi_)
				pi_->addref();
		}
		return *this;
	}

	explicit sharedptr_count(sharedptr_count const &other):pi_(other.pi_)
	{
		if (pi_)
			pi_->addref();
	}

	long use_count() const
	{
		return pi_ != 0? pi_->use_count(): 0;
	}

	void swap(sharedptr_count & other)
	{
		std::swap(pi_,other.pi_);
	}

};

template<typename T>
class YCSharedPtr
{
private:

	typedef YCSharedPtr<T> this_type;

public:

	typedef T element_type;
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;

	explicit YCSharedPtr(): px(0), pn()
	{
	}

	template<typename Y>
	explicit YCSharedPtr( Y * p ): px( p ), pn( p )
	{
	}

	template<typename Y, typename D>
	explicit YCSharedPtr( Y * p, D d): px( p ), pn( p, d )
	{
	}

	explicit YCSharedPtr( YCSharedPtr &other )
	{
		px = other.px;
		pn = other.pn;
	}

	T* get() const
	{
		return px;
	}

	reference operator*() const
	{
		return *px;
	}

	pointer operator->() const
	{
		return px;
	}

	template<class Y>
	YCSharedPtr(YCSharedPtr<Y> const & r): px(r.px), pn(r.pn)
	{
	}

	YCSharedPtr & operator=(YCSharedPtr const &other)
	{
		px =other.px;
		pn =other.pn;
		return *this;
	}

	long use_count() const
	{
		return pn.use_count();
	}

	void swap(YCSharedPtr<T> &other)
	{
		std::swap(px, other.px);
		pn.swap(other.pn);
	}

	void reset()
	{
		this_type().swap(*this);
	}

	template<class Y> void reset(Y * p)
	{
		((p == 0 || p != px) ? (void)0 : _assert("p == 0 || p != px", "YCSharedPtr.hpp", 236));
		this_type(p).swap(*this);
	}

private:
	T * px;
	sharedptr_count pn;
};

template<class T, class U> 
inline bool operator==(YCSharedPtr<T> const & a, YCSharedPtr<U> const & b)
{
	return a.get() == b.get();
}

template<class T, class U> 
inline bool operator!=(YCSharedPtr<T> const & a, YCSharedPtr<U> const & b)
{
	return a.get() != b.get();
}

template<class T, class U> 
inline bool operator<(YCSharedPtr<T> const & a, YCSharedPtr<U> const & b)
{
	return a.get() < b.get();
}

#endif
