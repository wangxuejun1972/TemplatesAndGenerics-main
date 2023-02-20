# std::void_t

## 借助void_t和declval实现is_copy_assignable

is_copy_assignable是c++标准库中的一个类模板，他的作用是用来判断一个类对象是否可以用来进行拷贝赋值

先通过一个简单的范例来认识一下：

```c++
namespace _nmsp1
{
    // 借助void_t和declval实现类似is_copy_assignable
    // is_copy_assignable是c++标准库中的一个类模板，他的作用是用来判断一个类对象是否可以用来进行拷贝赋值
    
    class ACPABL
    {};
    
    class BCPABL
    {
    public:
    // 拷贝赋值运算符
        BCPABL &operator= (const BCPABL& th)
        {
            return *this;
        }
    };
    
    // 如果我们将一个类的拷贝赋值运算符设置为私有或者delete的话，那就不能进行拷贝赋值操作了
    class CCPABL
    {
    public:
    // 拷贝赋值运算符
        CCPABL &operator= (const CCPABL& th) = delete;  // 标记为delete，表示不允许使用拷贝赋值运算
    };
    
    
    void func()
    {
        ACPABL aobj1;
        ACPABL aobj2;
        aobj1 = aobj2;
        
        BCPABL bobj1;
        BCPABL bobj2;
        bobj1 = bobj2;
        
        CCPABL cobj1;
        CCPABL cobj2;
        // cobj1 = cobj2;
        // use of deleted function ‘_nmsp1::CCPABL& _nmsp1::CCPABL::operator=(const _nmsp1::CCPABL&)’
        
        std::cout << "int 跟否拷贝赋值 ？ " << std::is_copy_assignable<int>::value << std::endl;
        // int 跟否拷贝赋值 ？ 1
        
        std::cout << "ACPABL 跟否拷贝赋值 ？ " << std::is_copy_assignable<ACPABL>::value << std::endl;
        // ACPABL 跟否拷贝赋值 ？ 1
        
        std::cout << "BCPABL 跟否拷贝赋值 ？ " << std::is_copy_assignable<BCPABL>::value << std::endl;
        // BCPABL 跟否拷贝赋值 ？ 1
        
        std::cout << "CCPABL 跟否拷贝赋值 ？ " << std::is_copy_assignable<CCPABL>::value << std::endl;
        // CCPABL 跟否拷贝赋值 ？ 0
    }
}
```



IsCopyAssignable这个特化版本，是用来测试类对象之间是否能进行拷贝赋值运算

```c++
namespace _nmsp2
{
    // 借助void_t和declval实现类似is_copy_assignable
    // is_copy_assignable是c++标准库中的一个类模板，他的作用是用来判断一个类对象是否可以用来进行拷贝赋值
    
    class ACPABL
    {};
    
    class BCPABL
    {
    public:
    // 拷贝赋值运算符
        BCPABL &operator= (const BCPABL& th)
        {
            return *this;
        }
    };
    
    // 如果我们将一个类的拷贝赋值运算符设置为私有或者delete的话，那就不能进行拷贝赋值操作了
    class CCPABL
    {
    public:
    // 拷贝赋值运算符
        CCPABL &operator= (const CCPABL& th) = delete;  // 标记为delete，表示不允许使用拷贝赋值运算
    };
    
    
    // 自己实现：IsCopyAssignable类模板
    // 泛化版本
    template<typename T, typename U = std::void_t<>>
    struct IsCopyAssignable : public std::false_type
    {
        
    };
    
    // 特化版本
    template<typename T>
    struct IsCopyAssignable<T, std::void_t<decltype(std::declval<T&>() = std::declval<const T&>())>> : public std::true_type
    {
        
    };
    // IsCopyAssignable这个特化版本，是用来测试类对象之间是否能进行拷贝赋值运算
    // 而类对象本身，他是一个左值，
    // std::declval<T&>() = std::declval<const T&>()
    // 那么对于这个对象，赋值符号左边，std::declval<T&>(),本身我们知道，std::declval<>这个返回的是一个右值引用
    // 但是如果你给类型参数T加一个&，根据引用折叠，这里返回的就是T类型的左值引用，
    // 其实我们可以看看类中拷贝赋值运算符的书写规则
    // CCPABL &operator= (const CCPABL& th)
    // 他不就是返回的就是T& 这种左值引用类型吗？std::decltype既然想要模拟拷贝赋值运算符，那么拷贝赋值运算符返回的是什么类型
    // decltype里面就得返回什么类型（注意这里左边的&不能去掉，不然就返回右值引用类型了）
    // 那么赋值符号右边这个 std::declval<const T&>()
    // 比对一下拷贝赋值运算符书写规则里，左边是一个const T&类型，对吧，那么我们这里 = 右边也就需要实现相同的返回类型
    // 在对象模型中学过：a = b 相当于调用了 a.operator=(b);
    // 所以这里 std::decltype里就相当于 std::declval<T&>().operator=(std::declval<const T&>())
    // 所以 【decltype(std::declval<T&>() = std::declval<const T&>())】这一段返回的（推断出来的类型）就是 T& 即 (class T&)
    
    void func()
    {
        ACPABL aobj1;
        ACPABL aobj2;
        aobj1 = aobj2;
        
        BCPABL bobj1;
        BCPABL bobj2;
        bobj1 = bobj2;
        
        CCPABL cobj1;
        CCPABL cobj2;
        // cobj1 = cobj2;
        // use of deleted function ‘_nmsp1::CCPABL& _nmsp1::CCPABL::operator=(const _nmsp1::CCPABL&)’
        
        std::cout << "int 跟否拷贝赋值 ？ " << IsCopyAssignable<int>::value << std::endl;
        // int 跟否拷贝赋值 ？ 1
        
        std::cout << "ACPABL 跟否拷贝赋值 ？ " << IsCopyAssignable<ACPABL>::value << std::endl;
        // ACPABL 跟否拷贝赋值 ？ 1
        
        std::cout << "BCPABL 跟否拷贝赋值 ？ " << IsCopyAssignable<BCPABL>::value << std::endl;
        // BCPABL 跟否拷贝赋值 ？ 1
        
        std::cout << "CCPABL 跟否拷贝赋值 ？ " << IsCopyAssignable<CCPABL>::value << std::endl;
        // CCPABL 跟否拷贝赋值 ？ 0
    }
}
```



