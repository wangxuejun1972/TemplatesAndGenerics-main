/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

// 成员函数模板
// 基本概念，构造函数模板
// 1）类模板中的成员函数只有源程序代码中出现调用这些成员函数的代码时，这些成员函数才会出现在一个实例化了的类模板中
// 2）类模板中的成员函数模板，只有源程序代码中出现调用这些成员函数模板的代码时，这些成员函数模板才会出现在一个实例化了的类中
// 3）目前编译器并不支持虚成员函数模板：
// 因为虚函数表vptr的大小是固定的，每个vptr的表项里都是一个地址，但是成员函数模板只有在被调用的时候才会实例化出来
// 如果编译器允许模板里虚函数表的存在，那么就会导致虚函数不会被实例化出来，虚函数没有被实例化出来会造成什么结果？
// 就会造成虚函数表vptr的大小没办法进行固定
// c++之父有个说法：如果允许虚函数模板，则每次有人用新的参数类型调用该虚函数模板时，就必须给对应的虚函数表再增加一项
// 这意味着，只有链接程序才能去构造虚函数表，并在表中设置有关函数，因此，成员函数模板绝不能是虚函数
// 4）类模板中有普通的虚函数，这是允许的。因为普通成员函数如果不被调用，是不会别实例化出来
// （这里的实例化出来意思是不会被放到实例化出来的类模板中）
// 但是对于虚函数，不管是否被调用，编译器都会把它实例化出来，因为编译器要创建虚函数表vptr
// 该表中的每个具体的表项，都对应一个具体的虚函数地址，所以编译器必然要把所有的虚函数都给实例化出来

// 拷贝构造函数模板 与 拷贝赋值运算符模板
// 注意区分：
// 1）拷贝构造函数模板不是拷贝构造函数，拷贝赋值运算符模板也不是拷贝赋值运算符（同样：构造函数模板也不是构造函数）
// 因为拷贝构造函数或者拷贝赋值运算符， 要求拷贝的对象类型完全相同，
// 而拷贝构造函数模板和拷贝赋值运算符模板就没有这个要求
// aobj3.m_ic = 12.65f;
// A<float> aobj4(aobj3);
// 这个不会执行拷贝构造函数模板中的代码？
// 并不会（即使这里确实是一个拷贝构造，但是也没有执行类模板中的拷贝构造函数模板）
// std::cout << aobj4.m_ic << std::endl;// 12.65
// 发现，虽然没有执行拷贝构造函数模板，但是，aobj4中的成员已经被拷贝赋值为了12.65
// 这说明对象aobj4确实是通过对象aobj3拷贝构造生成的
// 为什么？
// 首先：因为aobj4的类型是A<float>类型，然后aobj3的类型也是A<float>类型
// 这两个对象类型相同，所以当执行这行代码的时候【A<float> aobj4(aobj3);】，
// 实际上是要执行类模板A中的拷贝构造函数的（注意这里，是拷贝构造函数，不是拷贝构造函数模板）
// 但是现在的情况是类模板A中没有拷贝构造函数，所以编译器内部实际上是执行了按值拷贝动作
// 这个拷贝动作执行完毕，就导致aobj4的值变成了12.65

// 那这时候就有个疑问，该类模板中虽然没有拷贝构造函数，但是有一个拷贝构造函数模板，为什么不执行拷贝构造函数模板呢？
// 因为：拷贝构造函数模板永远不可能成为一个拷贝构造函数。
// 这也就导致需要执行拷贝构造函数的时候，绝不会因为拷贝构造函数模板的存在就用对拷贝构造函数模板的调用去代替对拷贝构造函数的调用
// 所以这里：编译器不会用调用拷贝构造函数模板去代替调用拷贝构造函数

// 那这时候就又有个疑问
// 那拷贝构造函数模板声明时候被调用？
// 类型不同，但是都是用类模板A实例化出来的两个对象，用一个拷贝构造另一个的时候，就会调用拷贝构造函数模板
// 比如：A<double> 拷贝构造 A<float>

