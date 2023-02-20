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
    // 计算n的阶乘泛化版本
    template<int n>
    struct Factorial
    {
        enum
        {
            value = n * Factorial<n-1>::value
        };
    };
    
    // 计算n的阶乘特化版本
    template<>
    struct Factorial<0>
    {
        enum
        {
            value = 1
        };
    };
    
    // -----------------------------------
    // 增加指针
    template<typename T>
    using AddPoint = T*;
    
    void func()
    {
        std::cout << Factorial<5>::value << std::endl;
        // 120
        
        AddPoint<const char> s = "const char*";
    }
}

// 混合元编程
// 1）程序员写出一段元编程代码
// 2）编译器根据这段代码编译（生成）一段新代码，实现程序真正功能的是这段生成的新代码
// 3）编译器会对这段新代码进行编译，产生出最终的可执行程序

// 混合元编程可以看成是运行时c++代码生成器

// 常规的计算点积范例
// 1）数组a有3个元素， a[0], a[1], a[2], 值分别是 1， 2， 3
// 2）数组b有3个元素， b[0], b[1], b[2], 值分别是 4， 5， 6
// a和b的点积就是， a[0] * b[0] + a[1] * b[1] + a[2] * b[2] = 32
namespace _nmsp2
{
    // T 是数组元素类型，U是数组大小
    template<typename T, int U>
    auto DotProduct(T* array1, T* array2)
    {
        T dpResult = T{}; // 和值，零初始化
        for (int i = 0; i < U; ++i)
        {
            dpResult += array1[i] * array2[i];
        }
        return dpResult;
    }
    
    void func()
    {
        int a[] = {1, 2, 3};
        int b[] = {4, 5, 6};
        
        int result = DotProduct<int, 3>(a, b);
        
        std::cout << result << std::endl;
        // 32
    }
}

// 代码改进，使用inline
// 从编译出来的汇编代码可以看到，没有inline之前，是通过调用DotProduct<int, 3>实现数组a和数组b的点积运算
// 改成inline后，没有了对DotProduct的调用，而是直接替换成函数本体，这样就导致编译器也不再需要取实例化DotProduct<int, 3>
// 当然因为存在for循环，所以在汇编中会有jge和jmp的判断和跳转的字样，这些判断和调整必然也会对程序运行效率存在一定的影响
namespace _nmsp3
{
    // T 是数组元素类型，U是数组大小
    template<typename T, int U>
    inline auto DotProduct(T* array1, T* array2)
    {
        T dpResult = T{}; // 和值，零初始化
        for (int i = 0; i < U; ++i)
        {
            dpResult += array1[i] * array2[i];
        }
        return dpResult;
    }
    
    void func()
    {
        int a[] = {1, 2, 3};
        int b[] = {4, 5, 6};
        
        int result = DotProduct<int, 3>(a, b);
        
        std::cout << result << std::endl;
        // 32
    }
}

// 使用混合元编程计算点积
// 出发点就是，如果能做到不用循环，还能做到相加，这样必然效率会有提升
// 即
// result += array1[0] * array2[0]
// result += array1[1] * array2[1]
// result += array1[2] * array2[2]
namespace _nmsp4
{
    // 泛化版本
    template<typename T, int U> // T:元素类型， U是数组大小
    struct DotProduct
    {
        static T result(const T* a, const T* b)
        {
            return (*a) * (*b) + DotProduct<T, U-1>::result(a + 1, b + 1);
        }
    };
    
    // 特化版本（递归调用出口）
    template<typename T> // T:元素类型， U是数组大小
    struct DotProduct<T, 0>
    {
        static T result(const T* a, const T* b)
        {
            // 返回0，用T{},零初始化返回 0 
            return T{};
        }
    };
    
    void func()
    {
        int a[] = {1, 2, 3};
        int b[] = {4, 5, 6};
        
        int result = DotProduct<int, 3>::result(a, b);
        
        std::cout << result << std::endl;
    }
}

// 可以预想到，这种递归模板，最终会实例化出
// DotProduct<int, 3>
// DotProduct<int, 2>
// DotProduct<int, 1>
// DotProduct<int, 0>
// 这4个类，然后编译器会按顺序计算出点积值，即
// DotProduct<int, 3>::result(a, b) = 
// (*a) * (*b) + DotProduct<int, 2>::result(a +1, b + 1) =
// (*a) * (*b) + (*(a+1)) * (*(b+1)) + DotProduct<int, 1>::result(a +2, b + 2) =
// (*a) * (*b) + (*(a+1)) * (*(b+1)) + (*(a+2)) * (*(b+2)) + DotProduct<int, 0>::result(a + 3, b + 3) =
// (*a) * (*b) + (*(a+1)) * (*(b+1)) + (*(a+2)) * (*(b+2)) + 0

// c++编译的两个阶段
// 因为有混合元编程的存在，我们可以把c++的编译过程分为2个阶段：前期阶段 和 后期阶段
// 前期阶段，此阶段的c++编译器实际充当的解释器的角色，直接针对程序员开发的c++源码（元编程代码）进行解释性执行
// 这一阶段的工作成果，就是会产生一系列的c++代码（所以元编程才会被看做是运行期c++代码生成器）
// 后期阶段，此阶段c++编译器恢复了大家比较熟悉的c++编译器本来应该具备的功能--- 针对前期阶段产生的结果代码进行编译链接
// 最终生成可执行代码

namespace _nmsp5
{
    // 泛化版本
    template<typename T, int U> // T:元素类型， U是数组大小
    struct DotProduct
    {
        static T result(const T* a, const T* b)
        {
            return (*a) * (*b) + DotProduct<T, U-1>::result(a + 1, b + 1);
        }
    };
    
    // 特化版本（递归调用出口）
    template<typename T> // T:元素类型， U是数组大小
    struct DotProduct<T, 0>
    {
        static T result(const T* a, const T* b)
        {
            // 返回0，用T{},零初始化返回 0 
            return T{};
        }
    };
    
    void func()
    {
        int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
        int b[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
        
        int result = DotProduct<int, 13>::result(a, b);
        
        std::cout << result << std::endl;
        // 884
    }
}

// 比如上面这种有13个元素的计算，c++编译器为了避免产生冗长的实例化代码，c++编译器会将=代码编译成函数调用形式

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    // _nmsp4::func();
    _nmsp5::func();

    return 0;
}
