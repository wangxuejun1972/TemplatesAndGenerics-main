/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

namespace _nmsp1
{
    // 元函数
    constexpr int myfunc(int abc)
    {
        return abc*2;
    }
    
    void func()
    {
        // 编译期间调用的元函数
        constexpr int var = 11 * myfunc(12);
        static_assert(var == 264, "std error");
        // 使用静态断言，var等于264,如果不等，那么就输出 “std error”
        // 如果某个表达式不能确定是否在编译期间能够得到值，就能使用static_assert进行断言判断
        
        // 元函数不仅可以在编译期间被调用，还能再运行时被调用
        int var2 = 10;
        int var3 = myfunc(var2);
    }
}

// 编译期间能够被调用的类模板
namespace _nmsp2
{
    template<int x_v, int y_v>
    struct Calc
    {
        enum
        {
            addvalue = x_v + y_v,
            subvalue = x_v - y_v
        };
    };
    
    void func()
    {
        const int var3 = Calc<12, 15>::addvalue;
        static_assert(var3 == 27, "std error");
        std::cout << var3 << std::endl;
        // 27
        std::cout << Calc<12, 124>::subvalue << std::endl;
        // -112
    }
}

namespace _nmsp3
{
    template<int x_v, int y_v>
    struct Calc
    {
        // 静态const成员变量
        // 注意这里只能使用constexpr,而不能使用const，因为这里是作为编译时常量。
        static constexpr int addvalue = x_v + y_v;
        // 但是如果这样修改（将enum替换成static静态成员变量），某些情况下使用了Calc这个类模板后
        // 可能会导致编译器对Calc这个类模板进行实例化，并且为addvalue分配额外的内存空间
        // 但是你使用enum的时候不会为其（addvalue）额外分配内存空间
    };
    
    void testFunc(const int &tmpvalue)
    {
        return;
    }
    
    void func()
    {
        testFunc(Calc<12, 15>::addvalue);
        // 这种调用就会导致编译器实例化Calc类模板和为addvalue分配内存空间
    }
}
 
// 在编译期间计算5的阶乘 1*2*3*4*5
namespace _nmsp4
{
    // 计算n的阶乘
    // 泛化版本
    template<int n_v>
    struct Factorial
    {
        enum
        {
            value = n_v * Factorial<n_v - 1>::value
        };
    };
    
    // 特化版本
    template<>
    struct Factorial<1>
    {
        enum
        {
            value = 1
        };
    };
    
    void func()
    {
        std::cout << Factorial<5>::value << std::endl;
        // 120
        // 加一个静态断言
        static_assert(Factorial<5>::value == 120, "std error");
    }
}


// 使用constexpr在编译期间进行 计算n的阶乘
namespace _nmsp5
{
    constexpr int Factorial(int n_v)
    {
        return n_v <= 1 ? 1 : (n_v * Factorial(n_v - 1));
    }
    
    void func()
    {
        std::cout << Factorial(5) << std::endl;
        // 120
        static_assert(Factorial(5) == 120, "std error");
        
        // 添加上 constexpr 之后变成编译时求值
        constexpr int tmpvalue = Factorial(10);
        std::cout << tmpvalue << std::endl;
        // 3628800
    }
}

// 从执行效率上来看，_nmsp5中的求阶乘方式的执行效率没有_nmsp4中的类模板方式的执行效率高

// 随着c++的不断发展，自c++14之后，编译器对constexpr的功能进行了不断的增强和完善，
// 比如_nmsp4中，就可以不用递归，而使用for语句进行求值
// 同样的，从代码书写上看，比类模板简单，但是从执行效率来说，并没有类模板来的效率高
namespace _nmsp6
{
    constexpr int Factorial(int n_v)
    {
        int result = 1;
        for(int i = 1; i <= n_v; ++i)
        {
            result *= i;
        }
        return result;
    }
    
    
    void func()
    {
        std::cout << Factorial(5) << std::endl;
        static_assert(Factorial(5) == 120, "std error");
    }
}


// constexpr修饰的变量模板
namespace _nmsp7
{
    // 变量模板（因为有cosntexpr修饰，这里result代表的是常量）
    // 看起来也像是函数调用，这也是元函数的一种形式
    // 泛化版本
    template<int Arg>
    constexpr int result = Arg * result<Arg - 1>;
    
    // 特化版本
    template<>
    constexpr int result<0> = 1;
    
    void func()
    {
        std::cout << result<5> << std::endl;
        // 120
        static_assert(result<5> == 120, "std error");
    }
}

// 改造一下，以便能看到更明显的函数调用痕迹
namespace _nmsp8
{
    template<int Arg>
    constexpr int result()
    {
        return Arg * result<Arg - 1>();
    }
    
    // 特化版本
    template<>
    constexpr int result<0>()
    {
        return 1;
    }
    
    void func()
    {
        std::cout << result<5>() << std::endl;
        // 120
        static_assert(result<5>() == 120, "std error");
    }
}

// 求和
namespace _nmsp9
{
    // 泛化版本
    // 这里泛化版本应该是作为递归出口，因为...参数包代表0-n 个参数，只有这里可以允许存在0个参数
    template<int... FTArgs>
    constexpr int resultSum = 0;
    
    // 特化版本
    template<int First, int... FTArgs>
    constexpr int resultSum<First, FTArgs...> = First + resultSum<FTArgs...>;
    
    void func()
    {
        std::cout << resultSum<1, 10, 100, 1000, 10000> << std::endl;
        // 11111
    }
}

// 当然使用折叠表达式，可以更轻松的实现求和
namespace _nmsp10
{
    template<int... Args>
    constexpr int resultSum = (...+Args);
    
    // 当然上面这个也可以写成函数函数模板
    template<int... Args>
    constexpr int resultSum2()
    {
        return (...+Args);
        // 或者不用缩写，写完整就是
        // return (0+...+Args);
    }
    
    
    void func()
    {
        std::cout << resultSum<1, 10, 100, 1000, 10000> << std::endl;
        // 11111
        std::cout << resultSum2<1, 10, 100, 1000, 10000>() << std::endl;
        // 11111
    }
}

// 此前【4-6节】曾经介绍过 std::remove_all_extents 这个类模板，这个就是一个比较典型的类型元函数，
// 他这里用到了类型模板的递归实例化，也就是说，这个类模板实现的元编程就是靠递归模板实例化来驱动的
namespace _nmsp11
{
    // AddPoint 就是一个类型元函数
    // 该元函数简单定义了一个叫做type的类型别名，代表的是类型参数T的指针类型 T*
    template<typename T>
    struct AddPoint
    {
        using type = T*;
    };
    
    // 进一步来说，AddPoint这个类模板的功能就是在编译期间将T转换为对应的指针类型 T*
    // 像AddPoint这种，包含了using来定义类型别名的类模板，就称之为类型元函数（其实也就是之前的固定萃取类模板【fixed trait】）
    
    void func()
    {
        AddPoint<const char>::type s = "AddPoint";
        // 等价于 const char* s = "AddPoint";
        
        std::cout << "s的类型是： " << typeid(decltype(s)).name() << std::endl;
        // s的类型是：char const *
    }
}

namespace _nmsp12
{
    template<typename T>
    using AddPoint = T*;
    // 这种修改之后的写法，任然可以称之为元函数
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    // _nmsp4::func();
    // _nmsp5::func();
    // _nmsp6::func();
    // _nmsp7::func();
    // _nmsp8::func();
    // _nmsp9::func();
    // _nmsp10::func();
    _nmsp11::func();
    

    return 0;
}

