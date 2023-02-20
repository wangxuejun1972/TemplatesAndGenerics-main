/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>

using namespace std;

// 萃取技术中的value traits （值萃取）
// 固定萃取【给进来一个类型，萃取出一个类型】
// 值萃取【给进来一个类型，萃取出一个值】


// 总结数组元素类型初始化initValue的三个方法
// 1）在针对类A特化的SumFixedTraits类模板中，只对initValue进行声明，然后在某个.cpp源文件中，对该声明进行定义初始化
// 2）采用c++17中引入的inline内联变量
// 3）在SumFixedTraits的特化版本中，引入静态成员函数initValue 的方式来解决问题



namespace _nmsp1
{
    // 一些测试类
    class A
    {
    public:
        int m_i;
    public:
        A(int v1, int v2):m_i(v1+v2){};
        
    public:
        // 重载+=运算符
        A& operator+=(const A& tmpa)
        {
            m_i+=tmpa.m_i+m_i;
            return *this;
        }
    };
    
    
    // --------------------------------------------------------------------
    template<typename T>
    struct SumFixedTraits;
    
    template<>
    struct SumFixedTraits<char>
    {
        using type = int;
        
        // 对于char类型，给和值初始值为0（静态常量 编译期间就确定好值了）
        // static const type initValue = 0;
        static type initValue()
        {
            return 0;
        }
    };
    
    template<>
    struct SumFixedTraits<int>
    {
        using type = std::int64_t;
        // static const type initValue = 0;
        static type initValue()
        {
            return 0;
        }
    };
    
    template<>
    struct SumFixedTraits<double>
    {
        using type = double;
        // static const type initValue = 0.0;
        // 对于double类型，如果在初始化给0的时候，这样写，编译会报错
        // 带有类内初始化表达式的静态数据成员，必须具有不可变的常量整形类型，或者必须被指定为“内联”
        // 所以需要改成如下写法
        // static constexpr type initValue = 0.0;
        
        // --------------------------------------------------------------------
        static type initValue()
        {
            return 0.0;
        }
    };
    
    template<>
    struct SumFixedTraits<A>
    {
        using type = A;
        // static cons type initValue {0, 0};
        // 这样初始化会报编译错误：
        // 带有类内初始化表达式的静态数据成员，必须具有不可变的常量整形类型，或者必须被指定为“内联”
        // 说明不能这样进行初始化
        // static const type initValue; // 声明（不是定义）
        
        // --------------------------------------------------------------------
        // 使用inline 内联变量来解决
        // inline static const type initValue {0, 0};
        // --------------------------------------------------------------------
        static type initValue()
        {
            return type{0, 0};
        }
        
    };
    // A const SumFixedTraits<A>::initValue = A{0, 0};
    // 定义初始化
    // 这种定义初始化，在有多个.cpp文件的情况下，不能再多个.cpp源文件中都写这种代码行
    // 会爆链接错误的，所以只能选择某一个.cpp中写这种定义初始化
    
    // 对于类类型的0初始化，这种写法比较啰嗦繁琐，声明和定义得写在两个地方
    // 所以c++17中，引入了一个 inline 内联变量的东西来解决这种问题
    
