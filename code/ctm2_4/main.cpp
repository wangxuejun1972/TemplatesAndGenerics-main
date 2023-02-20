/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

namespace _nmsp1
{
    // 一：类模板的基本范例和模板参数的推断
    // 类模板：是产生类的模具，通过给定的模板参数，生成具体的类，也就是实例化一个特定的类
    // vector<int>等，引入类模板的目的，减少代码冗余
    // 1.1）基本范例
    // 1.2）模板参数的推断
    // 已往来讲，只有函数模板的参数类型可以进行推断
    // 但是在c++17中，类模板的类型模板参数也可以进行推断了
    
    // 类模板定义
    template<typename T>    // T是类型模板参数，标识myVector这个容器类所装的元素类型
    class MyVector          // MyVector可以称呼为类名或者类模板， MyVector<T>就表示一个被实例化的状态，可以称为类型名，因为一个类模板后面带尖括号<>，就表示一个具体的实例化了的类型了
    {
    public:
        typedef T* myItertor;   // 迭代器
        
    public:
        MyVector();     // 构造函数
        MyVector<T> &operator=(const MyVector&);    // 赋值运算符重载
        // MyVector &operator=(const MyVector&);    // 赋值运算符重载[因为是在类里面，所以类模板后面的<T>可以省略]，类型名可以简化写成类名
        
        MyVector(T tmpt)    // 带一个参数的构造函数
        {
            
        }
        
    public:
        void myFunc()
        {
            std::cout << "myFunc()" << std::endl;
        }
    public:
        // 迭代器接口
        myItertor begain(); // 迭代器起始位置
        myItertor end();    // 迭代器结束位置
        
        static void myStaticFunc()
        {
            std::cout << "myStaticFunc()" << std::endl;
        }
    };
    
    // 类模板实现
    template<typename T>
    MyVector<T>::MyVector() // 类外构造函数实现
    {
        
    }
    
    
    void func()
    {
        MyVector<int> mvec; // 类型模板参数T就被替换成了int
        mvec.myFunc();  // 类模板中，只有被调用的函数，编译器才会实例化出这些函数代码
        
        MyVector<std::string>::myStaticFunc();  // 同样的，类模板中，只有被调用的静态函数，编译器才会实例化这些函数代码
    
        // --------------------------
        MyVector mvc2(24);  // 注意这里，因为有了带参数的构造函数，类模板可以根据实参推断出来类模板的模板参数类型，
        // 所以这里和上面不同的地方就在于，这里就可以省略<int>这个模板参数类型申明
        mvc2.myFunc();
        
    }
}

namespace _nmsp2
{
    // 1.3）推断指南（deduction guide）概要了解
    // c++17提出来的新概念，主要用来在推断类模板参数的时候提供推断指引
    // 1.3.1）隐式的推断指南
    // 针对类模板A的每个构造函数，都有一个隐式的模板参数推断机制存在，这个机制就被称为隐式的推断指南
    // template<typename T>
    // A(T, T)->A<T>
    // 表示：当出现->左侧内容或者形式时，请推断成->右侧的类型，右侧的类型也被称为“指南类型”
    // ->左侧部分：该推断指南所对应的构造函数的函数声明，多个参数之间用,分隔
    // ->右侧部分：类模板名，接着一个尖括号，尖括号中是模板参数名
    // 整个推断指南的含义：当调用带两个参数的构造函数通过类模板A创建相关对象时，
    // 请调用所提供的构造函数的实参来推断类模板A的模板参数类型
    // 一句话：推断指南的存在意义就是让编译器能够通过实参把模板参数的类型推断出来
    // 隐式的推断指南是依托于构造函数而存在的
    
    // 这种隐式推断指南是隐式默认存在的，
    // 如果存在下面这种推断需求
    // template<typename T>
    // A(T, T)->A<double>
    // 那么 A obj(12, 34); 代码行就相当于A<double> obj(12, 34);
    
    template<typename T>
    class A
    {
    public:
        A(T v1, T v2)
        {
            std::cout << "A(T v1, T v2)" << std::endl;
        }
        A(T v1)
        {
            std::cout << "A(T v1)" << std::endl;
        }
    };
    template<typename T>
    A(T)->A<T>;
    
    
    void func()
    {
        // A<int> obja（14, 35);
        A obja(14, 35);    // 这里没有提供类型模板参数，依然能够顺利编译通过，隐式推断 A<int>
        A obja2(34.3f); //这个也是同理，编译器能够通过实参推断出类型模板参数T的类型 A<float>
    
        // A* obj3 = NULL; // 模板推断指南并不能通过NULL来进行类型推断。所以不行
        A<int>* obj3 = NULL;
        
    }
}

namespace _nmsp3
{
    // 1.3.2）自定义的推断指南
    // B bobj4{13}; 不再报错的原因（一下3条，缺一不可）
    // 1）类B是聚合类，是可以通过{}来进行类初始化的，
    // 2）B bobj4{13};这个写法就恰好相当于调用了类模板B的带一个参数的构造函数
    // 尽管类B中实际并不存在构造函数
    // 3）因为template<typename T>B(T)->B<T>;这个推断指南的存在，当调用了类模板带一个参数的构造函数时，
    // 编译器推断出来的类型就是B<T>,最终推断出来的类型就是B<int>
    
    // 
    
    template<typename T>
    class B
    {
    public:
        // 因为我们知道，隐式的推断指南是依托于构造函数而存在的
        T m_b;
        T m_b2;
    };
    // 增加一个自定义的推断指南(支持一个形参的推断)
    template<typename T>
    B(T)->B<T>;
    // 增加一个自定义的推断指南(支持两个形参的推断)
    template<typename T>
    B(T, T)->B<T>;
    
    
    void func()
    {
        B<int> bobj1;   // 明确指定模板参数类型
        B<int> bobj2{13};    // 可以使用初始化列表的方式来定义对象，成员变量m_b = 13
        // 上面这两种都必须明确指定模板参数类型(在没有自定义的推断指南的情况下)
        
        // B bobj3;     // 这个不带参数的不行，因为即使是自定义的推断指南也需要一个实参进行推断
        B bobj4{13};
        
        B bobj5{1.3f, 53.34f};
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();
    
    

    return 0;
}
