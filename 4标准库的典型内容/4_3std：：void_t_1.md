# std::void_t

## 源码分析和常规范例

c++17中引入了一个别名模板 std::void_t  

```c++
// 源码：
template<class... _Types>
using void_t = void;
```

从源码来看，无论你给定的是什么类型，给定多少个类型参数，最后都生成一个void的别名 void_t

虽然代码比较简单，但是他却能够检测到应用SFINAE（替换失败并不是一个错误）特性时出现的非法类型

换句话说，给进去的类型必须是一个有效类型，不能是一个非法类型

### 判断类中是否存在某个类型别名

```c++
namespace _nmsp2
{
    // 判断类中是否存在某个类型别名
    
    struct NoInnerType
    {
        int m_i;
    };
    
    struct HaveInnerType
    {
        using type = int;   // 类型别名 ，叫 type
        void myfunc() {}
    };
    
    // 、泛化版本
    template<typename T, typename U = std::void_t<>>
    struct HasTypeMen : std::false_type     // struct 默认是public继承，所以这里可以不写public
    {};
    // 可以看到泛化版本的HasTypeMen他的父类是fale_type，同时泛化版本他有两个模板参数，
    // 同时第二个模板参数U他是有默认值的 std::void_t<>
    
    
    // 特化版本
    template<typename T>
    struct HasTypeMen<T, std::void_t<typename T::type>> : std::true_type
    {
        
    };
    // 特化版本的父类是true_type，同时注意特化版本的类模板的特化<>里的参数类型
    // 这个类模板就是通过这里来判断一个类中是否有类型别名 type 的
    // 其实判断有没有类型别名，从代码中就可以看出来，就是在判断T这个类型中有没有type这个成员存在 T::type
    
    // 当然我们现在写的这个测试代码，只适合用于判断 public 访问修饰符下的 类型别名判断，如果改为private那就判断不到了
    
    // 同时因为我们这个HasTypeMen的第二个模板参数中死提供了缺省的默认参数的，所以在实际调用中，只需要给定第一个类型参数即可
    
    
    
    void func()
    {
        // 判断类中是否有一个叫 type 的类型别名（0表示没有， 1表示有）
        
        std::cout << "HasTypeMen<NoInnerType>::value = " << HasTypeMen<NoInnerType>::value << std::endl;
        // HasTypeMen<NoInnerType>::value = 0
        
        
        std::cout << "HasTypeMen<HaveInnerType>::value = " << HasTypeMen<HaveInnerType>::value << std::endl;
        // HasTypeMen<HaveInnerType>::value = 1
        
        // 、注意这里这个::value是父类 true_type 或者 false_type 中定义的一个静态常量（1 和 0）
    }
}
```

当然我们当前这个判断类型别名的类模板有一定局限性，因为类型名是写死的（type），如果要判断的类型名不叫（type）

那么就不适用了。这种情况可以使用 宏 来进行解决

```c++
namespace _nmsp3
{
    // 判断类中是否存在某个类型别名
    
    struct NoInnerType
    {
        int m_i;
    };
    
    struct HaveInnerType
    {
        using type = int;   // 类型别名 ，叫 type
        void myfunc() {}
    };
    

    // 当然我们当前这个判断类型别名的类模板有一定局限性，因为类型名是写死的（type），如果要判断的类型名不叫（type）
    // 那么就不适用了。这种情况可以使用 宏 来进行解决
    
    
    // 注意：该宏定义是在定义类模板，类模板的定义显然在这里不能放在main主函数中
    // 带参数的宏定义   \ 表示下一行接着本行来，是本行的一部分
    // ## 表示字符串连接使用 (他其实就是把你传递进来的字符串 parMTpNm 作为一个宏将它和HTM_连接起来作为一个字符串（这里就是作为一个类名）)
    #define _HAS_TYPE_MEM_(parMTpNm)    \
    template<typename T, typename U = std::void_t<>>    \
    struct HTM_##parMTpNm:std::false_type {};   \
    template<typename T>    \
    struct HTM_##parMTpNm<T, std::void_t<typename T::parMTpNm>>:std::true_type {};
    
    
    // 下面这两行就是在使用宏
    _HAS_TYPE_MEM_(type);
    // 这行作用就是定义了一个判断type类型别名是否存在的类模板
    
    
    _HAS_TYPE_MEM_(size_type);
    // 这行展开之后就相当于定义了一个判断size_type类型别名是否存在的类模板
    
    void func()
    {
        // 判断类中是否有一个叫 type 的类型别名（0表示没有， 1表示有）
        
        std::cout << "HTM_type<NoInnerType>::value = " << HTM_type<NoInnerType>::value << std::endl;
        // HTM_type<NoInnerType>::value = 0
        std::cout << "HTM_type<HaveInnerType>::value = " << HTM_type<HaveInnerType>::value << std::endl;
        // HTM_type<HaveInnerType>::value = 1
        
        std::cout << "HTM_size_type<NoInnerType>::value = " << HTM_size_type<NoInnerType>::value << std::endl;
        // HTM_size_type<NoInnerType>::value = 0
        std::cout << "HTM_size_type<HaveInnerType>::value = " << HTM_size_type<HaveInnerType>::value << std::endl;
        // HTM_size_type<HaveInnerType>::value = 0
        
        
    }
}
```



### 判断类中是否存在某个成员变量

