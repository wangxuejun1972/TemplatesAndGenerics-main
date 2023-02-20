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
    // 缺省参数
    // 函数模板的缺省参数可以放在最前面，这点和类模板不一样，类模板要求，
    // 从第一个缺省模板参数开始，后续的，模板参数都必须有缺省类型参数
    
    template<typename T = int, typename U>
    void testFunc2(U m)
    {
        std::cout << m << std::endl;
    }
    
    // 普通函数
    int mf(int tmp1, int tmp2)
    {
        return tmp1 * tmp2;
    }
    
    template<typename T>
    T addFunc(T v1, T v2)
    {
        return v1 + v2;
    }
    
    // 定义一个函数指针类型
    typedef int(*FuncType)(int, int);
    
    template<typename T, typename F = FuncType>
    void testFunc(T i, T j, F functionPointer = mf)
    {
        std::cout << "functionPointer(i, j) = " << functionPointer(i, j) << std::endl;
    }
    
    
    
    void func()
    {
        testFunc(23, 365);
        
        FuncType fadd = addFunc;
        
        testFunc(90.34f, 34.35f, fadd);
        
        testFunc2(89.34f);
    }
}

namespace _nmsp2
{
    // 非类型模板参数
    // 1）概念：前面的函数模板涉及到的模板参数都是“类型模板参数”，需要使用typename和class进行修饰
    // 除此之外，模板参数还可以是“非类型”模板参数（普通的参数）
    // c++17开始支持非类型模板参数为auto自动类型推断
    // 注意：
    // 1）指定非类型模板参数的值时，一般给的都是常量，不能给变量，因为编译器要求在编译期间就能确定非类型模板参数的值
    // 2）并不是任何类型的参数都可以作为函数模板的非类型参数。int类型可以，double*可以，但是double和float或者类类型（比如string）等等都不可以
    // 一般允许做非类型模板参数的类型如下（随着c++的迭代，可能会增加）
    // a）整形或者枚举类型
    // b）指针类型
    // c）左值引用类型
    // d）auto 或者 decltype(auto)
    // 可能还有其他类型
    
    // 比较奇怪的写法
    // 1）不管是类型模板参数还是非类型模板参数，如果代码中没有用到，则参数名都可以省略
    // 2）类型前面可以增加一个typename修饰以明确标识一个类型，一般跟模板有关的类型名前面是需要加typename
    
    // 注意：这里的非类型模板参数（val）位置是在尖括号里面【很好理解，因为模板参数都是在尖括号里面】
    // template<typename T, typename U, int val = 100>
    template<typename T, typename U, auto val = 100>
    auto Add(T v1, U v2)
    {
        std::cout << "非类型模板参数值为 = " << val << std::endl;
        return v1 + v2;
    }
    
    // ------------------------------------
    template<typename, int>
    auto ffunc()
    {
        return 1;
    }
    
    // ------------------------------------
    // 比如这种，前面这个是正常的模板类型参数前缀标识，可以用class，
    // 但是后面这个是用typename来修饰表明int是一个类型（这里这样做就明显画蛇添足了，因为int本来就是一个类型，但是这样写并没有语法错）
    // 所以后面这个typename和前面的那个不是一个东西，后面这个不能用class替换。前面的是类型模板参数语法前缀，后面这个是一个类型标识
    template<typename T, typename int value>
    auto testFn()
    {
        return 100;
    }
    
    
    void func()
    {
        std::cout << Add<std::string,std::string>("pao", "hui") << std::endl;
        // 非类型模板参数值为 = 100
        // paohui
        std::cout << Add<float,float, 990>(43.43f, 90.33f) << std::endl;
        // 非类型模板参数值为 = 990
        // 133.76
        int a = 12;
        std::cout << Add<int, double>(76, 34.48) << std::endl;
        
        std::cout << ffunc<int, 100>() << std::endl;
    }
    
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}

