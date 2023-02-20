/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <boost/type_index.hpp>

using namespace std;

namespace _nmsp1
{
    // 这里我们自己实现的这个RemoveConst和标准库里提供的std::remove_const类似
    
    // const修饰符的移除
    template<typename T>
    struct RemoveConst
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveConst<const T>
    {
        using type = T;
    };
    // 根据需要可以增加其他特化版本
    
    template<typename T>
    using RemoveConst_t = typename RemoveConst<T>::type;
    
    void func()
    {
        RemoveConst_t<const int> m = 23;
        m = 3535;
    }
}

// 退化（decay）技术，某些类型一旦传递给函数模板，或者通过函数模板来进行某些类型参数的推断，推断出来的类型会产生退化
// 退化，就是把类型中的一些修饰符给丢弃了（const int -> int），对于const int来说，这就是一种退化

// const 修饰符，&， &&修饰符被丢弃，数组类型变成指针类型，函数名变成函数指针等等这些都是类型退化的表现

// c++标准库中： std::decay， 该类模板的作用就是能够把一个类型退化掉


namespace _nmsp2
{
    template<class T>
    void testType(T tmpv)
    {
        std::cout << "---------------------------------begin------------------------------" << std::endl;
        
        using boost::typeindex::type_id_with_cvr;
        
        std::cout << "T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
        
        std::cout << "tmpv = " << type_id_with_cvr<decltype(tmpv)>().pretty_name() << std::endl;
        
        std::cout << "---------------------------------endl-------------------------------" << std::endl;
    }
    
    void testFunc()
    {
        
    }
    
    
    void func()
    {
        const int a = 15;
        const int& b = a;
        const int&& c = 124;
        int arr[5] = {1, 2, 3, 4, 5};
        
        testType(a);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(b);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(c);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(arr);
        // ---------------------------------begin------------------------------
        // T = int*
        // tmpv = int*
        // ---------------------------------endl-------------------------------
        
        testType(testFunc);
        // ---------------------------------begin------------------------------
        // T = void (*)()
        // tmpv = void (*)()
        // ---------------------------------endl-------------------------------
        
        
        // --------------------------------------------------------------------
        
        std::decay<const int&>::type nb = 1214;
        std::cout << "nb的类型为： " << typeid(decltype(nb)).name() << std::endl;
        // nb的类型为: int
    }
}


// 如何实现类似std::decay这样的代码？
// 需要前置代码 RemoveConst和RemoveReference

// 无边界数组
extern int g_array[];

namespace _nmsp3
{
    // const修饰符的移除
    template<typename T>
    struct RemoveConst
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveConst<const T>
    {
        using type = T;
    };
    // 根据需要可以增加其他特化版本
    
    template<typename T>
    using RemoveConst_t = typename RemoveConst<T>::type;
    // --------------------------------------------------------------------
    
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
    
    // 定义别名模板
    template<typename T>
    using RemoveReference_t = RemoveReference<T>::type;
    // --------------------------------------------------------------------
    
    // 实现std::decay
    
    template<typename T>
    struct RemoveCR:RemoveConst<typename RemoveReference<T>::type> // 继承，然后把const和引用都去除
    {};
    // typename RemoveReference<T>::type
    // 这段代码是用来把类型参数中的引用给去掉，然后执行得到的结果仍然是类型
    // 然后在把去掉了引用类型的类型作为RemoveConst模板的模板参数，放到尖括号中
    // 
    // 然后因为继承关系，RemoveCR继承了RemoveConst,所以，RemoveCR就也有type这么个类型成员
    // 这个type类型成员，就是最初那个去掉了引用，去掉了const修饰符的类型
    // 然后，再通过别名模板简化书写
    
    template<typename T>
    using RemoveCR_t = typename RemoveCR<T>::type;
    
    // --------------------------------------------------------------------
    // 当然，我们也可以直接写别名模板，不用写RemoveCR这个类模板
    // template<typename T>
    // using RemoveCR_tt = RemoveConst_t<typename RemoveReference_t<T>>;
    
    
    // --------------------------------------------------------------------
    // 泛化版本
    template<class T>
    struct Decay:RemoveCR<T>
    {
        // 当前泛化版本的Decay只能做到把const和引用给去掉，这还不够
    };
    
    // 特化版本1：处理数组 把有边界数组变成指针
    // 该特化版本不继承任何父类
    // 而且我们这里的数组是有边界数组（数组是有大小的）
    template<class T, std::size_t Size>
    struct Decay<T[Size]>
    {
        using type = T*;
    };
    
    // 无边界数组：
    // 某个.cpp源文件中定义一个全局数组，int g_array[10];，数组类型是int[10]
    // 然后在另一个文件中需要用到这个全局量（在这个源文件中就变成一个无边界数组了，他的类型就是 int[]）
    // extern int g_array[]; // 外部变量声明
    
    // 特化版本2：处理数组 把无边界数组变成指针
    // 该特化版本不继承任何父类
    // 这个特化版本是针对无边界数组
    template<class T>
    struct Decay<T[]>
    {
        using type = T*;
    };
    
    // --------------------------------------------------------------------
    void testFunc2()
    {
        std::cout << "testFunc2（）执行了" << std::endl;
    }
    // 函数类型：由函数返回值类型和函数参数类型决定，与函数名关系不大
    // 所以，本函数的函数类型 void()
    // 可以利用函数指针指向某种函数类型。若要指向testFunc2，那么函数指针的类型就应该是
    // void(*)()
    // void (*P)() = testFunc2;    // 定义一个函数指针p指向testFunc2
    // p(); 
    // --------------------------------------------------------------------
    
    // 行数名退化为函数指针
    template<typename T, typename... Args>
    struct Decay<T(Args...)>    // 返回类型T，参数类型是一包参数类型Args...
    {
        using type = T(*)(Args...);
    };
    
    // --------------------------------------------------------------------
    // 定义别名模板
    template<typename T>
    using Decay_t = typename Decay<T>::type;
    
    
    
    void func()
    {
        RemoveCR_t<const int &&> rc = 235;  // rc int类型
        rc = 3535;
        
        int ar[2] = {1, 2};
        Decay<decltype(ar)>::type myarray;
        std::cout << "myarray的类型为： " << typeid(decltype(myarray)).name() << std::endl;
        // myarray的类型为： int*
        
        Decay<decltype(g_array)>::type nboerArray;
        std::cout << "nboerArray的类型为： " << typeid(decltype(nboerArray)).name() << std::endl;
        // nboerArray的类型为： int*
        
        // 如果不为函数名退化成函数指针 写一个Decay的特化版本，观察一些测试代码
        // 发现，如果不写退化代码，那么给进去什么类型，返回的就是什么类型，不会退化为函数指针
        Decay<decltype(testFunc2)>::type rfunc;
        // 表面上看,这行代码定义了一个函数类型的变量,但是实际上我们把它理解成函数声明更好
        std::cout << "rfunc 类型为 ：" << typeid(decltype(rfunc)).name() << std::endl;
        // rfunc 类型为 ：void(void)
        // 添加了函数名的退化代码后
        // rfunc 类型为 ：void(*)(void)
        
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
