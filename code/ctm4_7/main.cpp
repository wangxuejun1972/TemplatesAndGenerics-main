/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// std::remove_all_extents是c++11中引入的类模板
// 他的能力是把一个数组中的数组类型部分移除掉，只保留元素类型
// int a[12];  具体描述一下a的类型应该是 int[12]类型
// std::remove_all_extents的能力就是把这个[]给剔除掉，保留int，那这里int就正好是数组a中保存的元素类型

namespace _nmsp1
{
    
    void func()
    {
        int a[12];    // a的类型就是 float [12]
        int b[5][5];    // b的类型就是int[5][8]
        int c[2][2][2];     // c的类型就是int[2][2][2]
        
        std::cout << typeid(std::remove_all_extents<decltype(a)>::type).name() << std::endl;
        // int
        std::cout << typeid(std::remove_all_extents<decltype(b)>::type).name() << std::endl;
        // int
        std::cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << std::endl;
        // int
    }
}

namespace _nmsp1
{
    // 源码分析
    // 泛化版本
    template<class _Ty>
    struct remove_all_extents
    {
        using type = _Ty;
    };
    
    // 特化版本1
    template<class _Ty, size_t _Ix>
    struct remove_all_extents<_Ty[_Ix]>
    {
        using type = typename remove_all_extents<_Ty>::type;    // 元编程手法，元编程多数情况下都会涉及到递归
    };
    
    // 特化版本2
    template<class _Ty>
    struct remove_all_extents<_Ty[]>
    {
        using type = typename remove_all_extents<_Ty>::type;  
    };
    
    // []表示是一个无边界数组
    // 什么时候会出现这个无边界数组呢？
    // 比如 在某个.cpp 文件中，定义一个外部变量说明，这个时候，定义的数组就可以不用给长度值
    // extern int g_arr[]; 这就是一个无边界数组，类型就是 int[]
    
    // 那这里是怎么做到，把一个乃至多个维度的[]都给去掉的呢？
    // 这里就用到了递归
    // typename remove_all_extents<_Ty>::type;
    // int c[2][3][4];
    // c的类型是int[2][3][4]
    // 所以在std::remove_all_extents<decltype(c)>::type这行代码里
    // 我们把::type写法理解成一种函数调用，
    // 这个写法就等价于std::remove_all_extents<int[2][3][4]>::type
    // 这个写法就会导致编译器实例化出std::remove_all_extents<int[2][3][4]>这样一个类
    
    // std::remove_all_extents<int[2][3][4]>这种写法就正好满足remove_all_extents的第一个特化版本
    // 满足该特化版本时，他的非类型模板参数_Ix的值就是2(也就是第一个维度，注意不是最后一个维度4)
    // 所以std::remove_all_extents<int[2][3][4]>::type实际得到的类型就被分割了，得到
    // std::remove_all_extents<int[3][4]>::type, [2]就被拆到_Ix里去了
    // 这里就可以看到，通过std::remove_all_extents就把第一维度的数字2给拿掉了
    // 然后就进入递归了
    // std::remove_all_extents<int[3][4]>::type 这个就又满足 std::这种写法就正好满足remove_all_extents的第一个特化版本
    // 这个时候，拿掉 [3], 变成 std::remove_all_extents<int[4]>::type
    // 然后继续递归，还是满足第一个特化版本，拿掉[4]，变成std::remove_all_extents<int>::type
    // 然后继续调用，此时就满足泛化版本，因为没有[]
    // 然后得到type = int;
    // 这个时候,递归结束(因为只有特化版本有::type进行递归，但是泛化版本没有，泛化版本直接给类型赋值)
    
    // std::remove_all_extents类模板的实现用到了递归模板实例化的技术实现
    // std::cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << std::endl;
    // 这行代码最终实例化出来了4个remove_all_extents实例
    // std::remove_all_extents<int[2][3][4]>
    // std::remove_all_extents<int[3][4]>
    // std::remove_all_extents<int[4]>
    // std::remove_all_extents<int>
    
    
    void func()
    {
        int a[12];    // a的类型就是 float [12]
        int b[5][5];    // b的类型就是int[5][8]
        int c[2][2][2];     // c的类型就是int[2][2][2]
        
        std::cout << typeid(std::remove_all_extents<decltype(a)>::type).name() << std::endl;
        // int
        std::cout << typeid(std::remove_all_extents<decltype(b)>::type).name() << std::endl;
        // int
        std::cout << typeid(std::remove_all_extents<decltype(c)>::type).name() << std::endl;
        // int
    }
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}


