/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

#include <vector>
#include <list>
#include <deque>

// 可变参类模板和可变参模板一样，他也允许模板参数中包含0到任意多个模板参数
// 参数包展开方式有多种

// 通过递归继承方式展开类型。非类型， 模板模板参数包

// 注意一些点：
// 1）这种一包参数在模板中智能出现一次，不能说你写一个 两包参数 template<typename... a1, typename... a2>这种是不行的
// 2）这种参数的必须放在最后的位置，不能说<First, Other..., ooo> ，当然这种是可以的 <First, OOO, Other...>
// 当然这两条规则是针对类模板的泛化版本来说的，对于类模板的特化版本，不一定会遵照这两条，后续再说

// 类型模板参数包展开
//
// 该范例取自c++标准库中的tuple（元组），元祖的参数展开也就是这种递归继承式的展开方式
// 
// 有参构造函数怎么写，初始化列表怎么写，如何给成员变量赋值，如果要调用父类的有参构造函数的话该怎么写？



// 非类型的模板参数包的展开
// 参数包，不但可以是类型，还可以是非类型，还可以是模板模板参数包


// 模板模板参数包的展开


namespace _nmsp1
{
    // 泛化版本(主模板定义)
    template<typename... T>
    class CT
    {
    public:
        CT()
        {
            std::cout   << "CT::CT()泛化版本构造函数执行了。this = "
                        << (void*)this
                        << "  sizeof...(Other) = "
                        << sizeof...(T)
                        << std::endl;
        }
    };
    
    // 当然，也有人不写定义，只写主模板声明（泛化版本模板声明）这也是可以的
    // 这种声明叫做前向声明（前置声明）这个写法不报错的前提是你在代码中没有用主模板去创建对象
    // 如果你实际代码中药用到，那就必须得再对他进行定义
    // 当然。后面学习中会遇到很多只申明，不定义的情景，后面再说
    // 只要记住一点：这种只写类模板声明不定义类模板的编程手法很重要，他能够帮助程序员排错
    // template<typename... T> class CT;
    
    // 和函数模板的参数包递归展开思路一样，类模板的递归继承方式展开需要一个偏特化版本的类模板
    
    // 偏特化版本
    template<typename First, typename... Other>
    class CT<First, Other...> : private CT<Other...>
    {
    public:
        First m_i;
    
    public:
        // 构造函数
        CT():m_i(0)
        {
            std::cout   << "CT<First>::CT()构造函数执行了。this = "
                        << (void*)this
                        << "  sizeof...(Other) = "
                        << sizeof...(Other)
                        << std::endl;
        }
        
        // 有参构造函数怎么写?
        // 初始化列表怎么写？
        // 注意这里，如果我希望构造的时候执行的是父类的有参构造函数版本，那我这里就得把相关的参数往父类里面传
        CT(First value, Other... prop):m_i(value), CT<Other...>(prop...)
        {
            std::cout << "--------------------------begin-----------------------------" << std::endl;
            std::cout << "CT<First, Other...>::CT(First value, Other... prop) 执行了, this = " << (void*)this << std::endl; 
            std::cout << "m_i = " << m_i << "  sizeof...(Other) = " << sizeof...(Other) << std::endl;
            std::cout << "---------------------------endl-----------------------------" << std::endl;
        }
    };
    
    // 写一个模板参数为0个的特化版本
    // 看起来像全特化，实际上不是，因为可变参类模板不存在全特化的说法
    template<>
    class CT<>
    {
    public:
        CT()
        {
            std::cout   << "CT::CT()模板参数为0个的特化版本构造函数执行了。this = "
                        << (void*)this
                        << std::endl;
        }    
    };
    
    void func()
    {
        // CT<int, float, double> myCT1; 
        // CT::CT()泛化版本构造函数执行了。this = 0x7ffd390bcac0  sizeof...(Other) = 0
        // CT<First>::CT()构造函数执行了。this = 0x7ffd390bcac0  sizeof...(Other) = 0
        // CT<First>::CT()构造函数执行了。this = 0x7ffd390bcac0  sizeof...(Other) = 1
        // CT<First>::CT()构造函数执行了。this = 0x7ffd390bcac0  sizeof...(Other) = 2
        
        // 可以看到，执行了4个构造函数，实例化出来了4个类
        // 首先执行这行代码行的时候，这里有3个类型模板参数，那么系统就会调用构造函数去实例化3个类型模板参数的类模板
        // 根据我们这里的写法，3个类型模板参数的类模板继承的是2个类型模板参数的类模板，所以系统会先去实例化2个类型参数的类模板
        // 在实例化两个类型参数的类模板的时候，发现
        // 这2参数类型的类模板继承的又是一个类型参数的类模板，所以系统就会先去实例化一个类型参数的类模板
        // 然后在实例化一个类型参数的时候，发现他继承自一个0个模板参数的类模板，那系统就会先去实例化这个0个参数的类模板
        // 也就是最先去实例化我们的泛化版本（因为泛化版本里模板参数的数量可以是0到任意多个）
        // 所以这就是为什么会 是上面打印的那个执行顺序了
        
        // 那这里就有个问题：
        // 如果我自己写一个带0个模板参数的特化版本的类模板，是否也可以呢？
        // 当然是可以的，添加一个0个模板参数的特化版本后，系统就不会去执行哪个泛化版本，而来执行我们写的这个特化版本
        // CT::CT()模板参数为0个的特化版本构造函数执行了。this = 0x7ffc3648d550
        // CT<First>::CT()构造函数执行了。this = 0x7ffc3648d550  sizeof...(Other) = 0
        // CT<First>::CT()构��函数执行了。this = 0x7ffc3648d550  sizeof...(Other) = 1
        // CT<First>::CT()构造函数执��了。this = 0x7ffc3648d550  sizeof...(Other) = 2
        
        // 学过对象模型，这些this指针因为继承关系，所以都是一样的。这里不过多解释
        
        
        CT<int, float, double, std::string> myCT2(100, 23.3f, 34.33, "success");
        // CT::CT()模板参数为0个的特化版本构造函数执行了��this = 0x7fff73177610
        // --------------------------begin-----------------------------
        // CT<First, Other...>::CT(First value, Other... prop) 执行了, this = 0x7fff73177610
        // m_i = success  sizeof...(Other) = 0
        // ---------------------------endl-----------------------------
        // --------------------------begin-----------------------------
        // CT<First, Other...>::CT(First value, Other... prop) 执行了, this = 0x7fff73177610
        // m_i = 34.33  sizeof...(Other) = 1
        // ---------------------------endl-----------------------------
        // --------------------------begin-----------------------------
        // CT<First, Other...>::CT(First value, Other... prop) 执行了, this = 0x7fff73177610
        // m_i = 23.3  sizeof...(Other) = 2
        // ---------------------------endl-----------------------------
        // --------------------------begin-----------------------------
        // CT<First, Other...>::CT(First value, Other... prop) 执行了, this = 0x7fff73177610
        // m_i = 100  sizeof...(Other) = 3
        // ---------------------------endl-----------------------------
        
    }
}


