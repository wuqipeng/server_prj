#ifndef LIB_NONCOPYABLE_HPP_INCLUDED
#define LIB_NONCOPYABLE_HPP_INCLUDED 1

//this software was writted by wuqipeng
//it is like boost::noncopyable_::noncopyable class


namespace server { namespace lib {

	class noncopyable{
	
	protected:
		noncopyable(){};        //这两个函数需要提供一个函数定义，因为他们会被调用，在链接时要找到其定义体         
		~noncopyable(){};
	private:
		noncopyable(const noncopyable &);  //这两个函数不需要提供函数定义，因为他们不会被调用，如果被调用，在编译期间就过不了。链接器只会在调用相应的函数时才会去查找相应的函数定义。
		noncopyable& operator=(const noncopyable &);
	};

}
}

#endif
