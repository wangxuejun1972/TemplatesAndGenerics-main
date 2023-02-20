/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// 使用了SFINEA特性的信息萃取
// SFINEA 替换失败并不是一个错误
// 用成员函数重载实现is_default_constructible
// c++标准库中： std::is_default_constructible 类模板
// 该类模板的能力是用于判断一个对象是否能被默认构造
// （也就是构造一个类的时候，不需要给该类传递任何参数, 只要该类没有构造函数或者有一个不带参数的构造函数就都能默认构造）

namespace _nmsp1
{
    class A
    {
        
    };
    
    class B
    {
        int m_i;
    public:
        B(int a):m_i(a){}
    };
    
    void func()
    {
        // A可以默认构造
        A a;
        // B不能被默认构造
        // B b;
        B b(10);
        // **----------------------------------**
        std::cout << std::is_default_constructible<A>() << std::endl;       // 1
        std::cout << std::is_default_constructible<B>::value << std::endl;  // 0
    }
}

namespace _nmsp2
{
    // 判断两个类型是否相同
    template<class T1, class T2>
    struct IsSameType
    {
        static const bool value = false;
    };
    
    template<class T>
    struct IsSameType<T, T>
    {
        static const bool value = true;
    };
    
    // ----------------------------------
    
    template<typename T>
    class IsDefaultConstructible
    {
    private:
        template<typename = decltype(T())>
        static std::true_type test(void*);
        
        template<typename = int>
        static std::false_type test(...);
        
    public:
        static constexpr bool value = IsSameType<decltype(test(nullptr)), std::true_type>::value;
    };
    
//     首先 IsDefaultConstructible 这个类模板中有两个同名的静态成员函数模板，其中第一个函数模板的返回值是true_type
//     第二个函数模板的返回值是false_type，第一个函数模板的形参时 void*， 第二个函数模板的形参时...，返回类型不同，参数也不同
//     （...）是c语言中的省略号形参，代表可以接收0到任意多个实参，要注意，这里两个函数模板的模板参数都有默认值
//     第一个函数模板的默认值比较关键，decltype(T())，这里显然是生成一个T对象，那么这里能不能生成，直接就决定了它能不能被缺省构造
//     然后第二个函数模板缺省参数就给int，其实这里给其他类型也都可以
//     这两个test（函数模板）都是只有声明，没有实现体。因为做这种类型推断的这种，一般都是用decltype来进行类型推断，
//     这种往往不需要具体实现
    
//     对于test这两个静态成员函数，在调用这两个重载函数的时候，编译器会优先选择有具体形参的test版本，也就是会优先选择
//     第一个test函数模板，因为他有具体的形参（void*），而第二个是(...)，然后看有具体形参的test版本是否匹配，只有这个有具体形参
//     的函数模板版本不匹配的时候，才会选择带（...）这个形参的test版本，换句话说，带（...）的版本时具有最低匹配优先级的
   
//   value的最终取值是怎么得到的，【IsSameType<decltype(test(nullptr)), std::true_type>::value】
// 这里用到is_same_type来判断 decltype(test(nullptr)) 类型和 std::true_type 是否类型相等
// 进一步说，就是 decltype(test(nullptr)) 的类型，它这里使用decltype来判断test的返回类型，
// 试想，如果传递给IsDefaultConstructible的类型参数T支持默认构造，那么编译器显然在选择test的时候，
// 编译器就会选择第一个test，并通过decltype推导出该test的返回值类型，（std::true_type）,那么这个整个的value就是true
    
    class A
    {
        
    };
    
    class B
    {
        int m_i;
    public:
        B(int a):m_i(a){}
    };
    
    void func()
    {
        std::cout << IsDefaultConstructible<A>::value << std::endl;
        std::cout << IsDefaultConstructible<B>::value << std::endl;
    }
}

namespace _nmsp3
{
    template<typename T>
    class IsDefaultConstructibleHelper
    {
    private:
        template<typename = decltype(T())>
        static std::true_type test(void*);
        
        template<typename = int> //这行代码其实可以不要
        static std::false_type test(...);
    
    public:
        using type = decltype(test(nullptr));
        // 如果能默认构造，那么这个type = true_type，反之为false_type
        // 所以这里type是一个类型，下面这个继承就是正确的
    };
    