## 综合范例

综合范例：
    有两个vector容器，元素数量相同（10），但是这两个容器的元素类型不同，比如第一个装int, 第二个装double。需求是希望重载一下 + 运算符：做一下这两个容器的加法运算，需要实现第一个容器的第一元素与第第二个容器的第一个元素相加。 以此类推（一一对应相加）

```c++
template<typename T, typename U>
std::vector<T> operator+(std::vector<T> const& vecl1, std::vector<U> const& vecl2);
```

两个容器元素相加，重载了加法运算符

写这个代码的时候，我们遇到一个难题，这个operator+他的返回类型到底是什么，或者说的更确切一点就是，所返回的vector中元素类型到底应该是一个什么类型

所以基于这种情景，我们考虑设计一个类模板 VecAddResult，该类模板我们想的是传递进去容器元素类型参数，然后类模板把这个结果类型得到

加入容器中的元素类型不是基本类型，而是两个类类型

代码改进：希望通过SFINEA特性检测一下两个对象之间是否支持加法操作。准备引入一个叫做
IfCanAdd类模板，用来检测两个类型之间能否相加

注意：VecAddResult和IfCanAdd这两个类模板之间，有一种SFINEA-frendly(SFINEA友好关系)； 因为 IfCanAdd继承自std::true_type的时候，VecAddResult就定义了type；（即：你有true_type，那么我就有type，你没有true_type，我就没有type）

然后结合我们设计的这个VecAddResult,通过这个非类型模板参数ifCando【也就是IfCanAdd来判断是否能够相加】，如果能够相加，那么就为true，就走泛化版本，里面就会定义一个type，如果判断不能相加，就是false，就走特化版本，就什么也不做
    

如果VecAddResult不定义这个type的话，那么func函数中，vec1 + vec2;相加的时候，因为SFINEA特新的原因，重载了的operator+操作就不会被选中。不会被选中来做加法，因为operator+他的返回类型是std::vector<VecAddResult_t<T, U>>这个类型，而这个类型又是 typename VecAddResult<T, U>::type，这个类型中定义了一个type，如果两个对象能相加就定义了一个type，如果不能就不定义type，

编写这种VecAddResult类模板的时候，有一个原则：
如果要实例化某个类模板（包括间接的实例化operator+,VecAddResult），则实例化时不应该失败（编译错误），换句话说，就是我们写的这种类模板本身不应该出现编译错误，要报错也应该是调用者的错误。