namespace _nmsp2
{
    
    template<int... args>   // int 用 auto 代替也行
    class CT
    {
    public:
        CT()
        {
            std::cout << "CT::CT()泛化版本构造函数执行了" << std::endl;
        }
    };
    
    // 后续设计模式的时候再谈这里public继承和private继承有什么含义
    // 偏特化
    template<int first, int... args>
    class CT<first, args...>:private CT<args...>
    {
    public:
        CT()
        {
            std::cout   << "CT<first>::CT()构造函数执行了。this = "
                        << (void*)this
                        << "  sizeof...(Other) = "
                        << sizeof...(args)
                        << "  first = " << first
                        << std::endl;
        }
    };
    
    
    void func()
    {
        CT<12, 232, 43, 2> mmyCT3;
        // CT::CT()泛化版本构造函数执行了
        // CT<first>::CT()构造函数执行了。this = 0x7ffddc806767  sizeof...(Other) = 0  first = 2
        // CT<first>::CT()构造��数执行了。this = 0x7ffddc806767  sizeof...(Other) = 1  first = 43
        // CT<first>::CT()构造��数执行了。this = 0x7ffddc806767  sizeof...(Other) = 2  first = 232
        // CT<first>::CT()构造函数执行了。this = 0x7ffddc806767  sizeof...(Other) = 3  first = 12
    }
}


namespace _nmsp3
{
    template<typename T, template<class>class... Container>
    class ParentNM
    {
    public:
        ParentNM()
        {
            std::cout << "ParentNM::ParentNM()泛化版本构造函数执行了" << std::endl;
        }
    };
    
    template<typename T,
        template<typename>class FirstContainer,
        template<typename>class... OtherContainer>
    class ParentNM<T,FirstContainer,  OtherContainer...>:private ParentNM<T, OtherContainer...>
    {
    
    public:
        ParentNM()
        {
            std::cout   << "ParentNM::ParentNM()偏特化版本构造函数执行了。this = "
                        << (void*)this
                        << "  sizeof...(OtherContainer) = "
                        << sizeof...(OtherContainer)
                        << std::endl;
            pushIn(5);
        }
        
    public:
        FirstContainer<T> m_container;
        
        void pushIn(int size)
        {
            for (int i = 0; i < size; ++i)
            {
                m_container.push_back("(T)(i)");
            }
        }
        
        void popOut()
        {
            for(auto iter = m_container.begin(); iter != m_container.end(); ++iter)
            {
                std::cout << "打印容器内的值：" << *iter << std::endl;
            }
        }
    };
    
    template<typename T, template<typename>class... Container>
    class MyCT:private ParentNM<T, Container...>
    {
    public:
        MyCT()
        {
            std::cout   << "MyCT::MyCT()构造函数执行了。this = "
                        << (void*)this
                        << "  sizeof...(Container) = "
                        << sizeof...(Container)
                        << "  T的类型是"
                        <<  typeid(T).name()  
                        << std::endl;
        }
    };
    
    
    void func()
    {
        
        MyCT<std::string, std::vector, std::list, std::deque> myCT4;
        // ParentNM::ParentNM()泛化版本构造函数执行了
        // ParentNM::ParentNM()偏特化版本构造函数执行了。this = 0x7fff29ff4a20  sizeof...(OtherContainer) = 0
        // ParentNM::ParentNM()偏特化版本构造函数执行了。this = 0x7fff29ff4a20  sizeof...(OtherContainer) = 1
        // ParentNM::ParentNM()偏特化版本构造函数执行了。this = 0x7fff29ff4a20  sizeof...(OtherContainer) = 2
        // MyCT::MyCT()构造函数执行了。this = 0x7fff29ff4a20  sizeof...(Container) = 3  T的类型是NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE


        // 这行代码实例化出来这5个类：
        // 1）ParentNM<string>
        // 2）ParentNM<string, deque>
        // 3）ParentNM<string, list, deque>
        // 4）ParentNM<string, vector, list, deque>
        // 5）MyCT<string, vector, list, deque>
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
