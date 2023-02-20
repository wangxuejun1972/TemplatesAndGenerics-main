/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <utility>

using namespace std;

namespace _nmsp1
{
    // 逆向排列数字生成一个类型Integer_Sequence_Reverse
    
    // 向integer_sequence头部插入元素
    
    // 泛化版本
    template<typename INTSEQ, unsigned int NewElem>
    struct InSeq_PushFront;
    
    // 特化版本
    template<typename T, unsigned int... Elems, unsigned int NewElem>
    struct InSeq_PushFront<std::integer_sequence<T, Elems...>, NewElem>
    {
        using type = std::integer_sequence<T, NewElem, Elems...>;
    };
    
    // ----------------------------------------------------------------
    
    // Integer_Sequence_Reverse
    // 泛化版本
    // Count 计数（从1开始）
    template<typename T, unsigned int N, unsigned int Count = 1>
    struct Integer_Sequence_Reverse
    {
        using type = typename InSeq_PushFront<typename Integer_Sequence_Reverse<T, N-1>::type, N-Count>::type;
    };
    
    // 特化版本
    template<typename T, unsigned int N>
    struct Integer_Sequence_Reverse<T, N, N>
    {
        using type = std::integer_sequence<T, N-1>;
    };
    
    // ----------------------------------------------------------------
    
    // 定义别名模板
    template<typename T, unsigned int N>
    using Integer_Sequence_Reverse_T = typename Integer_Sequence_Reverse<T, N>::type;
    
    void func()
    {
        Integer_Sequence_Reverse_T<int, 5> tmobj;
        std::cout << "tmobj类型 = " << typeid(decltype(tmobj)).name() << std::endl;
        // tmobj类型 = std::integer_sequence<int, 4, 3, 2, 1, 0>
        // Integer_Sequence_Reverse_T<int, 5> tmobj; 等价于
        // Integer_Sequence_Reverse<int, 5>::type; // 把::type看成函数调用，走到泛化版本
        // （因为泛化版本默认提供了第三个非类型模板参数1，而这里没有提供那个计数参数，所以比对下来就选择走非类型模板参数，很好理解，等到递归到计数参数和N相等之时，就走特化版本）
        // ----using type = typename InSeq_PushFront<typename Integer_Sequence_Reverse<int, 4>::type, 4>::type;
        // ----Integer_Sequence_Reverse<int, 4>::type 展开如下
        // --------using type = typename InSeq_PushFront<typename Integer_Sequence_Reverse<int, 3>::type, 3>::type;
        // --------Integer_Sequence_Reverse<int, 3>::type 展开如下
        // ------------using type = typename InSeq_PushFront<typename Integer_Sequence_Reverse<int, 2>::type, 2>::type;
        // ------------Integer_Sequence_Reverse<int, 2>::type 展开如下
        // ----------------using type = typename InSeq_PushFront<typename Integer_Sequence_Reverse<int, 1>::type, 1>::type;
        // ----------------Integer_Sequence_Reverse<int, 1>::type 展开如下、这行展开调用的就是特化版本
        // --------------------using type = std::integer_sequence<int, 0>;  // 这行之后，递归结束，开始回溯
        // ----------------using type = std::integer_sequence<int, 1, 0>;
        // ------------using type = std::integer_sequence<int, 2, 1, 0>;
        // --------using type = std::integer_sequence<int, 3, 2, 1, 0>;
        // ----using type = std::integer_sequence<int, 4, 3, 2, 1, 0>;
        // 最终结果就是 
        // using type = std::integer_sequence<int, 4, 3, 2, 1, 0>;
        
        
    }
}

namespace _nmsp2
{
    
    // 将一个数字重复多次生成一个类型 Repeat_Integer
    // 前面是通过递归调用的方式实现，现在这个采用递归继承的方式实现
    // 泛化版本
    template<std::size_t Num, std::size_t RepeatTime, typename INISEQ = std::integer_sequence<std::size_t>>
    class Repeat_Integer;
    
    // 特化版本
    template<std::size_t Num, std::size_t RepeatTime, std::size_t... index>
    class Repeat_Integer<Num, RepeatTime, std::integer_sequence<std::size_t, index...>>
    :public Repeat_Integer<Num, RepeatTime-1, std::integer_sequence<std::size_t, index..., Num>>
    {};
    
    // 特化版本（用于结束递归的特化版本）
    template<std::size_t Num, std::size_t... index>
    class Repeat_Integer<Num, 0, std::integer_sequence<std::size_t, index...>>
    {
    public:
        using type = std::integer_sequence<std::size_t, index...>;
    };
    // ----------------------------------------------------------------
    // 定义别名模板
    template<std::size_t Num, std::size_t RepeatTime>
    using Repeat_Integer_T = typename Repeat_Integer<Num, RepeatTime>::type;
    
    
    void func()
    {
        Repeat_Integer_T<9, 5> tmobj;
        std::cout << "tmobj类型 = " << typeid(decltype(tmobj)).name() << std::endl;
        // tmobj类型 = std::integer_sequence<size_t, 9, 9, 9, 9, 9>
        // 因为Repeat_Integer_T<9, 5> tmobj;存在，所以编译器会根据Repeat_Integer的特化版本实例化出如下类
        // Repeat_Integer<9, 5, std::integer_sequence<std::size_t>>
        // 要想实例化出这个，根据继承关系，就得先有 如下父类
        // Repeat_Integer<9, 4, std::integer_sequence<std::size_t, 9>>
        // 要想实例化出这个，根据继承关系，就得先有 如下父类
        // Repeat_Integer<9, 3, std::integer_sequence<std::size_t, 9, 9>>
        // 要想实例化出这个，根据继承关系，就得先有 如下父类
        // Repeat_Integer<9, 2, std::integer_sequence<std::size_t, 9, 9, 9>>
        // 要想实例化出这个，根据继承关系，就得先有 如下父类
        // Repeat_Integer<9, 1, std::integer_sequence<std::size_t, 9, 9, 9, 9>>
        // 要想实例化出这个，根据继承关系，就得先有 如下父类
        // Repeat_Integer<9, 0, std::integer_sequence<std::size_t, 9, 9, 9, 9, 9>>
        
        Repeat_Integer_T<9, 5, std::integer_sequence<std::size_t, 10, 100, 1000>> tmobj2;
        std::cout << "tmobj类型 = " << typeid(decltype(tmobj2)).name() << std::endl;
        // tmobj2类型 = std::integer_sequence<size_t, 10, 100, 1000, 9, 9, 9, 9, 9>
        // 这里实例化 Repeat_Integer<9, 5, std::integer_sequence<std::size_t 10， 100， 1000>>
        // 实例化的是第一个特化版本，那么非类型可变参模板参数 index... 就是这里的 10， 100， 1000
        // 
    }
}

int main()
{
    // _nmsp1::func();
    _nmsp2::func();

    return 0;
}