// 但是这里有个有有趣的现象。如果拷贝构造函数模板中，参数前面不加const修饰，就会导致不管是相同类型还是不同类型的拷贝构造，
// 都会去调用执行拷贝构造函数模板
// template<typename U>
// A(A<U>& tmpvalue)
// 所以，一般来讲，拷贝构造函数（或者拷贝构造函数模板，参数都得加上const进行修饰）


// （这里拷贝赋值运算符模板和上面拷贝构造函数模板一样，过多的就不在赘述）
// 同样的，拷贝赋值运算符模板用于不可能成为拷贝赋值运算符
// 但是如果去掉拷贝赋值运算符模板中的const修饰符，那么拷贝赋值运算符模板就可以被像拷贝赋值运算符用于调用



// 特化
// 注意，现目前的c++标准，不允许在类模板之外全特化一个未被特化的类模板A的成员函数模板
// 整体感觉，类模板中的成员函数模板全特化可能还不太完善，实际应用中应该注意测试
// 实际项目中建议把这些特化版本实现在类内部，然后类模板一般也要写在头文件中


// 类 / 类模板中的类模板（类模板嵌套）


namespace _nmsp1
{
    template<typename T1>
    class A
    {
    public:
        // 构造函数模板，引入了自己的模板参数T2，与类A的模板参数T1没有任何关系
        template<typename T2>
        A(T2 v1, T2 v2);
        
    public:
        A(double v1, double v2)
        {
            std::cout << "A::A(double v1, double v2)构造函数执行了" << std::endl;
        }
        
        A(T1 v1, T1 v2)
        {
            std::cout << "A::A(T1 v1, T1 v2)构造函数执行了" << std::endl;
        }
        
        // 定义一个普通的成员函数模板
        template<typename T3>
        void myft(T3 v)
        {
            std::cout << "普通的成员函数模板打印 === " << v << std::endl;
        }
        
        // template<typename T4>
        // virtual void virtualFuncT()
        // {
        //     // 报错：模板函数里不允许出现虚函数
        // }
        
        T1 m_ic;
        static constexpr int m_stcvalue = 200;
        
        
    public:
        // 拷贝构造函数模板
        template<typename U>
        A(const A<U>& tmpvalue)
        {
            std::cout << "A(const &A<U> tmpvalue)拷贝构造函数模板执行了" << std::endl;
        }
        
        // 拷贝赋值运算符模板
        template<typename V>
        A<T1>& operator=(const A<V>& tempvalue)
        {
            std::cout << "A<t1>& operator=(const A<V>& tempvalue)拷贝赋值运算符模板执行" << std::endl;
            return *this;
        }
        
        // 拷贝赋值运算符
        // A<T1>& operator=(const A<T1>& tempvalue)
        A& operator=(const A& tempvalue)        // 类内的话，类型参数也可以不要
        {
            std::cout << "A& operator=(const A& tempvalue)拷贝赋值运算符执行" << std::endl;
            return *this;
        }
        
    public:
        // 成员函数模板特化
        // 泛化版本
        template<typename N, typename M>
        void myTeFunc(N v1, M v2)
        {
            std::cout << "成员函数模板泛化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
        }
        
        // 偏特化版本
        template<typename M>
        void myTeFunc(int v1, M v2);
        // {
        //     std::cout << "成员函数模板偏特化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
        // }
        
        // 全特化版本
        // template<>   // linux（gcc编译器下）这种类模板中的全特化版本不能带这个template声明
        void myTeFunc(int v1, float v2)
        {
            std::cout << "成员函数模板全特化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
        }
        
        // 两个打算在类外进行特化的全特化版本
        void myTeFunc(const char* v1, const char* v2);
        void myTeFunc(std::string v1, std::string v2);
        
        // 类模板中再套一个类模板
    public:
        template<typename U>
        class OtherC
        {
        public:
            void myfoc();
        };
        
    };
    
