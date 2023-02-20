/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// std::is_class c++11中用于判断某个类型是否是一个类类型（不是联合类型）的类模板

namespace _nmsp1
{
    // IsSameType
    template<typename T, typename F>
    struct IsSameType
    {
        static const bool value = false;
    };
    
    template<typename T>
    struct IsSameType<T, T>
    {
        static const bool value = true;
    };
    
    // -------------------------------------------
    template<typename T>
    class IsClass
    {
    private:
        template<typename U>
        static std::integral_constant<bool, !std::is_union<U>::value> test(int U::*);
        
        template<typename>
        static std::integral_constant<bool, false> test(...);
        
    public:
        static constexpr bool value = IsSameType<
            decltype(test<T>(nullptr)),
            std::integral_constant<bool, true>
        >::value;
    };
    
    // 依旧是两个同名的静态成员函数模板，test，返回类型都是integral_constant
    // integral_constant<bool, !std::is_union<U>::value> 在这里如果U（其实就是这里的T）的类型不是一个联合类型，
    // 那么这整个这个东西就变成true，否者返回类型就变成了一个false
    // 对于这两个test，编译器依旧会优先选择有具体形参的test版本
    // 对于这个有具体形参的版本的形参 （int U::*）
    // 这个对象其实是一个成员变量指针，这里只是为了简化书写，把成员变量指针名 给去掉，因为本身也没有用到这个指针名
    // 所以，这里很关键。如果U他是一个类类型的话，那么（int U::*）这种写法就是成立的，也就是说
    // 只要他是一个类类型，那么他就可以写这种成员变量指针
    // 反之，如果U不是一个类类型，那么这种成员变量指针写法就不符合语法规则
    // 、然后根据SFINEA特性,因为第一个test版本它不符合语法规则，所以他就只能去匹配第二个test版本
    // 如果他去匹配第二个test版本，那么这里他就一定能匹配成功，因为它这里是不挑参数的。
    
    // 然后继续看，IsClass 这个整个的value的值取决于 test的返回值类型和 std::integral_constant<bool, true>
    // 这两个类型是否相同，如果相同，那么就为true，也就是表示是一个类类型，而不是一个联合类型
    
    // -------------------------------------------
    class A{};
    class B:public A{};
    // -------------------------------------------
    
    void func()
    {
        std::cout << IsClass<A>::value << std::endl;    // 1
        std::cout << IsClass<B>::value << std::endl;    // 1
        std::cout << IsClass<int>::value << std::endl;  // 0
        
        // 成员变量指针 T::*
        int A::* mp = nullptr;
        // 这个代码表示，有个名字叫做 mp的指针
        // 它可以指向类A中的一个int类型成员变量
        // 注意：这行代码并不表示，类A中一定要有一个int类型的成员变量
        // 即使类A是一个空类也没有关系，从语法上讲，这个代码他就没有问题
        // 当然如果说这里要给mp附一个非nullptr（非空）的值的话，那么就要求类A中确实要有一个int类型的成员变量
    }
}

// std::is_base_of c++11用于判断某个类是否是某个类的父类的类模板
// template<typename Base, typename Derive>
// struct is_base_of{...}
// 注意他与is_convetible的关系，is_convertible用于判断某个类能否转换到另一个类，这两个类型之间并不一定要要有父子关系


namespace _nmsp2
{
    
    class A{};
    class B:public A{};
    
    void func()
    {
        std::cout << std::is_base_of<A, B>::value << std::endl; // 1
        std::cout << std::is_base_of<B, A>::value << std::endl; // 0
        std::cout << std::is_base_of<A, A>::value << std::endl; // 1
        std::cout << std::is_base_of_v<B, B> << std::endl;  // 1
    }
}

namespace _nmsp3
{
    // 注意顺序 《父， 子》
    template<class Base, class Derive>
    class IsBaseOf
    {
    private:
        template<typename T>
        static std::true_type test(T*);
        
        template<typename>
        static std::false_type test(void *);
        
