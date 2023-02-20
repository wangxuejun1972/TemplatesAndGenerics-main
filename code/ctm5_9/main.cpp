
#include <stdio.h>
#include <iostream>

using namespace std;

// 将trait类模板作为模板参数
// SumFixedTraits类模板和 funcsum函数模板。这两个模板用来计算数组元素的和值

namespace _nmsp1
{
    // SumFixedTraits类模板
    template<typename T>
    struct SumFixedTraits;
    
    template<>
    struct SumFixedTraits<char>
    {
        using sumT = int;
        static sumT initValue() {return 0; }
    };
    
    template<>
    struct SumFixedTraits<int>
    {
        using sumT = int64_t;
        static sumT initValue() {return 0; }
    };
    
    template<>
    struct SumFixedTraits<double>
    {
        using sumT = double;
        static sumT initValue() {return 0.0; }
    };
    
    // ----------------------------------------
    
    // template<typename T>
    // auto funcsum(const T* begin, const T* end)
    // {
    //     using sumT = typename SumFixedTraits<T>::sumT;
        
    //     sumT sum = SumFixedTraits<T>::initValue();
        
    //     for(;;)
    //     {
    //         sum += (*begin)
    //         if (begin == end)
    //             break;
    //         ++begin;
    //     }
    //     return sum;
    // }
    // 给funcsum增加一个模板参数以进一步funcsum的灵活性
    template<typename T, typename U = SumFixedTraits<T>>
    auto funcsum(const T* begin, const T* end)
    {
        typename U::sumT sum = U::initValue();
        
        for(;;)
        {
            sum += (*begin);
            if (begin == end)
                break;
            ++begin;
        }
        return sum;
    }
    
    void func()
    {
        char mc[] = "abc";
        std::cout << (int)funcsum(&mc[0], &mc[2]) << std::endl;
        // 294
        
        
    }
}

int main()
{
    _nmsp1::func();

    return 0;
}