    // 这里补充一点，对于对于这种泛化版本（偏特化版本应该也是一样）的类模板里的嵌套类这里必须加上外层类模板的模板参数声明
    // 在类外实现myfoc
    template<typename T1>
    template<typename U>
    void A<T1>::OtherC<U>::myfoc()
    {
        std::cout << "A<T1>::OtherC<U>::myfoc()执行了" << std::endl;
    }
    
    
    // 在类外实现类模板的构造函数模板
    template<typename T1>   // 先写类模板的template
    template<typename T2>   // 然后写成员函数模板的template，顺序不能乱
    A<T1>::A(T2 v1, T2 v2)
    {
        std::cout << "A<T1>::A(T2 v1, T2 v2)构造函数执行，打印变量值" << v1 << "-----" << v2 << std::endl;
    }
    
    // 在类外实现类模板中的函数模板偏特化版本
    template<typename T1>
    template<typename M>
    void A<T1>::myTeFunc(int v1, M v2)
    {
        std::cout << "成员函数模板偏特化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
    }
    
    // 然后在类外实现一个类模板的函数模板的全特化版本
    // 这种情况下编译会直接报错 {封闭的类模板不是显式特化的}
    // template<typename T1>
    // template<>
    // void A<T1>::myTeFunc(int v1, int v2)
    // {
    //     std::cout << "void A<T1>::myTeFunc(int v1, int v2)成员函数模板全特化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
    // }
    
    // 这个时候就需要先全特化一个类模板，然后才能在全特化的类模板的基础上进行类外函数模板的全特化
    // 全特化一个类模板
    template<>
    class A<std::string>
    {
    public:
        // 成员函数模板特化
        // 泛化版本
        template<typename N, typename M>
        void myTeFunc(N v1, M v2)
        {
            std::cout << "全特化类A<std::string>的成员函数模板泛化版本执行， 打印值" << v1 << "****" << v2 << std::endl;
        }
        
        void myTeFunc(const char* v1, const char* v2);
        void myTeFunc(std::string v1, std::string v2);
        
    public:
        template<typename U>
        class OtherC
        {
        public:
            void myfoc();
            // {
            //     std::cout << "OtherC::myfoc()执行了" << std::endl;
            // }
        };
    
    };
    
    // 这个时候就可以进行类模板的函数模板的类外全特化了
    // template<>   // 同样的linux（gcc编译器下，全特化模板不能带上空模板声明）
    // 而且这里与vs编译器下不同的地方是，这个特化版本，必须在特化类内部进行定义
    // 【vs版本编译器因为在类模板的泛化版本里已经进行了函数模板声明，那这里就不需要重复声明，但是gcc下需要】
    void A<std::string>::myTeFunc(const char* v1, const char* v2)
    {
        std::cout << "myTeFunc(const char* v1, const char* v2)类外实现类模板的函数模板的全特化版本" << v1 << "--" << v2 << std::endl;
    }
    
    void A<std::string>::myTeFunc(std::string v1, std::string v2)
    {
        std::cout << "myTeFunc(std::string v1, std::string v2)类外实现类模板的函数模板的全特化版本" << v1 + v2 << std::endl;
    }
    
    
    // 将myfoc()实现在类模板外部
    // template<>  // 注意这里，这行类模板的模板参数声明，在gcc下可以编译通过（不加也可以通过）
    // 但是在vs环境下，编译会报错，所以在类模板外部实现嵌套类模板的函数的时候，这里就统一不加这个了（不管是不是空的类模板参数）
    // 这里补充一点，应该是对于全特化版本的类模板里的嵌套类是这样
    template<typename U>
    void A<std::string>::OtherC<U>::myfoc()
    {
        std::cout << "A<std::string>::OtherC<U>::myfoc()执行了" << std::endl;
    }
    
    
    
