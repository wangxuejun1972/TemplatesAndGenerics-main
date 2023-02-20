# std::is_union

```c++
namespace _nmsp1
{
    // std::is_union
    // c++11新标准中出现的类模板
    // 用于判断一个类型是否是一个联合类型（union）,这个类模板中有一个value成员，如果给进去的类型是union类型的话
    // 那么这个value就是true（1），如果不是union的话，该value就是false（0）
    struct A{};
    union B
    {
        int num;
        char type;
    };
    
    template<class T>
    inline constexpr bool is_union_v = std::is_union<T>::value;
    
    
    void func()
    {
        std::cout << std::is_union<A>::value << std::endl;  // 0
        std::cout << std::is_union<B>::value << std::endl;  // 1
        
        // c++17中又引入了变量模板，可以改善这个书写过长的问题
        
        std::cout << is_union_v<A> << std::endl;  // 0
        std::cout << is_union_v<B> << std::endl;  // 1
    }
}
```

# std::is_class

```c++
namespace _nmsp2
{
    // std::is_class
    // c++11新标准中出现的类模板
    // 用于判断一个类型是否是一个类类型（class）,这个类模板中有一个value成员，如果给进去的类型是类类型的话
    // 那么这个value就是true（1），如果不是类类型的话，该value就是false（0）
    struct A{};
    union B
    {
        int num;
        char type;
    };
    
    template<class T>
    inline constexpr bool is_class_v = std::is_class<T>::value;
    
    
    void func()
    {
        std::cout << std::is_class<A>::value << std::endl;  // 1
        std::cout << std::is_class<B>::value << std::endl;  // 0
        
        // c++17中又引入了变量模板，可以改善这个书写过长的问题
        
        std::cout << is_class_v<A> << std::endl;  // 1
        std::cout << is_class_v<B> << std::endl;  // 0
    }
}
```

# std::integral_constant

```c++
namespace _nmsp3
{
    // std::integral_constant
    // c++11新标准中出现的类模板
    // std::integral_constant 是一个用于包装目的的类
    
    // 源码
    // template<class _Ty, _Ty, _Val>
    // struct integral_constant
    // {
    //     static constexpr _Ty value = _Val;
        
    //     using value_type = _Ty;
    //     using type = integral_constant;
        
    //     constexpr operator value_type() const noexcept
    //     {
    //         return value;
    //     }
        
    //     _NODISCARD constexpr value_type operator() () const noexcept
    //     {
    //         return value;    
    //     }
    // };
    
    
    struct A{};
    union B
    {
        int num;
        char type;
    };
    
    
    void func()
    {
        std::cout << std::integral_constant<int, 15>::value << std::endl;  // 15
        std::cout << std::integral_constant<bool, true>::value << std::endl;  // 1
        
        // 单独使用看起来平淡无奇，但是结合std::is_union
        std::integral_constant<bool, !std::is_union<B>::value> myobj;
        std::integral_constant<bool, !std::is_union<A>::value> myobj2;
        
        std::cout << myobj.value << std::endl;
        // 0
        std::cout << myobj2.value << std::endl;
        // 1
        
        // 但是这样判断会显得很啰嗦而且多此一举。明明可以直接使用std::is_union来进行判断
        // 为什么还要多此一举弄一个std::integral_constant包裹呢？
        
        // 这妙就妙在 std::integral_constant 是一个用于包装目的的类
        // 通过 std::integral_constant的包装，把std::is_union<B>::value的值 （0）包装成一个std::integral_constant<bool, !std::is_union<B>::value>类型
        // 包装成这个类型后，就可以在很多需要用到类型的场合（比如函数返回类型）就可以使用这个类型
        // 不要忘记 !std::is_union<B>::value 这个值是在编译的时候就已经确定好了的
    }
}
```

