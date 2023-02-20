
#include <stdio.h>
#include <iostream>
#include <boost/type_index.hpp>

using namespace std;

// std::declval  是c++11新标准中出现的函数模板。这个函数模板很奇怪，因为这个函数模板没有函数体
// （只有声明，没有实现），所以它没办法被调用。
// 它一般都是用于与decltype、sizeof等关键字配合来进行类型推导，占用内存空间计算等

// std::declval源码
// template<class _Ty>
// add_rvalue_reference_t<_Ty>declval() noexcept;

// add_rvalue_reference
// 也是c++11标准库中提供的一个类模板，他的能力是给定一个类型，他能够返回该类型的右值引用类型
// 1）比如给定一个int类型，他返回的就是 int &&
// 2）如果给进来一个int & 类型，那么他返回的还是 int &。 （注意这里是用到了引用折叠）
// 3）给定一个int &&类型，返回的还是int &&类型（依然是发生了引用折叠）

// 所以结合来看：std::declval 的能力：就是返回类型T的右值引用，不管该类型是否有默认构造函数
// 或者该类型是否可以创建对象（抽象类是不能创建对象的）
// 返回某个类型T的右值引用这个动作是在编译时完成的，所以很多人也把std::declval称为编译时工具
// 当然，我们很少见到哪个函数会返回右值引用的，所以std::declval的地位比较特殊


namespace _nmsp1
{
    class A
    {
    public:
        A(int i)
        {
            std::cout << "A::A()执行了， this = " << this << std::endl;
        }
        
        // 如果在实际工作中，我们想把myfunc这个成员函数的返回类型取到手的话
        // 如果是传统的做法，我们可能就得这样写代码
        
        double myfunc()
        {
            std::cout << "A::myfunc()执行了， this = " << this << std::endl;
            return 12.33;
        }
    };
    
    void func()
    {
        using YT = decltype(std::declval<A>());
        // 这里注意，declval是函数模板，所以使用的时候要加() 【std::declval<A>()】，虽然这里不加()编译也不会报错
        // 但是不加之后含义就完全变了，也正是因为不加()编译还不报错，所以这里要格外注意。
        // 可以想象成函数调用的时候，即便函数没有形参也得把()带上
        
        // 利用boost库输出类型名
        using boost::typeindex::type_id_with_cvr;
        std::cout << "YT类型 = " << type_id_with_cvr<YT>().pretty_name() << std::endl;
        // YT类型 = _nmsp1::A&&
        
        // 如果在实际工作中，我们想把myfunc这个成员函数的返回类型取到手的话
        // 如果是传统的做法，我们可能就得这样写代码
        A mya1(1);
        // A::A()执行了， this = 0x7ffd8dc9a18f
        
        std::cout << "mya1.myfunc()的返回类型 = " << type_id_with_cvr<decltype(mya1.myfunc())>().pretty_name() << std::endl;
        
        // mya1.myfunc()的返回类型 = double
        
        // 从上面这种传统做法中获取一个类的普通成员函数的返回值类型，
        // 1）我必须创建一个该类的对象
        // 2）在这种写法中 【type_id_with_cvr<decltype(mya1.myfunc())>().pretty_name()】
        // 从打印结果上看，没有调用myfunc()函数，就获得了该函数的返回值类型。
        // 这也就是使用decltype获取类型的好处
        
        // 那现在如果我们既不想创建类A对象，又想把该对象的成员函数的返回值类型拿到，该怎么实现呢？
        // 借助std::declval就能实现
        std::cout << "A::myfunc()的返回类型 = " << type_id_with_cvr<decltype(std::declval<A>().myfunc())>().pretty_name() << std::endl;
        // A::myfunc()的返回类型 = double
        // 可以看到这种写法成功的获取到了函数返回类型
        // 这里让人比较迷惑的地方就是 【std::declval<A>().myfunc()】这段代码
        
        // 我们先看下面这段代码
        A&& mayy1();
        // 这行代码看起来像是一个函数声明的语法，返回类型是A&&,函数名mayy1，参数列表为空
        // mayy1();
        // 这行代码就像是函数调用一样
        // 但是这两段代码结合起来，如果只是编译，发现没有编译错误，但是一旦到连接生成可执行文件的时候
        // 就会报链接错误。
        // 这个报错并不奇怪。因为对于这个mayy1这个函数来说。我们只声明没有定义，那么在调用的时候就会报错这很正常
        
        // 注意 【A&& mayy1();】这个代码段的函数，既然返回类型是A&&，那么就可以看成是返回了一个A&&类型的对象
        // 该对象可以看成是类A对象，因此，你这种对象去调用类A对象的myfunc成员函数就没有语法错
        // mayy1().myfunc(); 
        // 这行代码也就和上面一样，编译阶段没问题，链接的时候报错
        
        // 既然如此，那么在std::declval这个函数模板存在的情况下，修改一下代码，就可以做到编译链接都不报错
        
        decltype(mayy1().myfunc()) mydbvalue;
        // 通过decltype去获取该对象的成员函数返回值类型（这里是double），也就相当于这里 double mydbvalue;
        // 为什么这里不报错呢？
        // 因为我们知道，decltype他在获取函数返回值类型的时候，是不需要去真实调用函数的，那既然如此，这里也就不需要为该函数提供函数体
        // 所以也就不报错
        
        // 所以，这样总结下来，这一堆事情所做的，不就是【std::declval<A>().myfunc()】这个代码段做的事吗？
        
        // 所以，如果我们不借助std::declval这个函数模板的话，就不会存在这么一个假想的类A对象
        // 不存在这么一个假想的类A对象，也就不能通过decltype()去虚假调用myfunc这个成员函数
        // 也就没有办法拿到这个成员函数的返回类型
        
        // 而且，这种写法【std::declval<A>()】不需要给类A提供实参就能创建一个类A对象，
        // 这种创建类对象的方式比较在泛型模板编程中比较通用
        
        // std::declval的作用
        // 1）从类型转换的角度来讲，他是将一个类型转换成右值引用类型
        // 2）从假想创建出某个类型对象的角度来说，配合decltype，让decltype表达式中不必经过该类型
        // 的构造函数就能使用该类型的成员函数
        // 注意：std::declval不能被调用，也不能创建任何对象。
        // 但是他的能力就是能在不创建对象的情况下，达到创建了一个该类型对象的假想效果
        // 程序员可以假定创建了一该类型对象
        
    }
}