    void func()
    {
        A<float> aobj(12, 43);
        // A<T1>::A(T2 v1, T2 v2)构造函数执行，打印变量值12-----43
        // 实例化了一A<float>这样一个类型，并用int类型来实例化构造函数
        
        
        // aobj.myft<float>(89.34f);
        aobj.myft(89.34f);  // 也可以不写类型参数，编译器会自动类型推断
        // 普通的成员函数模板打印 === 89.34
        A<float> aobj2(12.45, 43.55);
        // A<float>已经被上面的代码实例化过了，并用double类型实例化了一个double类型参数的构造函数
        // 但是如果这里类中自己添加了一个double类型的构造函数，那么这里就不会继续实例化一个double类型的构造函数
        // 而是会调用类中已经有个那个double构造函数
        
        A<float> aobj3(12.34f, 43.3f);
        // A<float>已经被上面的代码实例化过了，并用float类型实例化了一个float类型参数的构造函数
        
        
        // A<T1>::A(T2 v1, T2 v2)构造函数执行，打印变量值12-----43
        // 普通的成员函数模板打印 === 89.34
        // A::A(double v1, double v2)构造函数执行�
        // A::A(T1 v1, T1 v2)构造函数执行了
        
        
        std::cout << "---------------------------------" << std::endl;
        aobj3.m_ic = 12.65f;
        A<float> aobj4(aobj3);
        // 这个不会执行拷贝构造函数模板中的代码？
        // 并不会（即使这里确实是一个拷贝构造，但是也没有执行类模板中的拷贝构造函数模板）
        std::cout << aobj4.m_ic << std::endl;// 12.65
        // 发现，虽然没有执行拷贝构造函数模板，但是，aobj4中的成员已经被拷贝赋值为了12.65
        // 这说明对象aobj4确实是通过对象aobj3拷贝构造生成的
        
        std::cout << "---------------------------------" << std::endl;
        A<char> aobj5(aobj3);
        // aobj5的类型是A<std::string>，aobj3的类型是 A<float>,这两对象类型不同，就会调用拷贝构造函数模板进行拷贝构造
        std::cout << aobj5.m_ic << std::endl;   // null
        
        
        // 拷贝赋值运算符
        std::cout << "---------------------------------" << std::endl;
        aobj3 = aobj4; // 类型相同
        // A& operator=(const A& tempvalue)拷贝赋值运算符执行
        // 没有执行拷贝赋值运算符模板
        aobj3 = aobj5; // 类型不同
        // A<t1>& operator=(const A<V>& tempvalue)拷贝赋值运算符模板执行
        
        
        // 成员函数模板特化
        std::cout << "---------------------------------" << std::endl;
        aobj2.myTeFunc(12, "test");
        // 成员函数模板偏特化版本执行， 打印值12****test
        aobj2.myTeFunc("55", 'c');
        // 成员函数模板泛化版本执行， 打印值55****c
        aobj2.myTeFunc(123, 99.34f);
        // 成员函数模板全特化版本执行，打印值123****99.34
        
        // 类外成员函数模板的全特化
        std::cout << "---------------------------------" << std::endl;
        A<std::string> aobj7;
        aobj7.myTeFunc("hello", "world");
        // myTeFunc(const char* v1, const char* v2)类外实现类模板��函数模板的全特化版本hello--world
        aobj7.myTeFunc(std::string("nihao"), std::string("shijie"));
        // myTeFunc(std::string v1, std::string v2)类外实��类模板的函数模板的全特化版本nihaoshijie
    
        
        // 类模板中的类模板（类嵌套）
        std::cout << "---------------------------------" << std::endl;
        A<std::string>::OtherC<float> myobjOtherC1;
        myobjOtherC1.myfoc();
        // A<std::string>::OtherC<U>::myfoc()执行了
        A<long>::OtherC<float> myobjOtherC2;
        myobjOtherC2.myfoc();
        // A<T1>::OtherC<U>::myfoc()执行了
    }
}

int main()
{
    _nmsp1::func();

    return 0;
}


