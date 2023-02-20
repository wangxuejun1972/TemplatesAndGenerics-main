
#include <stdio.h>
#include <iostream>
#include <string>

namespace _nmsp1
{
    // 重载
    // 函数或者函数模板名字相同，但是参数数量或者参数类型不同
    // 函数模板和函数也可以同时存在，此时可以把函数看成是一种重载
    // 当普通函数和函数模板调用都比较合适的时候，编译器会选择普通函数来进行调用
    // 如何选择最合适的（最特殊）的函数模板/函数，编译器内部有一套比较复杂的排序规则
    template<typename T>
    void myfunc(T tempvalue)
    {
        std::cout << "myfunc(T tempvalue)执行" << std::endl;
    }
    
    template<typename T>
    void myfunc(T* tempvalue)
    {
        std::cout << "myfunc(T* tempvalue)执行" << std::endl;
    }
    
    void myfunc(int tempvalue)
    {
        std::cout << "myfunc(int tempvalue)执行" << std::endl;
    }
    
    
    void func()
    {
        myfunc(12);     // 当普通函数和函数模板调用都比较合适的时候，编译器会选择普通函数来进行调用
        char *p = nullptr;
        myfunc(p);
        myfunc(23.4f);
    }
}

namespace _nmsp2
{
    // 特化
    // 泛化：大众化，常规的，常规情况下，写的函数模板都是泛化的函数模板
    // 特化版本往往代表从泛化版本中抽离出来的一组子集
    // 全特化:就是把泛化版本的所有模板参数都用具体的类型来代替构成一个特殊的版本
    // 全特化等价于实例化一个函数模板，并不等价于函数重载
    // 编译器考虑的顺序：优先选择普通函数，然后才会考虑选择函数模板的特化版本，最后才会考虑选择函数模板的泛化版本
    
    
    // 偏特化（局部特化）
    // 1）模板参数数量上的偏特化
    // 实际上，从模板参数数量上来讲，函数模板不能被偏特化，否者会导致编译错误
    // 2）模板参数范围上的偏特化
    // int -> const int 类型变小， T -> T*, T -> T&,  T->T&&,针对T类型从类型范围上变小
    // 实际上，对于函数模板来讲，也不存在模板范围上的偏特化，因为这种所谓的模板参数范围上的偏特化
    // 实际上是函数的重载
    // 3）既然函数模板不能偏特化，那就只能通过函数重载进行解决
    // 3-1）模板函数模板参数数量上的重载
    
    
    template<typename T, typename U>
    void tfunc(T &tmpv1, U& tmpv2)
    {
        std::cout << "tfunc泛化版本 ==> " << tmpv1 << "----------" << tmpv2 << std::endl;
    }
    
    // 全特化版本
    template<>
    // void tfunc(int &tmpv1, double& tmpv2)
    void tfunc<int, double>(int &tmpv1, double& tmpv2)  // <int, double>这个可以省略，因为根据实参完全可以推导出形参类型
    {
        std::cout << "tfunc全特化版本 ==> " << tmpv1 << "----------" << tmpv2 << std::endl;
    }
    
    // 从模板参数数量上进行偏特化(编译错误)
    // template<typename U>
    // void tfunc<const char *, U>(const char *&tmpv1, U& tmpv2)
    // {
    //     std::cout << "tfunc<char *, U>(char *&tmpv1, U &tmpv2)偏特化版本 ==>" << tmpv1 << "----------" << tmpv2 << std::endl;
    // }
    
    // 从模板参数范围上进行偏特化（编译错误）
    // template<typename T, typename U>
    // void tfunc<const T&, u>(const T& tmpv1, U& tmpv2)
    // {
    //     std::cout << "tfunc<const T&, u>(const T& tmpv1, U& tmpv2)偏特化 ==> " << tmpv1 << "----------" << tmpv2 << std::endl;
    // }
    
    // 模板函数重载版本
    template<typename T, typename U>
    void tfunc(const T& tmpv1, U& tmpv2)
    {
        std::cout << "tfunc<const T&, u>(const T& tmpv1, U& tmpv2)模板函数重载版本 ==> " << tmpv1 << "----------" << tmpv2 << std::endl;
    }
    
    template<typename U>
    void tfunc(char *&tmpv1, U& tmpv2)
    {
        std::cout << "void tfunc(char *&tmpv1, U& tmpv2)重载版本 ==>" << tmpv1 << "----------" << tmpv2 << std::endl;
    }
    
    // template<typename U>
    // void tfunc(char(&tmpv1)[], U& tmpv2)
    // {
    //     std::cout << "void tfunc(char[] &tmpv1, U& tmpv2)重载版本 ==>" << tmpv1 << "----------" << tmpv2 << std::endl;
    // }
    
    
    
    // 重载函数
    void tfunc(int &tmpv1, double& tmpv2)
    {
        std::cout << "tfunc函数重载 ==> " << tmpv1 << "----------" << tmpv2 << std::endl;
    }
    
    
    void func()
    {
        const char *p = "fenyuwuzhu";
        int i = 123;
        
        tfunc(p, i);
        // T的类型 = const char *
        // U的类型 = int
        // tmpv1 = const char * &
        // tmpv2 = int &
        
        double d = 34.3f;
        const int ck = 124;
        
        tfunc(i, d);
        
        tfunc(ck, d);
        // tfunc<const T&, u>(const T& tmpv1, U& tmpv2)模板函�重载版本 ==> 124----------34.3

        std::string a = "iojioij";
        tfunc(a, d);
    }
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}

