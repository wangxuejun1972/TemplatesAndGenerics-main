/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// 萃取技术以及fixed traits (固定萃取)
// 萃取（trait）技术用于对模板中的各类模板参数进行管理
// 这种萃取技术往往是通过书写一些类模板进行体现，从表现上来讲，这些trait类模板像是一些中间件
// 它们夹杂（运行）在不同的功能代码之间，使得这些代码之间的调用，结合更加灵活
// trait可以做很多事情，在泛型编程中也有广泛的应用

// type traits 类型萃取，类型萃取函数库，实现了很多类型信息的提取，类型变换功能
// 萃取技术是使用模板与泛型的方式来编写代码，与传统的面向对象编程有一定的区别

// fixed traits(固定萃取)主要是用于给进来一种类型，萃取（得到）另外一种类型

namespace _nmsp1
{
    template<typename T>
    T funcsun(const T* begin, const T* end)
    {
        T sum{};    //  零初始化（T如果是数值型类型，那就被初始化为 0 ， 如果是指针类型就被初始化为nullptr，如果是bool类型，就被初始化成false 0）
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << (int)funcsun(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << (int)funcsun(&navl[0], &navl[2]) << std::endl;
        // -2044967296
        std::cout << (int)funcsun(&navc[0], &navc[2]) << std::endl;
        // 38
        
        // 上面这3行调用编译器会实例化出如下3个函数
        // int funcsun<int>(const int*, const int*)
        // int funcsun<int>(const int*, const int*)
        // char funcsun<char>(const char*, const char*)
        
        
    }
}

// 然后，上面这些执行结果后两行都因为类型问题，导致最后结果溢出，而出现错误
// 那么如何解决这种结果值溢出的问题？
// 这里我们可以引入一个新的类型参数U，来传入合适的结果类型作为返回值类型
namespace _nmsp2
{
    template<typename T, typename U>
    U funcsun(const T* begin, const T* end)
    {
        U sum{};    //  零初始化（T如果是数值型类型，那就被初始化为 0 ， 如果是指针类型就被初始化为nullptr，如果是bool类型，就被初始化成false 0）
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << funcsun<int, int>(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << funcsun<int, int64_t>(&navl[0], &navl[2]) << std::endl;
        // 2250000000
        std::cout << funcsun<char, int>(&navc[0], &navc[2]) << std::endl;
        // 294
        
    }
}

// 通过传递两个类型模板参数，把计算溢出问题解决了，但是这种解决方法并不太方便
// 因为每次调用这个函数模板的时候，都需要额外指定返回值类型参数，显得麻烦
// 所以。基于这种考量，这里打算使用fixed traits模板来进行改进

namespace _nmsp3
{
    // fixed traits 泛化版本
    template<typename T>
    struct SunmFiexTraits;  // 不需要实现代码，因为不需要该版本进行实例化
    
    // 各个版本的固定萃取（fixed traits）
    // 1）给进来char 类型，返回int类型
    template<>
    struct SunmFiexTraits<char>
    {
        using sumT = int;   // 类型别名sumT为int类型，也就是返回类型为int类型
    };
    
    // 2）给进来int类型，返回的是__int64/long long/int64_t
    template<>
    struct SunmFiexTraits<int>
    {
        using sumT = int64_t;   // 类型别名sumT为int64_t类型，也就是返回类型为int64_t类型
    };
    
    // 3）其他给进来的是某个类型，返回的是另外一个类型。。。。。
    // 可以扩展出任意多个特化版本
    
    template<typename T>
    auto funcsun(const T* begin, const T* end)
    {
        using sumT = typename SunmFiexTraits<T>::sumT;
        // 给进来一个类型T，返回一个类型（这就是固定萃取的应用）
        sumT sum{};
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << funcsun(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << funcsun(&navl[0], &navl[2]) << std::endl;
        // 2250000000
        std::cout << funcsun(&navc[0], &navc[2]) << std::endl;
        // 294
    }
}

// 固定萃取类模板的作用就是通过某个类型，得到另外一个类型

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}