```c++
namespace _nmsp3
{
    // 综合范例：
    // 有两个vector容器，元素数量相同（10），但是这两个容器的元素类型不同，比如第一个装int, 第二个装double
    // 需求是希望重载一下 + 运算符：做一下这两个容器的加法运算，需要实现第一个容器的第一元素与第第二个容器的第一个元素相加
    // 以此类推（一一对应相加）
    
    // template<typename T, typename U>
    // std::vector<T> operator+(std::vector<T> const& vecl1, std::vector<U> const& vecl2);
    // 两个容器元素相加，重载了加法运算符
    // 写这个代码的时候，我们遇到一个难题，这个operator+他的返回类型到底是什么
    // 或者说的更确切一点就是，所返回的vector中元素类型到底应该是一个什么类型
    
    // 所以基于这种情景，我们考虑设计一个类模板 VecAddResult,
    // 该类模板我们想的是传递进去容器元素类型参数，然后类模板把这个结果类型得到
    
    // 加入容器中的元素类型不是基本类型，而是两个类类型
    
    // 代码改进：希望通过SFINEA特性检测一下两个对象之间是否支持加法操作。准备引入一个叫做
    // IfCanAdd类模板，用来检测两个类型之间能否相加
    
    
    // 泛化版本
    // template<typename T, typename U, typename V = std::void_t<>>
    template<typename, typename, typename = std::void_t<>>    // 其实这里的TUV都可以省略，因为没用到
    struct IfCanAdd : public std::false_type
    {
        
    };
    
    // 特化版本
    template<typename T, typename U>
    struct IfCanAdd<T, U, std::void_t<decltype(std::declval<T>() + std::declval<U>())>> : public std::true_type
    {
        
    };
    
    struct elec
    {
        elec(int temv);
        elec operator+(const elec& tj); // 必须重载加法运算符已支持加法操作
        // 类型推导不需要做具体的调用，所以这里只定义不实现
    };
    
    /*
    template<typename T, typename U>
    struct VecAddResult
    {
        // using type = decltype(T() + U());
        // 把T类型和U类型相加的结果类型交给编译器来进行推导
        // 如果类中构造函数需要传递实参进行构造，那么这这种创建临时变量的写法就会出错，提示没有合适的默认构造函数
        // 所以改成下面这种写法
        using type = decltype(std::declval<T>() + std::declval<U>());
        // 然后代码设计到这里，还有一个比较大的不足就是你没办法判断两个对象能否相加
        
        // 所以，我们这里需要对这个类模板进行重新实现，实现一个泛化版本和一个特化版本，并在泛化版本中使用功能我们的IfCanAddJ进行判断
    };
    */
    
    // 泛化版本 VecAddResult
    template<typename T, typename U, bool ifCando = IfCanAdd<T, U>::value>
    struct VecAddResult
    {
        using type = decltype(std::declval<T>() + std::declval<U>());  
    };
    
    // 特化版本VecAddResult
    template<typename T, typename U>
    struct VecAddResult<T, U, false>
    {};
    
    // 注意：VecAddResult和IfCanAdd这两个类模板之间，有一种SFINEA-frendly(SFINEA友好关系)
    // 因为 IfCanAdd继承自std::true_type的时候，VecAddResult就定义了type
    // （即：你有true_type，那么我就有type，你没有true_type，我就没有type）
    // 然后结合我们设计的这个VecAddResult,通过这个非类型模板参数ifCando【也就是IfCanAdd来判断是否能够相加】
    // 如果能够相加，那么就为true，就走泛化版本，里面就会定义一个type，如果判断不能相加，就是false
    // 就走特化版本，就什么也不做
    
    // 如果VecAddResult不定义这个type的话，那么func函数中，vec1 + vec2;相加的时候，因为SFINEA特新的原因
    // 重载了的operator+操作就不会被选中。不会被选中来做加法，因为operator+他的返回类型是
    // std::vector<VecAddResult_t<T, U>> 这个类型，而这个类型又是 typename VecAddResult<T, U>::type;
    // 这个类型中定义了一个type，如果两个对象能相加就定义了一个type，如果不能就不定义type，
    
    // 编写这种VecAddResult类模板的时候，有一个原则：
    // 如果要实例化某个类模板（包括间接的实例化operator+,VecAddResult），则实例化时不应该失败（编译错误）
    
    // 取一个别名模板
    template<typename T, typename U>
    using VecAddResult_t = typename VecAddResult<T, U>::type;
    
    template<typename T, typename U>
    // std::vector<typename VecAddResult<T, U>::type> operator+(std::vector<T> const& vecl1, std::vector<U> const& vecl2)
    std::vector<VecAddResult_t<T, U>> operator+(std::vector<T> const& vecl1, std::vector<U> const& vecl2)
    {
        // std::vector<typename VecAddResult<T, U>::type> tmpvalue;
        std::vector<VecAddResult_t<T, U>> tmpvalue;
        return tmpvalue;
    }
    
    void func()
    {
        
        std::vector<elec> vec1;
        std::vector<elec> vec2;
        
        vec1 + vec2;
    }
    
}
```

