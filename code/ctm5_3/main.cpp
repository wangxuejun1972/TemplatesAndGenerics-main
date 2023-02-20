/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <list>

using namespace std;
// 通过容器（数组）类型萃取元素类型范例
// 

namespace _nmsp1
{
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType;
    
    // vector特化版本
    template<typename T>
    struct GetEleType<std::vector<T>>
    {
        using type = T;
    };
    
    // list特化版本
    template<typename T>
    struct GetEleType<std::list<T>>
    {
        using type = T;
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    void func()
    {
        std::cout << "vector<double> 元素类型 = " << typeid(GetEleType<vector<double>>::type).name() << std::endl;
        // vector<double> 元素类型 = double
        std::cout << "list<string> 元素类型 = " << typeid(GetEleType<list<string>>::type).name() << std::endl;
        // list<string> 元素类型 = std::string
        
        std::cout << "float[24] 元素类型 = " << typeid(GetEleType<float[24]>::type).name() << std::endl;
        // float[24] 元素类型 = float
        std::cout << "float[24] 元素数量 = " << GetEleType<float[24]>::size << std::endl;
        // float[24] 元素��量 = 24
    }
}

namespace _nmsp2
{
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType;
    
    // vector特化版本
    template<typename T>
    struct GetEleType<std::vector<T>>
    {
        using type = T;
    };
    
    // list特化版本
    template<typename T>
    struct GetEleType<std::list<T>>
    {
        using type = T;
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    // 引入函数模板PrintEleType
    template<typename T>
    void PrintEleType(const T& Container)
    {
        std::cout << "容器或者数组的元素类型为 = " << typeid(typename GetEleType<T>::type).name() << std::endl;
    }
    
    void func()
    {
        vector<double> vb;
        list<int> li;
        float sc[34]; 
        PrintEleType(vb);
        // 容器或者数组的元素类型为 = double
        PrintEleType(li);
        // 容器或者数组的��素类型为 = int
        PrintEleType(sc);
        // 容器或者数组的元素类型为 = float
    }
}

namespace _nmsp3
{
    
    // GetEleType代码改进
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType
    {
        // 改造泛化版本，实现对容器类型的参数T的元素类型获取
        using type = typename T::value_type;    // 针对容器
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    // ---------------------
    // 定义别名模板
    template<typename T>
    using GetEleType_T = typename GetEleType<T>::type;
    
    // ---------------------
    // 引入函数模板PrintEleType
    template<typename T>
    void PrintEleType(const T& Container)
    {
        std::cout << "容器或者数组的元素类型为 = " << typeid(GetEleType_T<T>).name() << std::endl;
    }
    
    void func()
    {
        vector<double> vb;
        list<int> li;
        float sc[34]; 
        PrintEleType(vb);
        // 容器或者数组的元素类型为 = double
        PrintEleType(li);
        // 容器或者数组的��素类型为 = int
        PrintEleType(sc);
        // 容器或者数组的元素类型为 = float
        
        std::cout << "vector<double>的元素类型 = " << typeid(vector<double>::value_type).name() << std::endl;
        // vector<double>的元素类型 = double
        
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
