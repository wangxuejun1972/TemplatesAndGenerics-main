/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// std::conditional是c++11引入的类模板
// 它用于表现一种编译期间的分支逻辑

// 实现代码(标准库中的实现源码)

// template<bool _Test, class _Ty1, class _Ty2>
// struct conditional
// {
//     using type = _Ty1;
// };

// template<class _Ty1, class _Ty2>
// struct conditional<false, _Ty1, _Ty2>
// {
//     using type = _Ty2;
// };

// 可以看到，源码中，上半部分是泛化版本，下半部分是特化版本
// 泛化版本中，第一个模板参数是一个非类型的（bool）类型的参数
// 如果非类型模板参数为true的话，那么type这个类型别名所代表的类型就是conditional这个类模板的第二个模板参数_Ty1
// 否者就是conditional第三个模板参数_Ty2
// 所以这个类模板看起来表达的就是 if else 这种逻辑

// std::conditional<true, int, float>::type tp1;   // tp1 就是int 类型 ----》 int tp1;
// std::conditional<false, int, float>::type tp2;  // tp2 就是float类型 ----》float tp2;

// std::conditional这种代码虽然使用不复杂，实现的基本上都是真或者假两种分支，但是如果要正常多分支，那么可以看到，写出来的代码非常难看

namespace _nmsp1
{
    
    void func()
    {
        // 先看下面这种if语句
        int i = 35;
        if (i > 100)
        {
            std::cout << "i>100" << std::endl;
        }
        else
        {
            if (i > 80)
            {
                std::cout << "100 > i > 80" << std::endl;
            }
            else
            {
                if (i > 30)
                {
                    std::cout << "80 > i > 30" << std::endl;
                }
                else
                {
                    std::cout << "i < 30" << std::endl;
                }
            }
        }
    }
}


// 假设有如下需求
// 定义一个名字叫做tsvar的变量，然后根据某个常量（取名为J）值的不同，tsvar变量的类型也不相同
// 如果 J 值大于100，tsvar = double;
// 100 > j >80 ,tsvar = float;
// 80 > j > 40, tsvar =int;
// j < 40, tsvar = char,

namespace _nmsp2
{
    // 
    
    void func()
    {
        constexpr int j = 53;
        std::conditional<
            (j > 100), double,  // 值大于100，tsvar 是double类型
            std::conditional<
                (j > 80), float,    // 值小于100大于80，tsvar 是float类型
                std::conditional<
                    (j > 40), int, char // 值小于80大于40，tsvar 是int类型,小于40是char类型
                >::type
            >::type
        >::type tsvar;
        
        
        std::cout << "tsvar 的类型 = " << typeid(decltype(tsvar)).name() << std::endl;
        // int
    }
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}