    template<typename T>
    class IsDefaultConstructible : public IsDefaultConstructibleHelper<T>::type
    {};
    // 当类型T支持默认构造的时候，IsDefaultConstructible的父类就是true_type
    // 当类型T不支持默认构造的时候，IsDefaultConstructible的父类就是false_type
    
    class A
    {
        
    };
    
    class B
    {
    public:
        B(int a){};
    };
    
    void func()
    {
        std::cout << IsDefaultConstructible<A>::value << std::endl;
        std::cout << IsDefaultConstructible<B>::value << std::endl;
    }
}

// is_convertible c++标准库中的一个类模板，用于判断一个类型能否隐式的装换到另一个类型
// 、比如一般的从int转到float，从float转到int这些都是可以的，

namespace _nmsp4
{
    // 用成员函数重载实现is_convertible
    class A
    {};
    
    class B:public A
    {};
    // 因为类B的父类是类A，所以从类B装换到类A是可以的，但是从类A转换到类B是不可以的
    
    void func()
    {
        std::cout << std::is_convertible<float, int>() << std::endl;  // 1
        std::cout << std::is_convertible<int, float>() << std::endl;  // 1
        std::cout << std::is_convertible<B, A>() << std::endl;  // 1
        std::cout << std::is_convertible<A, B>() << std::endl;  // 0
    }
}

namespace _nmsp5
{
     // 用成员函数重载实现is_convertible
    class A
    {};
    
    class B:public A
    {};
    // 因为类B的父类是类A，所以从类B装换到类A是可以的，但是从类A转换到类B是不可以的
    
    // 实现测试能否从FROM类型到TO类型的隐式类型装换 的类模板
    template<class FROM, class TO>
    struct IsConvertibleHelper
    {
    private:
        static void testFunc(TO);
        
        template<typename = decltype(testFunc(std::declval<FROM>()))>
        static std::true_type test(void *);
        
        template<typename = decltype(testFunc(std::declval<FROM>()))>
        static std::false_type test(...);
        
    public:
        using type = decltype(test(nullptr));
    };
    // test这两个重载函数，他的返回类型分别是true_type和false_type，如果能从FROM类型转到TO类型
    // 那么就会匹配返回true_type的这个test成员函数模板，否者就会匹配返回false_type的这个成员函数模板
    // 值得注意的是返回true_type的这个成员函数模板的模板参数默认值写法
    // decltype(testFunc(std::declval<FROM>()))
    // 看起来是使用decltype去推断 testFunc(std::declval<FROM>()) 这个成员函数的返回类型
    // 而传递给testFunc这个成员函数的实参 是 std::declval<FROM>()
    // 这个东西他其实就是一个（或者看成一个） FROM 类型的对象
    // 如果FROM类型能被顺利的转换为TO类型的话，那么通过decltype去推断testFunc的返回类型的写法
    // （也就是第一个重载的写法）就是有效的,那么test就会返回true_type，否者就会匹配第二个test，返回false_type
    
    // 注意这里为什么能用testFunc这个函数来进行类型筛选呢？
    // 因为 testFunc 他本身接收的是 TO类型的形参，如果FROM能转成TO类型，自然就会选择第一个test，反之就会选择第二个test
    
    template<typename FROM, typename TO>
    struct IsConvertible : public IsConvertibleHelper<FROM, TO>::type
    {};
    
    // 简化书写，使用变量模板
    template<typename FROM, typename TO>
    constexpr bool IsConvertible_v = IsConvertible<FROM, TO>::value;
    
    void func()
    {
        std::cout << IsConvertible<float, int>::value << std::endl;  // 1
        std::cout << IsConvertible<int, float>::value << std::endl;  // 1
        std::cout << IsConvertible<B, A>::value << std::endl;  // 1
        std::cout << IsConvertible<A, B>::value << std::endl;  // 0
        
        // ----------------------------------
        
        std::cout << IsConvertible_v<float, int> << std::endl;  // 1
        std::cout << IsConvertible_v<int, float> << std::endl;  // 1
        std::cout << IsConvertible_v<B, A> << std::endl;  // 1
        std::cout << IsConvertible_v<A, B> << std::endl;  // 0
    }
    
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    // _nmsp4::func();
    _nmsp5::func();

    return 0;
}


