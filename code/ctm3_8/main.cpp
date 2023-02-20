/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// enable_if：
// 是c++11新标准中引入的类模板（结构模板）
// 他的使用体现了c++编译器的SFINAE（替换失败不是一个错误）特新。定位为一个helper模板（助手模板）
// 用于辅助其他模板的设计，表现一种编译器的分支逻辑（编译器就可以确定走哪条分支）

namespace _nmsp1
{
    // 这个类模板只使用了一个using关键字给这个类型T取了一个别名type
    template<typename T>
    struct NED
    {
        using type = T;
    };
    
    // // enable_if 标准库里的模板源码
    // // STRUCT TEMPLATE enable_if
    // template<bool _Test, class _Ty = void>  // 泛化版本
    // struct enable_if{};     // no member 'type' when '_Test'
    
    // template<class _Ty> // 特化版本
    // struct enable_if<true, _Ty> // type is _Ty for _Test
    // {
    //     using type = _Ty;
    // }
    // // 介绍一下
    
    void func()
    {
        NED<int>::type abc = 15;
        //  NED<int>::type 代表的就是 int 类型
        // 所以这行代码就相当于定义了一个int 类型的变量 abc
        
        std::enable_if<(3>2)>::type *mypointer = nullptr;
        // 这行代码能通过编译，没有错误
        // 首先：因为enable_if的条件判断语句（3>2）这个是在编译期间就能确定为true，所以走到他的偏特化版本
        // 偏特化版本里有个type类型别名，给这个类型别名取一个mypointer的变量名，值为一个空指针
        
        // 而且我们还看到，enable_if我这里值提供了一个参数，第二个参数我这里为空，说明enable_if是允许值提供一个参数的
        // 主要是因为他的泛化版本中有一个默认值 void
        // 所以这行代码在此情此景下就等于 void *mypointer = nullptr;
        
        // std::enable_if<(3<2)>::type *mypointer2 = nullptr;
        // 如果这样写，那就会编译器报错，找不到type这个成员属性
        // 也很简单，因为单（3<2）false的时候，走的是泛化版本，泛化版本中根本就没有定义type这么个类型别名
        
        
    }
}

namespace _nmsp2
{
    // enable_if的一个典型应用就是作为其他函数模板的返回类型
    
    // enable_if用在函数模板中
    template<typename T>
    typename std::enable_if<(sizeof(T) > 2)>:: type funceb()
    {
        // ...
    }
    // 分析一下这个代码：函数模板名 funceb, 这个函数模板带了一个名字叫T的类型模板参数，
    // 然后这个函数模板的返回类型很长，【typename std::enable_if<(sizeof(T) > 2)>::type】
    // 返回类型里的typename作用就是告诉编译器返回类型是一个 模板 类型。
    
    void func()
    {
        funceb<int>();
        // 编译没问题，因为传递进去的int类型的sizeof值（整形类型长度 4 ）大于2，
        // 所以走到enable_if的偏特化分支里，有type成员
        // 按照这种写法，那么这个函数模板调用之后实例化出来的函数就等价于
        // void funceb()
        
        // funceb<char>();
        // 编译报错，没有找到funceb匹配的重载函数
    }
}

namespace _nmsp3
{
    // c++14中又对enable_if做了优化，只要在enable_if后面加一个_t，就可以省略掉前面的typename和后面的：：type
    
    // enable_if用在函数模板中
    template<typename T>
    // typename std::enable_if<(sizeof(T) > 2)>:: type funceb()
    // std::enable_if_t<(sizeof(T) > 2)> funceb()
    std::enable_if_t<(sizeof(T) > 2), T> funceb()
    {
        // ...
        T myt = {};
        return myt;
    }
    
    // enable_if_t的实现其实很简单，就是类似定义了一个别名模板一样
    // template<bool _Test, class _Ty = void>
    // using enable_if_t = typename enable_if<_Test, _Ty>::type; // 别名模板
    
    
    void func()
    {
        funceb<int>(); 
        // void funceb()
        // 给enable_if添加第二个参数后， int funceb();
        
        // 针对于这种调用，如果funceb函数模板中所涉及到的enable_if中的这个条件成立的话，
        // 那么整个enable_if代表的是一个类型，如果不成立，因为SFINAE特性的存在，所以就会请
        // 编译器忽略funceb所代表的这个模板，即这个模板就不存在
    }
}