        template<typename B, typename D>    // 父， 子
        static auto test_middle()->decltype(test<B>(static_cast<D*>(nullptr)));
        // 这个函数调用用到了返回类型后置语法
        // 他的返回类型是 decltype(test<B>(static_cast<D*>(nullptr)))
        // 他使用了decltype来推导【test<B>(static_cast<D*>(nullptr))】的返回类型
        // 所以问题的焦点就在于 test<B>(static_cast<D*>(nullptr)) 这个代码段
        // 该代码段调用的是上面的静态成员函数模板 test
        // 如果D是B的子类，那么显然 第一个test版本它就合适调用
        // 为什么？因为注意看这里test调用，它的模板参数类型实例化时B类型，但是传递进去的却是D类型
        // static_cast<D*>(nullptr) 将一个nullptr指针转为D类型指针
        // 只有在D类型是B类型的子类的时候，才能用B （父类）去接收这个 D* (子类指针)，
        // 那么编译器就会优先选择第一个test版本进行调用, 然后返回true_type，那就判断出D是B的子类了
        // 反之，如果D不是B的子类，那么就会去调用void*的那个重载版本，从而返回fale_type
        
    public:
        static constexpr bool value = std::is_same<
            std::integral_constant<
                bool,
                std::is_class_v<Base> &&
                std::is_class_v<Derive> &&
                decltype(test_middle<Base, Derive>())::value>,
            std::integral_constant<bool, true>
        >::value;
    };
    
    // 从这个代码中可以看出，要想认定Base是Derive的父类，需要具备3个条件
    // 1）std::is_class_v<Base> Base必须要是一个类类型，
    // 2）std::is_class_v<Derive> Derive也必须要是一个类类型
    // 3）decltype(test_middle<Base, Derive>())::value> 返回类型必须要是一个true
    // 然后对第三点展开说说他做了啥：那也就是要求 test_middle<Base, Derive>()他的返回类型要是一个true_type
    // 所以，核心代码其实也就在这里 test_middle<Base, Derive>()
    // 
    

    class A{};
    class B:public A{};
    
    void func()
    {
        std::cout << IsBaseOf<A, B>::value << std::endl; // 1
        std::cout << IsBaseOf<B, A>::value << std::endl; // 0
        std::cout << IsBaseOf<A, A>::value << std::endl; // 1
    }
}

// 辅助理解 static_cast<D*>(nullptr)
namespace _nmsp4
{
    class PaR{};    // 父类
    class Son:public PaR{}; // 子类
    class Other{};  // 其他类
    
    void f(PaR *)   // 使用父类指针作为函数形参
    {
        // 这里使用父类指针作为函数形参，是不是就相当于上面 
        // test<B>(static_cast<D*>(nullptr))
        // 这里的调用呢？
        std::cout << "void f(PaR *) " << std::endl;
    }
    
    // 新增一个f的重载版本，相当于上面的第二个test重载
    void f(void *)
    {
        std::cout << "void f(void *)" << std::endl;
    }
    
    void func()
    {
        f(static_cast<PaR*>(nullptr));  // 没问题
        // void f(PaR *) 
        
        f(static_cast<Son*>(nullptr));  // 也没问题，因为Son是PaR的子类类型
        // void f(PaR *) 
        
        // 、父类类型接收子类指针没问题A（子指针转到父指针没有任何问题）
        f(static_cast<Other*>(nullptr));
        // 编译错误
        // 因为Other*和PaR没有任何关系，不能通过Other*转到 PaR*
        // 当有了第二个f的重载版本的时候，这里就没问题了，因为他这里调用的是第二个重载（void *）
        // 因为你任何类型的指针都可以用void *来接收
        // void f(void *)
    }
}

// 用类模板特化的方式实现 is_default_constructible
// 需要用到std::void_t这样一个别名模板，能够检测到应用SFINEA特性时出现的非法类型
// template<typename... Args>
// using void_t = void;
// 
namespace _nmsp5
{
    // 泛化版本
    template<typename T, typename U = std::void_t<>>
    class IsDefConstructible:public std::false_type
    {
        // 当T类型不能被缺省构造的时候，因为SFINEA特性，就走的是泛化版本的分支，继承false_type

    };
    
    // 特化版本
    template<typename T>
    class IsDefConstructible<T, std::void_t<decltype(T())>>:public std::true_type
    {
        // 首先，该类模板首先会优先调用特化版本
        // 如果T类型能被缺省构造，那么decltype(T())中的T()就能被构造，这种写法就是合法的，那么就选择这个特化版本
        // 然后因为继承自true_type，所以就为 1
    };
    
    
    // -------------------------------------------
    class A{};
    class B
    {
    public:
        B(int a){};
    };
    
    
    void func()
    {
        std::cout << IsDefConstructible<A>::value << std::endl; // 1
        std::cout << IsDefConstructible<B>() << std::endl; // 0
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
