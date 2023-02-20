# std::true_type和std::false_type

std::true_type和std::false_type

源码如下

```c++
template<bool _Val>

 using bool_constant = intergral_constant<bool, _Val>;
    

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;
```

从源码来看，true_type和false_type是两个类型（类模板），

注意区分：true_type和false_type与true和false的区别。

true_type,false_type代表的是类型（类类型）而true和false代表的是值

同时注意与bool的区别：bool代表true也可以代表false，但是true_type只能代表true,false_type代表的值只能是false

比如：

```c++
true_type func();   // 代表一个返回类型为true_type的函数
true func();    // ???? 这个就不行，语法错
```



为了演示，这里实现一个类似true_type和false_type的类模板

```c++
namespace _nmsp1
{
    // std::true_type和std::false_type
    // 源码如下
    // template<bool _Val>
    // using bool_constant = intergral_constant<bool, _Val>;
    
    // using true_type = bool_constant<true>;
    // using false_type = bool_constant<false>;
    
    // 从源码来看，true_type和false_type是两个类型（类模板），
    // 注意区分：true_type和false_type与true和false的区别。
    // true_type,false_type代表的是类型（类类型）
    // 而true和false代表的是值
    // 同时注意与bool的区别
    // bool代表true也可以代表false，但是true_type只能代表true,false_type代表的值只能是false
    
    // 比如：
    // true_type func();   // 代表一个返回类型为true_type的函数
    // true func();    // ???? 这个就不行，语法错
    
    
    // 为了演示，这里实现一个类似true_type和false_type的类模板
    template<bool Val>
    struct BoolConstant
    {
        using type = BoolConstant;
        // using type = BoolConstant<Val>; // 或者这种写法也行
        
        static constexpr bool value = Val;
    };
    
    using TrueType = BoolConstant<true>;
    using FalseType = BoolConstant<false>;
    
    // 注意：
    // 1）TrueType和FalseType代表一个类类型，
    // TrueType代表一种true（真）的含义，理解成标准库中的std::true_type
    // FalseType代表一种false(假)的含义，理解成标准库中的std::false_type
    // 2）std::true_type和std::false_type一般都是单做基类来被继承使用，当作为基类被继承时，派生类也就具备了真或者假这种特性
    // 比如标准库中，很多is开头的类模板用于萃取某些信息，萃取的结果要么就是真，要么就是假。
    // is_pointer, is_union, is_function等类模板，他们都是继承自bool_constant<true>或者bool_constant<false>这样的基类来实现
    // 可以当做一种返回类型被使用，比如：
    // FalseType mfunc1();  // 表示mfunc1返回'假'
    // TrueType mfunc2();   // 表示mfunc2返回'真'
    // 千万不要以为返回‘假’可以用return false；来表示，返回‘真’可以用return true;来表示
    // 这种写法在利用萃取技术萃取一些有用信息的时候，有很神奇的效果
    
    // 可能有人会问：从一个bool值true或者false变成一个类类型true_type或者false_type，有什么具体用途呢？
    // 用途非常多。后面会慢慢展示，这里先举一个小的范例：
    // 
    
    template<typename T, bool val>
    class AC
    {
    public:
        AC()
        {
            std::cout << "AC::AC()构造函数执行了" << std::endl;
            // // if (val)
            // if constexpr(val)
            // {
            //     T tmpa = 190;
            //     std::cout << "真" << std::endl;
            // }
            // else
            // {
            //     T tmpa = "abcd";
            //     std::cout << "假" << std::endl;
            // }
            
            AC_Ex(BoolConstant<val>()); // BoolConstant<val>() 创建一个临时对象
            
        }
        
        // void AC_Ex(TrueType abc)
        void AC_Ex(TrueType)    // 当然，因为这个abc形参这里没有使用到，所以这里写不写都可以
        {
            T tmpa = 190;
            std::cout << "真" << std::endl;
        }
        
        // void AC_Ex(FalseType abc)
        void AC_Ex(FalseType)   // 当然，因为这个abc形参这里没有使用到，所以这里写不写都可以
        {
            T tmpa = "abcd";
            std::cout << "假" << std::endl;
        }
    };
    
    
    
    void func()
    {
        AC<int, true> temobj1;
        // AC::AC()构造函数执行了
        // 真
        
        AC<string, false> temobj2;
        // AC::AC()构造函数执行了
        // 假
        
        // 按道理，在我们第二个参数为true的时候，构造函数里应该执行的是true判断里的代码，
        // 第二个参数为false的时候，应该执行的是false里的代码，而且我们传递的类型也都匹配，
        // true的时候int,能进行正确的类型赋值，false的时候string，也能进行正确的类型匹配
        
        // 但是我们编译的时候，发现报错了。虽然编译器能够判断出执行那条分支，但是从编译出代码的角度来讲
        // 不管是if条件分支还是else条件分支，编译器都会去编译，所以，以我们当前这种写法，总有一个分支在编译的时候会出错（类型不匹配）
        // 那这个时候，就可以采用 if constexpr【编译期间if语句】 来进行改进
        
        // 当然这里为了演示true_type和false_type，这里不用这种解决方案，换一种解决方案
    }
}
```



注意：
    1）TrueType和FalseType代表一个类类型，
    TrueType代表一种true（真）的含义，理解成标准库中的std::true_type
    FalseType代表一种false(假)的含义，理解成标准库中的std::false_type
    2）std::true_type和std::false_type一般都是单做基类来被继承使用，当作为基类被继承时，派生类也就具备了真或者假这种特性

​    比如标准库中，很多is开头的类模板用于萃取某些信息，萃取的结果要么就是真，要么就是假。
​    is_pointer, is_union, is_function等类模板，他们都是继承自bool_constant<true>或者bool_constant<false>这样的基类来实现

​    可以当做一种返回类型被使用，比如：

​    

```c++
FalseType mfunc1();  // 表示mfunc1返回'假'
TrueType mfunc2();   // 表示mfunc2返回'真'
```

​    千万不要以为返回‘假’可以用return false；来表示，返回‘真’可以用return true;来表示
​    这种写法在利用萃取技术萃取一些有用信息的时候，有很神奇的效果
​    

可能有人会问：从一个bool值true或者false变成一个类类型true_type或者false_type，有什么具体用途呢？
用途非常多。后面会慢慢展示，这里先举一个小的范例：