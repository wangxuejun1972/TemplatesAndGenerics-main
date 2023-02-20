/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
// #include <boost/type_index.hpp>

using namespace std;

// 引用类型的移除和增加
// 在C++标准库中，提供了一个
// std::remove_reference
// 这是一个类模板，如果传递进来的模板参数是一个引用类型，那么该类模板会把这个引用部分给去掉

namespace _nmsp1
{
    // 引用类型的移除
    template<class T1, class T2>
    void print_is_same()
    {
        std::cout << "T1类型为：" << typeid(T1).name() << std::endl;
        std::cout << "T2类型为：" << typeid(T2).name() << std::endl;
        std::cout << "T1类型和T2类型是否相等？ " << std::is_same<T1, T2>() << std::endl;
        // std::is_same<>() 如果输出值为 1 ，表示两个值类型相等，如果输出值为0，表达两个值类型不等
        // is_same是标准库中用于判断两个类型是否相同的类模板
        // std::is_same<T1, T2>() 和 std::is_same<T1, T2>::value这两个写法的效果都是一样的
        // 这里加()就是产生了一个 std::is_same<T1, T2> 这个类型的临时对象，
        // 同时这个()也会调用该类模板的类型装换运算符，把这个对象转换成一个整形值来进行输出
        // 源码里就是对应 下面这段代码
        // constexpr operator value_type() const noexcept
        // {
        //     return value;
        // }
        
    }
    
    void func()
    {
        std::remove_reference<int>::type a;
        std::remove_reference<int&>::type b;
        std::remove_reference<int&&>::type c;
        
        print_is_same<decltype(a),decltype(b)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相�？ 1
        print_is_same<decltype(a),decltype(c)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相等？ 1
        print_is_same<decltype(b),decltype(c)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相等？ 1
        
        
        // ------------------------------
        // 在c++14中，std::remove_reference定义了大量别名(比如)
        // template<class T>
        // using remove_reference_t = remove_reference<T>::type;
        
    }
}

namespace _nmsp2
{
    // 泛化版本
    template<typename T>
    struct RemoveReference
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&>
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&&>
    {
        using type = T;
    };
    
    // ------------------------------
    // 定义别名模板
    template<typename T>
    using RemoveReferenceT = RemoveReference<T>::type;
    
    void func()
    {
        int&& a = 12;
        int b = 32;
        int &c = b;
        
        RemoveReferenceT<decltype(a)> a2 = 90;
        RemoveReferenceT<decltype(c)> c2 = 220;
        
        // using boost::typeindex::type_id_with_cvr;
        // std::cout << "a2 = " << type_id_with_cvr<decltype(a2)>().pretty_name() << std::endl;
        // // int
        // std::cout << "c2 = " << type_id_with_cvr<decltype(c2)>().pretty_name() << std::endl;
        // // int
    }
}

namespace _nmsp3
{
    // 引用类型的增加
    // 根据给定的类型来创建一个左值或者右值引用
    // std::add_lvalue_reference类模板（给进来一个类型，用于返回该类型的左值引用类型）
    // std::add_rvalue_reference类模板（给进来一个类型，返回该类型的右值引用类型）
    // std::is_lvalue_reference和std::is_rvalue_reference类模板，用于判断是否是左值引用类型或者右值引用类型
    
    void func()
    {
        int a =13;
        int i = 90;
        std::add_lvalue_reference<decltype(a)>::type b = a;
        std::cout << "b是否是左值引用类型 = " << std::is_lvalue_reference<decltype(b)>::value << std::endl;
        // b是否是左值引用类型 = 1
        
        std::add_rvalue_reference<decltype(a)>::type c = std::move(a);
        std::cout << "c是否是右值引用类型 = " << std::is_lvalue_reference<decltype(c)>::value << std::endl;
        // c是否是右值引用类型 = 0
        
        using btype = std::add_lvalue_reference_t<int>; // _t 表示是一个模板别名
        std::cout << "btype 是否和类型 int& 类型相等？" << std::is_same<int&, btype>() << std::endl;
        // btype 是否和类型 int& 类型相等？1
        
        using ctype = std::add_rvalue_reference_t<int>;
        std::cout << "ctype是否是左值引用类型" << std::is_lvalue_reference<ctype>::value << std::endl;
        // ctype是否是左�引用类型0
        
        std::cout << "ctype是否是右值引用类型" << std::is_rvalue_reference<ctype>::value << std::endl;
        // ctype是否是右值引用类型1
        
        std::add_rvalue_reference_t<int&> ccl = i;
        // ccl类型为int&， 这里用到了引用折叠 & + && = &
        std::add_rvalue_reference_t<int&&> ccr = 63;
        // ccr类型为int&&，这里也用到了引用折叠 && + && = &&
    }
}

namespace _nmsp4
{
    template<typename T>
    struct AddLvalueReference
    {
        using type = T&;  
    };
    template<typename T>
    using AddLvalueReference_t = typename AddLvalueReference<T>::type;
    // 实际上这种写法可以直接简化成, 就不用定义 AddLvalueReference
    // using AddLvalueReference_t = T&;
    
    template<typename T>
    struct AddRvalueReference
    {
        using type = T&&;  
    };
    template<typename T>
    using AddRvalueReference_t = typename AddRvalueReference<T>::type;
    
    
    void func()
    {
        int anew = 100;
        AddLvalueReference_t<decltype(anew)> bnew = anew;   // bnew 的类型 int&
        
        int&& anew2 = 12;
        AddLvalueReference_t<decltype(anew2)> bnew2 = anew; // 引用折叠之后，bnew2的类型int&
    
        
        int ar = 23;
        AddRvalueReference_t<decltype(ar)> dgd = 2424; // dgd 的类型 是 int&&
        
        int&& arr = 34;
        AddRvalueReference_t<decltype(arr)> ggg = 3523; // ggg 的类型是 int&&
        
        int aa = 24;
        int& argv = aa;
        AddRvalueReference_t<decltype(argv)> tttt = aa;
        // 引用折叠之后， tttt的类型变成了 int& 左值引用类型（这与这个类模板的语义不符合，这就是错误的使用导致的后果）
        
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    // _nmsp3::func();
    _nmsp4::func();

    return 0;
}
