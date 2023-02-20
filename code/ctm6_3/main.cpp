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
    // 泛化版本
    template<int x_v, int y_v>
    struct InstantialtionObServe
    {
        static const int value = (x_v > y_v) ? InstantialtionObServe<x_v - 1, y_v>::value : InstantialtionObServe<x_v, x_v>::value;
    };
    
    // 特化版本
    template<int v>
    struct InstantialtionObServe<v, v>
    {
        static const int value = v;
    };
    
    void func()
    {
        std::cout << InstantialtionObServe<9, 2>::value << std::endl;
        // 2
    }
}

// 元编程时，要小心使用条件运算符（？：）,因为不管条件是否成立，所涉及到的类都可能会被实例化出来

// 那既然条件运算符有这种弊端，有没有什么改进方法呢？
// std::conditional， 他能根据一个布尔值，在两者之间选择一个
// std::conditional比条件运算符（？：）更有优势

namespace _nmsp2
{
    // 泛化版本
    template<int x_v, int y_v>
    struct InstantialtionObServe
    {
        using TmpType = typename std::conditional<(x_v > y_v), InstantialtionObServe<x_v - 1, y_v>, InstantialtionObServe<x_v, x_v>>::type;
        // 注意这段代码只是为类型取一个别名，这段代码并不会导致c++编译器实例化出任何代码
        
        static const int value = TmpType::value;
        // 正真导致实例化的是这行代码，因为这行代码用到了上面定义的别名
    };
    
    // 特化版本
    template<int v>
    struct InstantialtionObServe<v, v>
    {
        static const int value = v;
    };
    
    void func()
    {
        std::cout << InstantialtionObServe<900, 2>::value << std::endl;
        // 2
        // 而且看到，这种写法，能支持的判断范围更大，上面的写法中因为不管你条件成立与否，
        // 都会进行实例化，这就导致编译器因为实例化限制（递归深度限制），能支持到的实例化不是很多，
        // 但是这种写法只会去实例化条件成立的，所以能支持的判断数肉眼可见的提升
    }
}

// 当然这种写法，因为静态成员变量时占据内存的，如果不希望静态成员变量的存在导致额外占用内存空间的话，
// 这里还可以进一步改进，改为使用枚举类型，枚举类型一般代表的都是整形，（当然这里如果是其他类型，那这里可能没有办法用枚举类型来进行优化取代）


namespace _nmsp3
{
    // 泛化版本
    template<int x_v, int y_v>
    struct InstantialtionObServe
    {
        using TmpType = typename std::conditional<(x_v > y_v), InstantialtionObServe<x_v - 1, y_v>, InstantialtionObServe<x_v, x_v>>::type;
        // 注意这段代码只是为类型取一个别名，这段代码并不会导致c++编译器实例化出任何代码
        
        // 改成使用枚举
        enum
        {
            value = TmpType::value
        };
    };
    
    // 特化版本
    template<int v>
    struct InstantialtionObServe<v, v>
    {
        // 改成使用枚举
        enum
        {
            value = v
        };
    };
    
    void func()
    {
        std::cout << InstantialtionObServe<900, 2>::value << std::endl;
        // 2
    }
}

// 同时观察到，在使用新版本编译器（比如vs2019及其以上的时候），编译器对于这种类内的枚举，
// 已经不需要在目标文件中产生任何的与InstantialtionObServe实例化有关的代码

// 元编程计算完整性（也就是怎么才能算是一个完整的元编程）
// 1）状态变量：指的是比如，InstantialtionObServe 类模板中的模板参数
// 2）迭代构造（循环构造）：通过在InstantialtionObServe模板中融入递归编程技术
// 3）执行路径选择：使用条件运算符(?:)或者使用std::conditional,以及类模板特化结合，当然也可以调用其他元函数
// 4）整数对象（整数运算）：静态成员变量value（或者枚举类型），其他功能就等价于变量（常量）的功能

// 元编程的代码实现其实和普通的c++代码实现是很不一样的，比如
// 在元编程中不能使用变量（因为元编程发生在编译期间，编译期间只能接受静态常量）
// 而对于传统意义上的条件分支和循环，是通过条件运算符，特化，std::conditional，递归等手段去进行实现
// 所以这种元编程风格常常也被称之为函数式编程


// inline静态成员变量
// inline这个关键字，一般用于修饰函数，主要作用就是建议编译器将函数调用代码替换成函数本体，来提高程序执行效率
// 对于一个项目来说，类中的静态成员变量一般都是在一个.h头文件中进行声明，然后在某一个.cpp源文件中进行定义（分配内存）
// 对于类中的静态成员变量的处理，在c++11新标准推出之后，语法上就支持了静态成员变量的类内初始化


// 当然这种语法目前是有一定限制的，首先mystatic他的类型必须得是int类型（或者是int 相关的类型 【char, unsigned, int , sort】）
// 其次mystatic必须用const来修饰
// 当然这种类内静态成员变量初始化也是有代价的，
// 编译器会给该类以及该静态成员变量分配内存空间，即使你在项目中从来没有用到过该类，他也会分配出内存
namespace _nmsp4
{
    struct Tst
    {
        static const int mystatic = 1;
    };
}

// 当然，在引入inline之后，这种类内的静态成员变量初始化，就不在局限于int相关的类型了
// 而且也不需要强制使用const修饰，同时这样写了之后，如果你在项目中没有使用过该类，
// 那么编译器是不会为该内分配内存，也不会为该静态成员变量分配内存
// （当然，如果这个静态成员变量时类类型的话，情况可能又会有所不同，类类型编译器可能会给其分配内存空间）

namespace _nmsp5
{
    struct Tst
    {
        static inline double mystatic = 12.33;
    };
}
// constexpr 从c++17开始，就自带inline属性了，所以我们这里改成使用constexpr来修饰也是可以的
// 当然因为 constexpr 带有常量属性，所以这种初始化之后， 该静态成员变量的值也就不能被修改了
namespace _nmsp6
{
    struct Tst
    {
        static constexpr double mystatic = 12.33;
    };
}

// static inline const double mystatic = 12.33;
// static constexpr double mystatic = 12.33;
// 这两种写法，在程序员眼中可能没有区别，但是在编译器看来，这两种写法是不等价的
// 因为，constexpr 第二种写法，在编译之后，编译器会为其分配内存空间，也就是说，假如你把这个类定义在一个头文件中
// 凡是include包含过这个类定义头文件的.cpp生成的目标文件(.obj)中都能看到为这个类和这个静态成员变量分配内存空间
// 即便是你这些.cpp源代码中你从来没有用过这些类

// 说了这么多，其实我们上面不使用枚举，使用inline其实也很不错
namespace _nmsp7
{
    // 泛化版本
    template<int x_v, int y_v>
    struct InstantialtionObServe
    {
        using TmpType = typename std::conditional<(x_v > y_v), InstantialtionObServe<x_v - 1, y_v>, InstantialtionObServe<x_v, x_v>>::type;
        // 注意这段代码只是为类型取一个别名，这段代码并不会导致c++编译器实例化出任何代码
        
        // 改成使用inline
        static inline const int value = TmpType::value;
    };
    
    // 特化版本
    template<int v>
    struct InstantialtionObServe<v, v>
    {
        // 改成使用inine
        static inline const int value = v;
    };
    
    void func()
    {
        std::cout << InstantialtionObServe<900, 2>::value << std::endl;
        // 2
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
