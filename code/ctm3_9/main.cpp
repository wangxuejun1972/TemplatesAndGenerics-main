/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// 类模板中可变参的逐步展开
// 
  
namespace _nmsp1
{
    // myclasst<Args...>继承
    template<typename... Args>
    class myclasst
    {
    public:
        myclasst()
        {
            std::cout << "myclasst::myclasst()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    template<typename... Args>
    class myclasst2 : public myclasst<Args...>
    {
    public:
        myclasst2()
        {
            std::cout << "myclasst2::myclasst2()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    void func()
    {
        myclasst2<double, int, float, std::string> mc1;
        // myclasst::myclasst()构造函数执行了。可变参个数 = 4
        // myclasst2::myclasst2()构造函数执行了。可变参个数 = 4
        
        // 通过打印，我们看到执行这行代码会实例化出下面这两个类，先实例化父类，然后再实例化子类
        // myclasst<double, int, float, std::string>类
        // myclasst2<double, int, float, std::string>类
        
    }
}

namespace _nmsp2
{
    // myclasst<Args>...继承
    template<typename... Args>
    class myclasst
    {
    public:
        myclasst()
        {
            std::cout << "myclasst::myclasst()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    template<typename... Args>
    class myclasst2 : public myclasst<Args>...
    {
    public:
        myclasst2()
        {
            std::cout << "myclasst2::myclasst2()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    void func()
    {
        myclasst2<double, int, float, std::string> mc1;
        // myclasst::myclasst()构造函数执行了。可变参个数 = 1
        // myclasst::myclasst()构造函数执行了。可变参个数 = 1
        // myclasst::myclasst()构造函��执行了。可变参个数 = 1
        // myclasst::myclasst()构造函数执行了。可变参个数 = 1
        // myclasst2::myclasst2()构造函数执行了。可变参个数 = 4
        
        // 从打印结果来看，这种实例化代码执行的时候，会实例化出5个类，分别是
        // myclasst<double>
        // myclasst<int>
        // myclasst<float>
        // myclasst<string>
        // myclasst2<double, int, float, string>
        // 
        // 即这种写法实现的继承关系就等价于
        // class myclasst2 : public myclasst<double>,  public myclasst<int>,  public myclasst<float>,  public myclasst<string>
        
    }
}

namespace _nmsp3
{
    // myclasst<Args, char>...继承
    template<typename... Args>
    class myclasst
    {
    public:
        myclasst()
        {
            std::cout << "myclasst::myclasst()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    template<typename... Args>
    class myclasst2 : public myclasst<Args, char>...
    {
    public:
        myclasst2()
        {
            std::cout << "myclasst2::myclasst2()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    void func()
    {
        myclasst2<double, int, float, std::string> mc1;
        // myclasst::myclasst()构造函数执行了。可变参个数 = 2
        // myclasst::myclasst()构造函数执行了。可变参个数 = 2
        // myclasst::myclasst()构造函数执行了。可变参个数 = 2
        // myclasst::myclasst()构造函��执行了。可变参个数 = 2
        // myclasst2::myclasst2()构造函数执行了。可变参个数 = 4
        
        // 从打印结果来看，这种实例化代码执行的时候，会实例化出5个类，分别是
        // myclasst<double, char>
        // myclasst<int, char>
        // myclasst<float, char>
        // myclasst<string, char>
        // myclasst2<double, int, float, string>
        // 
        // 即这种写法实现的继承关系就等价于
        // class myclasst2 : public myclasst<double, char>,  public myclasst<int, char>,  public myclasst<float, char>,  public myclasst<string, char>
        
    }
}

namespace _nmsp4
{
    // myclasst<Args, Args...>...继承
    template<typename... Args>
    class myclasst
    {
    public:
        myclasst()
        {
            std::cout << "myclasst::myclasst()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    template<typename... Args>
    class myclasst2 : public myclasst<Args, Args...>...
    // Args... 代表的是 double,int, float, string 这一个参数包整体
    {
    public:
        myclasst2()
        {
            std::cout << "myclasst2::myclasst2()构造函数执行了。可变参个数 = " << sizeof...(Args) << std::endl;
        }
    };
    
    void func()
    {
        myclasst2<double, int, float, std::string> mc1;
        // myclasst::myclasst()构造函数执行了。可变参个数 = 5
        // myclasst::myclasst()构造函数执行了。可变参个数 = 5
        // myclasst::myclasst()构造函��执行了。可变参个数 = 5
        // myclasst::myclasst()构造函数执行了。可变参个数 = 5
        // myclasst2::myclasst2()构造函数执行了。可变参个数 = 4
        
        // 从打印结果来看，这种实例化代码执行的时候，会实例化出5个类，分别是
        // myclasst<double, double,int, float, string>
        // myclasst<int, double,int, float, string>
        // myclasst<float, double,int, float, string>
        // myclasst<string, double,int, float, string>
        // myclasst2<double, int, float, string>
        // 
        // 即这种写法实现的继承关系就等价于
        // class myclasst2 : public myclasst<double, double,int, float, string>,  
        // public myclasst<int, double,int, float, string>,  
        // public myclasst<float, double,int, float, string>,  
        // public myclasst<string, double,int, float, string>
        
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    _nmsp4::func();

    return 0;
}
