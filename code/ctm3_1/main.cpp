/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// 万能引用（universal reference 、 forwarding reference 【转发引用】）
// 类型区别基本含义

// universal reference / 万能引用 / 未定义引用 基本认识
// 结论：万能引用是一种类型，就跟int是一种类型是一个道理
// 右值引用 （全程：右值引用类型）用 && 表示，
// 右值引用是绑定到右值上

// 万能引用离不开两种语境
// 1）必须是函数模板
// 2）必须是发生了模板类型推断并且函数模板形参长这样： T&& 

// 注意： T&& 这3个放在一起才是万能引用， T本身不是万能引用
// 虽然万能引用和右值引用长得像，但是万能引用要求T必须是一个类型模板参数

// 对于万能引用
// 如果实参传递来了一个整形左值给形参，那么形参 tmprv的类型会被编译器最终推断为 int & 类型
// 如果实参传递来了一个整形右值给形参，那么形参 tmprv的类型会被编译器最终推断为 int && 类型

// 结论: T&& 是一个万能引用类型

namespace _nmsp1
{
    void funcTest(const int& abc) {}
    // 问？abc什么类型 答：const int &类型
    
    void myfunc(int && tmprv)   // 参数tmprv是右值引用类型
    {
        std::cout << tmprv << std::endl;
        return;
    }
    
    
    void func()
    {
        int &&rv = 100;
        
        myfunc(100);    // 100
        // 正确，整形数右值做实参
        
        // myfunc(rv);
        // 错误，rv是左值， &&rv是右值引用，
        // 这里参数要求传递进去右值作为实参，rv是左值，所以不能把左值绑定到右值引用上
    }
}


namespace _nmsp2
{
    // 将函数改造为函数模板
    
    template<typename T>
    void myfunc(T && tmprv)   // 注意 && 是属于tmprv的一部分，他和 T没有关系，不是T类型的一部分
    {
        tmprv = 19; // 不管tmprv的类型是左值引用还是右值引用，都可以在这里进行赋值
        // 因为tmprv的类型是左值引用类型或者右值引用类型，但是 tmprv他本身是左值
        std::cout << tmprv << std::endl;
        return;
    }
    
    
    void func()
    {
        int &&rv = 100;
        
        myfunc(100);    // 100
        // 正确，整形数右值做实参
        
        myfunc(rv); // 100
        // 将函数改造为函数模板后，发现及时你传递进来的是一个左值，他也不会报错
        // 即。如论实参传递的是左值还是右值，都不会报错
        
        int i = 100;
        myfunc(i); 
        // 左值被传递，因此tmprv是一个左值引用，也就是int &，最终i的值变成19
        std::cout << "i = " << i << std::endl;
        // i = 19

        i = 200;
        myfunc(std::move(i)); 
        // 右值被传递，因此tmprv是一个右值引用，也就是int &&，最终i的值变成19
        std::cout << "i = " << i << std::endl;
        // i = 19
    }
}

namespace _nmsp3
{
    template<typename T>
    void myfunc(const T&& tmprv)    // 有const修饰，万能引用资格被剥夺，然后因为 && 的存在，所以只能变成右值引用
    {
        std::cout << tmprv << std::endl;
    }
    
    void func()
    {
        int i = 100;
        // myfunc(i);
        // 报错
        myfunc(std::move(i));
    }
}

namespace _nmsp4
{
    
    template<typename T>
    class CT
    {
    public:
        void testFunc(T&& x)    // 这个不是万能引用，而是右值引用
        {
            
        }
        // 那为什么这里这个成员函数不是万能引用？
        // 因为T这里不涉及到类型推断，或者说，testFunc这个成员函数本身没有涉及到类型推断
        // 因为他作为类模板的成员函数，只有在类模板被实例化的时候，他才存在，那么当他存在的时候
        // 也就说明类模板以及完成实例化了，那么也就是说这时候，T的类型已经确定好了，自然就不涉及到类型推断
    
    public:
        // 如何让成员函数也变成万能引用类型呢？
        // 把函数改造为模板函数，同时引入新的类型模板参数T2
        template<typename T2>
        void funcT2(T2&& tmprv)
        {
            
        }
        
    };
    
    
    void func()
    {
        CT<int> c1;
        int i = 100;
        // c1.testFunc(i);
        // 报错，不是万能引用，只能传递右值进去。左值作为实参是不能绑定到右值引用上
        c1.testFunc(std::move(i));
        
        c1.funcT2(i);
        c1.funcT2(100);
        
        // funcT2实参既能给左值，又能给右值，就说明了funcT2的形参类型是一个万能引用类型
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