namespace _nmsp4
{
    
    // 给类型模板定义别名
    template<typename T>
    using StrProcType = std::enable_if_t<std::is_convertible<T, std::string>::value>;
    
    
    class Humen
    {
    public:
    // 构造函数模板
        // template<typename T, typename = std::enable_if_t<std::is_convertible<T, std::string>::value>>
        // 如果类型T能够成功转换成string类型的话，那么整个后面这部分就等于 void，即 typename void
        // 即，如果传递进来的实参不能装换成string，那么这一整个构造函数模板就会被忽略
        // （也就是摆明了，这个构造函数模板只适合哪些能够成功隐式转换成string类型的实参进行调用）
        // 当然这里其实更完整的写法是typename后面应该跟一个参数类型U，只是这里U没有用到，所以可以忽略不写
        // 也就是这种情况下typename后面直接跟 = 语法上是没问题的
        // template<typename T, typename U = std::enable_if_t<std::is_convertible<T, std::string>::value>>
        // 当然，如果你嫌弃这种写法太长了，那么你还可以给这个写法定义一个类型别名，这里直接使用类型别名也是可以的
        template<typename T, typename U = StrProcType<T>>
        Humen(T&& tmpname):m_sname(std::forward<T>(tmpname))
        {
            std::cout << "Humen(T&& tmpname):m_sname(std::forward<T>(tmpname))构造函数执行了" << std::endl;
        }
        
        // 拷贝构造函数
        Humen(const Humen& th):m_sname(th.m_sname)
        {
            std::cout << "Humen(const Humen& th):m_sname(th.m_sname)拷贝构造函数执行了" << std::endl;
        }
        
        // 移动构造函数
        Humen(Humen&& thr):m_sname(std::move(thr.m_sname))
        {
            std::cout << "Humen(Humen&& thr):m_sname(std::move(thr.m_sname))移动构造函数执行了" << std::endl;
        }
        
    private:
        string m_sname;
        
    };
    
    void func()
    {
        string sname = "zhansan";
        Humen myhume1(sname);
        // Humen(T&& tmpname):m_sname(std::forward<T>(tmpname))构造函数执行了
        Humen myhume2(myhume1);     
        // 这里我们的原意是想调用拷贝构造函数（但是实际上没有，因为myhume1实参不是const常量修饰的）
        // 导致他去调用构造函数模板去了出现了混乱
        // 解决办法：
        // 针对构造函数模板，如果给进来的参数是一个string类型参数，我们才希望执行构造函数模板，让这个构造函数模板生效
        // 如果不是string，那么我们就让这个构造函数模板失效（忽略）
        // 也就是说如果使用enable_if在构造函数模板中，只需要设置enable_if的条件为形参类型 == string 即可
        // 那么这里就需要额外介绍另一个标准库中的类模板
        // std::is_convertible c++11中引入的，他有两个模板参数分别是 From和To
        // 他的能力就是判断是否能够从某个类型隐式转换成另一个类型，能否转换返回的就是一个bool值
        // 这样我们改造好构造函数模板之后，函数模板对于这行实例化代码来说就不存在（因为不适合），所以他就自然而然的只能去调用我们提供的拷贝构造函数去了
        
        // Humen(const Humen& th):m_sname(th.m_sname)拷贝构造函数执行了
        
        const Humen myhume3("lisi");
        // Humen(T&& tmpname):m_sname(std::forward<T>(tmpname))��造函数执行了
        Humen myhume4(myhume3);
        // Humen(const Humen& th):m_sname(th.m_sname)拷贝构造函数执行了
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    _nmsp4::func();
    
    std::cout << "string ==> float ?  " << std::is_convertible<std::string, float>::value << std::endl;
    std::cout << "float ==> int ?  " << std::is_convertible<float, int>::value << std::endl;

    return 0;
}