```c++
namespace _nmsp4
{
    // 判断某个类中是否包含某个成员变量
    
    struct NoInnerType
    {
        int m_i;
    };
    
    struct HaveInnerType
    {
        using type = int;   // 类型别名
        void myfunc() {}
    };
    
    // 泛化版本：
    template<typename T, typename U = std::void_t<>>
    struct HasMember : std::false_type
    {
        
    };
    
    // 特化版本：
    template<typename T>
    struct HasMember<T, std::void_t<decltype(T::m_i)>>:std::true_type
    {
        
    };
    
    // 同样，和上面一样，这里也可以使用宏定义的手段，然这个类模板更加实用
    
    void func()
    {
        std::cout << "HasMember<NoInnerType>::value = " << HasMember<NoInnerType>::value << std::endl;
        // HasMember<NoInnerType>::value = 1
        
        // 当遇到这些代码行的时候，编译器会首先看特化版本是否满足需求，因为特化版本中，确实有一个m_i的成员变量
        // 所以特化版本中std::void_t<decltype(T::m_i)>这个写法就是有效合法的，所以编译器就选择这个特化版本
        // 选择特化版本的时候，整个 std::void_t<decltype(T::m_i)> 就被推断为void
        // 然后该类模板实例化出来应该是 HasMember<NoInnerType, void>这个类型，因为该类型的父类是true_type，所以他的::value = 1
        // 注意这里加强一下void_t的理解，不管你给进去的是啥，void_t出现的地方都统统用void来进行替换理解
        
        std::cout << "HasMember<HaveInnerType>::value = " << HasMember<HaveInnerType>::value << std::endl;
        // HasMember<HaveInnerType>::value = 0
        // 
        // 当遇到这行代码的时候，编译器会先去看特化版本是否满足，因为haveInnerType这个类中没有一个叫做 m_i的成员，
        // 所以特化版本中的这个写法 std::void_t<decltype(T::m_i)> 就不合法（无效），根据SFINAE特性，编译器就不会选择这个版本
        // 那么这个特化版本就被忽略，进而选择泛化版本，此情此景下实例化出来的类就应该是HasMember<HaveInnerType,void>
        // 因为泛化版本继承自std::false_type，他的::value = 0
    }
}
```



### 判断类中是否存在某个成员函数

```c++
namespace _nmsp5
{
    // 判断某个类中是否包含某个成员函数
    
    struct NoInnerType
    {
        int m_i;
    };
    
    struct HaveInnerType
    {
        using type = int;   // 类型别名
        void myfunc() {}
    };
    
    // 泛化版本：
    template<typename T, typename U = std::void_t<>>
    struct HasMemFunc : std::false_type
    {
        
    };
    
    // 特化版本：
    // std::declval<T>()从语义上来讲，这个是需要创建一个临时的T对象，实际上并没有创建
    // 因为创建了临时对象，所以这里才能直接.调用成员函数
    template<typename T>
    struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>>:std::true_type
    {
        
    };
    
    // 同样，和上面一样，这里也可以使用宏定义的手段，然这个类模板更加实用
    
    void func()
    {
        std::cout << "HasMemFunc<NoInnerType>::value = " << HasMemFunc<NoInnerType>::value << std::endl;
        // HasMemFunc<NoInnerType>::value = 0
        
        std::cout << "HasMemFunc<HaveInnerType>::value = " << HasMemFunc<HaveInnerType>::value << std::endl;
        // HasMemFunc<HaveInnerType>::value = 1
        
    }
}
```



## 泛化版本和特化版本到底编译器如何选择

```c++
namespace _nmsp6
{
    // 泛化版本和特化版本编译器到底如何选择？
    
    // 编译器有一个很复杂的排序规则来决定使用特化版本还是泛化版本
    // 一般来说，void跟其他任何类型相比，都是最不受待见的，只有最后没招了才会考虑使用void
    
    struct NoInnerType
    {
        int m_i;
    };
    
    struct HaveInnerType
    {
        using type = int;   // 类型别名
        void myfunc() {}
    };
    
    // 泛化版本：
    // template<typename T, typename U = std::void_t<>>
    // 有人说，既然这里std::void_t返回都是一个类型，那这里我随便给一个int类型，是不是也能起到一样的效果呢？
    // 不用 std::void_t<>
    // template<typename T, typename U = int>
    // 如果这里直接使用void,那么两个推断都是void，就会正常选择特化版本，那么最终判断结果就正确
    // 那既然这里用void也可以正确判断，为什么要用std::void呢？
    // 主要还是因为想和特化版本保持一致，特化版本中因为智能使用std::void_t，所以为了保持一致毛豆使用std::void_t
    template<typename T, typename U = void>
    struct HasMemFunc : std::false_type
    {
        
    };
    
    // 特化版本：
    // std::declval<T>()从语义上来讲，这个是需要创建一个临时的T对象，实际上并没有创建
    // 因为创建了临时对象，所以这里才能直接.调用成员函数
    template<typename T>
    struct HasMemFunc<T, std::void_t<decltype(std::declval<T>().myfunc())>>:std::true_type
    {
        
    };
    
    // 同样，和上面一样，这里也可以使用宏定义的手段，然这个类模板更加实用
    
    void func()
    {
        // 如果不用 std::void_t<>，那么这里就不能正常正确判断是否存在成员函数（成员变量判断也是一样的）
        // 其实主要你这样一改之后，本来应该选择特化版本的类模板，结果全跑去选择泛化版本了，就达不到筛选判断的效果
        
        std::cout << "HasMemFunc<NoInnerType>::value = " << HasMemFunc<NoInnerType>::value << std::endl;
        // HasMemFunc<NoInnerType>::value = 0
        // 改为void之后
        // HasMemFunc<NoInnerType>::value = 0
        
        std::cout << "HasMemFunc<HaveInnerType>::value = " << HasMemFunc<HaveInnerType>::value << std::endl;
        // HasMemFunc<HaveInnerType>::value = 0
        // 改为void之后        （判断正确了）
        // HasMemFunc<HaveInnerType>::value = 1
    }
}
```

