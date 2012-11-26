#ifndef _INCLUDE_YCANY_H_
#define _INCLUDE_YCANY_H_

#include <iostream>  
#include <list> 

#include "YCDef.h"
#include "YCAssert.h"

//自定义的any类  
class YCAny  
{  
public:  
      
    //保存真正数据的接口类  
    class placeholder  
    {  
    public:       
        virtual ~placeholder()  
        {  
        }  
    public:   
 
        virtual const std::type_info & type() const = 0;  
        virtual placeholder * clone() const = 0;      
    };  
 
    //真正保存和获取数据的类。  
    template<typename ValueType>  
    class holder : public placeholder  
    {  
    public:           
        holder(const ValueType & value): held(value)  
        {  
        }  
 
    public:   
 
        virtual const std::type_info & type() const 
        {  
            return typeid(ValueType);  
        }  
 
        virtual placeholder * clone() const 
        {  
            return new holder(held);//使用了原型模式  
        }  
 
    public:   
 
        //真正的数据，就保存在这里  
        ValueType held;  
    };  
 
public:  
 
    YCAny(): content(NULL)     
    {         
    }  
 
    //模板构造函数，参数可以是任意类型，真正的数据保存在content中  
    template<typename ValueType>  
    YCAny(const ValueType & value): content(new holder<ValueType>(value))  
    {  
    }    
 
    //拷贝构造函数  
    YCAny(const YCAny& other)  
        : content(other.content ? other.content->clone() : 0)  
    {  
    }  

	//赋值操作符重载
	YCAny& operator=(const YCAny& other)
	{
		content = other.content 
			    ? other.content->clone() 
				: 0;
		return *this;
	}
 
    //析构函数，删除保存数据的content对象  
    ~YCAny()  
    {  
       SAFE_DELETE(content);  
    }  
 
    //查询真实数据的类型。  
    const std::type_info & type() const 
    {  
        return content ? content->type() : typeid(void);  
    }  

private:  
    //一个placeholde对象指针，指向其子类folder的一个实现  
    // 即content( new holder<ValueType>(value) )语句  
    placeholder* content;  
 
    template<typename ValueType> friend ValueType any_cast(const YCAny& operand);  
};

//获取content->helder数据的方法。用来获取真正的数据  
template<typename ValueType>  
ValueType any_cast(const YCAny& operand)  
{  
	SMART_ASSERT( operand.type() == typeid(ValueType) );  
	return static_cast<YCAny::holder<ValueType> *>(operand.content)->held;  
}

#endif