    template<typename T>
    auto funcSum(const T* begin, const T* end)
    {
        using sumT = typename SumFixedTraits<T>::type;
        // sumT sum{}; // 零初始化
        // 这里我们计算的主要是int和char类型，char类型可以看成是范围较窄的int类型，可以进行零初始化
        // 但是遇到一些数组类型是其他类型，比如说类类型的，那么上面这个零初始化的格式可能就不合适了
        
        // 这里可以利用萃取技术来解决这个问题
        // 这就需要利用值萃取技术对这个零初始化代码进行整改，让其可以应付各种类型，不在局限于int，char型
        // sumT sum = SumFixedTraits<T>::initValue;    // 给进来一个类型T，返回一个值 （这就是值萃取【value traits】）
        
        // --------------------------------------------------------------------
        sumT sum = SumFixedTraits<T>::initValue();
        
        
        for(;;)
        {
            sum+=(*begin);
            if(begin==end)
                break;
            ++begin;
        }
        return sum;
    }
    
    
    void func()
    {
        int myintarray[] = {10, 100, 1, 1000};
        int myintarray2[] = {1000000000, 1200000000, 150000000, 200000000};
        char mychararray[] = "abcd";
        double mydoublearray[] = {12.8, 23.42, 232.44, 32.21};
        A myAarray[] = {A{2, 3}, A{3, 4}, A{4, 5}, A{5, 6}};
        
        std::cout << funcSum(&myintarray[0], &myintarray[3]) << std::endl;
        // 1111
        std::cout << funcSum(&myintarray2[0], &myintarray2[3]) << std::endl;
        // 2550000000
        std::cout << funcSum(&mychararray[0], &mychararray[3]) << std::endl;
        // 394
        std::cout << funcSum(&mydoublearray[0], &mydoublearray[3]) << std::endl;
        // 300.87
        
        std::cout << funcSum(&myAarray[0], &myAarray[3]).m_i << std::endl;
        // 97
    }
}


// 判断是否是void
// c++标准库中有一个类模板 std::is_void用来判断是否是void类型

namespace _nmsp2
{
    // 使用值萃取自己实现一个类似is_void的类模板
    // 泛化版本
    template<typename T>
    struct VoidValueTraits
    {
        static const bool value = false;
    };
    
    // 特化版本
    template<>
    struct VoidValueTraits<void>
    {
        static const bool value = true;
    };
    
    void func()
    {
        std::cout << "int 是void类型吗？" << std::is_void<int>() << std::endl;
        // int 是void类型吗？0
        std::cout << "void 是void类型吗？" << std::is_void<void>() << std::endl;
        // void 是void类型吗？1
        
        std::cout << "int 是void类型吗？" << VoidValueTraits<int>::value << std::endl;
        // int 是void类型吗？0
        std::cout << "void 是void类型吗？" << VoidValueTraits<void>::value << std::endl;
        // void 是void类型吗？1
        
    }
}


// 判断两个类型是否相同
// c++标准库中有一个类模板 std::is_same用来判断两个类型是否相同
namespace _nmsp3
{
    // 使用值萃取实现一个类似is_same的类模板
    // 结合这个true_type和false_type，我们还可以对 SameValueTraits 代码做进一步的简化
    // 因为 true_type和false_type 他里面自带了一个value这么一个静态bool成员
    
    template<typename T, typename F>
    struct SameValueTraits:std::false_type{};
    // struct SameValueTraits
    // {
    //     static const bool value = false;
    // };
    
    template<typename T>
    struct SameValueTraits<T, T>:std::true_type{};
    // struct SameValueTraits<T, T>
    // {
    //     static const bool value = true;
    // };
    
    // --------------------------------------------------------------------
    // 简化这种书写，写一个 变量模板 【注意这里是 变量模板， 不是别名模板，因为我们这里返回的是一个值，而不是类型，所以是变量模板】
    template<typename T, typename F>
    const bool SameValueTraits_v = SameValueTraits<T, F>::value;
    // --------------------------------------------------------------------
    // std::true_type/false_type
    // 结合这个，我们还可以对 SameValueTraits 代码做进一步的简化
    
    void func()
    {
        std::cout << "int 和 const int类型相同吗？" << SameValueTraits<int, const int>::value << std::endl;
        // int 和 const int类型相同吗？0
        std::cout << "int 和 int类型相同吗？" << SameValueTraits<int, int>::value << std::endl;
        // int 和 int类型相同吗？1
        
        // --------------------------------------------------------------------
        
        std::cout << "int 和 const int类型相同吗？" << SameValueTraits_v<int, const int> << std::endl;
        // int 和 const int类型相同吗？0
        std::cout << "int 和 int类型相同吗？" << SameValueTraits_v<int, int> << std::endl;
        // int 和 int类型相同吗？1
        
    }
}

int main()
{
    // _nmsp1::func();
    // _nmsp2::func();
    _nmsp3::func();

    return 0;
}