namespace _nmsp2
{
    // 返回类本身是不好的
    // 在了解了std::declval的一些基本知识后，可能会有一个疑问，这个东西为什么非要返回右值引用？
    // 他是不是返回左值引用或者类型本身也可以？
    
    // 为了做测试，我这里自己实现一个类似std::declval的代码
    
    template<typename T>
    // T myDeclval() noexcept;         // 直接返回T 会导致出现语义限制问题
    // T& myDeclval() noexcept;        // 返回 T& 左值引用类型
    T&& myDeclval() noexcept;       // 返回右值引用
    
    
    class A
    {
    public:
        A(int i)
        {
            std::cout << "A::A()执行了， this = " << this << std::endl;
        }
        
        double myfunc()
        {
            std::cout << "A::myfunc()执行了， this = " << this << std::endl;
            return 12.33;
        }
        
    private:
        ~A(){}
    };
    
    void func()
    {
        using boost::typeindex::type_id_with_cvr;
        std::cout << "myDeclval<A>()的返回类型 = " << type_id_with_cvr<decltype(myDeclval<A>())>().pretty_name() << std::endl;
        // myDeclval<A>()的返回类型 = _nmsp2::A
        std::cout << "myDeclval<A>().myfunc()的返回类型 = " << type_id_with_cvr<decltype(myDeclval<A>().myfunc())>().pretty_name() << std::endl;
        // myDeclval<A>().myfunc()的返回类型 = double
    
        
        // 仅从当前范例来看，std::declval无论返回是右值引用还是左值引用还是类型本身都不影响我们后续的获取成员函数的类型
        
        // 但是当我们给类A添加了一个私有的析构函数的时候。再次编译运行发现出现了无法访问私有析构函数的错误。
    
        // 从他当前无法访问该类A对象的析构函数来看，执行这个获取类型代码是构造了一个临时的A类对象，然后在执行析构函数的时候访问出错
        // 当然这个只是语义上创建了一个类对象，但是实际上并没有。
        // 而且这个报错是第二行引起的，关键就在于myDeclval<A>().myfunc()这个他要遵循一个语义限制
        // 从语义上它需要创建一个A类对象，然后要执行析构函数。但是实际上并没有创建
        
        // 因为我们自己写的这个myDeclval返回的是类型本身，导致为了遵循语义限制，编译器内部创建了临时的A类对象，
        // 为了绕开语义限制，在设计myDeclval函数模板时，就不要返回类型T了，要么返回左值引用，要么返回右值引用
        // 这样从遵循语义限制方面来说，就不会创建临时的A类对象了，这就是返回T& 或者 T&&的好处
    
        
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        
        // 查看A类型返回左值引用是返回啥
        std::cout << "decltype(myDeclval<A>())的返回类型 = " << type_id_with_cvr<decltype(myDeclval<A>())>().pretty_name() << std::endl;
        // decltype(myDeclval<A>())的返回类型 = _nmsp2::A&
        // 改成返回右值引用后
        // decltype(myDeclval<A>())的返回类型 = _nmsp2::A&&
        
        
        // 查看A类型的左值引用返回左值引用是返回啥
        std::cout << "decltype(myDeclval<A&>())的返回类型 = " << type_id_with_cvr<decltype(myDeclval<A&>())>().pretty_name() << std::endl;
        // decltype(myDeclval<A&>())的返回类型 = _nmsp2::A&
        // 改成返回右值引用后
        // decltype(myDeclval<A&>())的返回类型 = _nmsp2::A&
        
        
        // 查看A类型的右值引用返回左值引用是返回啥
        std::cout << "decltype(myDeclval<A&&>())的返回类型 = " << type_id_with_cvr<decltype(myDeclval<A&&>())>().pretty_name() << std::endl;
        // decltype(myDeclval<A&&>())的返回类型 = _nmsp2::A&
        // 改成返回右值引用后
        // decltype(myDeclval<A&&>())的返回类型 = _nmsp2::A&&
        
        // 通过这个打印看到，不管我们给定的是左值引用还是右值引用，返回的都是左值引用
        // （其实这里发生了引用折叠，很好理解，因为我们是左值引用，&，所以不管他给的是左值&还是右值&&,统统折叠成左值引用返回）
    
        // 当我们改成防护右值引用后，发现，第一个和第三个返回类型变成了右值引用（第三个发生了引用折叠， && && 折叠成 &&）
        // 可以看到返回右值引用的时候，他实际能得到的类型更全面（所以为什么标准库中std::declval返回右值引用类型）
        
    }
}

namespace _nmsp3
{
    template<typename T>
    T&& myDeclval() noexcept;
    
    
    class ALR
    {
    public:
        void onAnyValue()
        {
            std::cout << "ALR::onAnyValue()成员函数执行了" << std::endl;
        }
        
        void onLvalue()&   // 函数末尾添加一个 & 表示只能被类ALR的左值对象调用（这个就叫做引用限定符）
        {
            std::cout << "ALR::onLvalue()函数被左值调用" << std::endl;
        }
        
        void onRvalue()&&   // 函数末尾添加两个 && 表示只能被类ALR的右值对象调用（这个就叫做引用限定符）
        {
            std::cout << "ALR::onRvalue()函数被右值调用" << std::endl;
        }
        
    };
    
    
    void func()
    {
        ALR alr;    // 左值对象ALR
        alr.onLvalue();
        // ALR::onLvalue()函数被左值调用

        // alr.onRvalue(); // 编译错误，因为onRvalue只能被右值对象调用
        
        
        // 类名加()就是创建临时对象，临时对象就是右值,所以ALR()就是右值对象
        
        ALR().onRvalue();
        // ALR::onRvalue()函数被右值调用
        
        // ALR().onLvalue();  // 编译报错，因为onLvalue()只能被左值对象调用
        
        // decltype(myDeclval<ALR>().onAnyValue());
        
        // decltype(myDeclval<ALR&>().onLvalue());  // myDeclval<ALR&>这种情况返回的是左值引用，左值引用对象调用onLvalue没有问题
        
        // decltype(myDeclval<ALR&&>().onRvalue()); // myDeclval<ALR&&>这种情况返回的是右值引用，右值引用对象调用onRvalue没有问题
        
        // decltype(myDeclval<ALR&>().onRvalue());  // myDeclval<ALR&>这种情况返回的是左值引用，左值引用对象调用onRvalue编译错误
        
        // decltype(myDeclval<ALR&&>().onLvalue()); // myDeclval<ALR&&>这种情况返回的是右值引用，右值引用对象调用onlvalue编译错误
        
        
    }
}

namespace _nmsp4
{
    int myfunc(int a, int b)    // 函数类型一般是由函数返回值和参数类型决定，与函数名没有关系
    {                           // 所以myfunc代表大函数类型是 int(int, int)
        return a+b;
    }
    
    
    // std::declval<U_Args>中的U_Args写成U_Args&或者U_Args&&都一样，不影响
    // decltype(std::declval<T_F>() (std::declval<U_Args>()...))这一串是函数模板的返回值类型推导代码
    // 根据函数类型以及函数参数类型推导出函数模板返回类型 （这里就推断成int类型）
    template<typename T_F, typename... U_Args>
    decltype(std::declval<T_F>() (std::declval<U_Args>()...)) TestFnRtnImpl(T_F func, U_Args... args)
    // decltype(T_F(U_Args)) TestFnRtnImpl(T_F func, U_Args... args)
    // 这种写法报错：因为decltype不是这么用的，decltype后面的()中出现的一般都是变量，对象，表达式，函数名，函数指针名等等
    // 但是不能出现类型名，所以才会报错
    {
        auto rtnvalue = func(args...);
        return rtnvalue;
    }
    
    void func()
    {
        auto result = TestFnRtnImpl(myfunc, 100, 200);
        // 我们在调用TestFnRtnImpl函数模板的时候，没有指定模板参数，参数类型是编译器自行推断出来的
        // 这个时候T_F被推断成函数指针类型 int(*)(int, int)
        // decltype(std::declval<T_F>() (std::declval<U_Args>()...)) 最终推断成int类型
        // 也就是该函数模板在该行代码的调用下的返回类型
        // std::declval<T_F>() 因为T_F是int(*)(int, int)，放进去返回来的就是int(*&&)(int, int)
        // 函数指针的右值引用类型，（其实这里只需要简单理解成函数指针类型）
        // 比如：
        int (*fp_var)(int x, int y);    // 函数指针类型，fp_var = int(*)(int, int)
        int (*&& yy_fp_var)(int x, int y) = std::move(fp_var);  // 函数指针的右值引用类型， yy_fp_var = int(*&&)(int, int)
        // 那现在这个yy_fp_var就可以代表fp_var了，
        // 所以这就是为什么说 函数指针的右值引用类型，（其实这里只需要简单理解成函数指针类型）
        
        fp_var = myfunc;
        
        std::cout << fp_var(1,2) << std::endl;// 3
        
        std::cout << yy_fp_var(1,2) << std::endl;// 3
        
        
        // 现在看完了前半部分，现在来看后半部分【(std::declval<U_Args>()...)】
        // decltype(std::declval<U_Args>()...)这种写法，推导出来的是两个int && (也就是上面我们给定的100， 200这两个实参)
        
        std::cout << result << std::endl;
        // 300
        
        // 当然如果我们想显式的指定模板函数的第一个参数类型，可以这样写
        auto result2 = TestFnRtnImpl<int(int, int)>(myfunc, 100, 300);
        cout << result2 << endl;
    }
}

namespace _nmsp5
{
    // 当然，还有另外一种写法，使用返回类型后置语法，也可以实现相同的功能
    template<typename T_F, typename... U_Args>
    auto TestFnRtnImpl(T_F func, U_Args... args)->decltype(func(args...))
    // decltype(func(args...)) 在下面调用的场景下，能够推断出来的返回类型是int类型
    // 如果不加参数，改为decltype(func)，推断出来的就是int(*)类型
    // 所以，这也就是为什么说 【函数类型一般是由函数返回值和参数类型决定】
    // 你带与不带参数进行类型推断，是有本质区别的
    {
        auto rtnvalue = func(args...);
        return rtnvalue;
    }
    
    int myfunc(int a, int b)
    {
        return a+b;
    }
    
    void func()
    {
        auto result = TestFnRtnImpl(myfunc, 99, 80);
        
        std::cout << result << std::endl;
        // 179
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

