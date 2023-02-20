萃取（trait）技术与策略（policy）技术

------

# 萃取技术及fixed traits（固定萃取）

萃取技术以及fixed traits (固定萃取)，萃取（trait）技术用于对模板中的各类模板参数进行管理，这种萃取技术往往是通过书写一些类模板进行体现，从表现上来讲，这些trait类模板像是一些中间件，它们夹杂（运行）在不同的功能代码之间，使得这些代码之间的调用，结合更加灵活，trait可以做很多事情，在泛型编程中也有广泛的应用

type traits 类型萃取，类型萃取函数库，实现了很多类型信息的提取，类型变换功能

萃取技术是使用模板与泛型的方式来编写代码，与传统的面向对象编程有一定的区别

fixed traits(固定萃取)主要是用于给进来一种类型，萃取（得到）另外一种类型

## 一个常规范例

```c++
namespace _nmsp1
{
    template<typename T>
    T funcsun(const T* begin, const T* end)
    {
        T sum{};    //  零初始化（T如果是数值型类型，那就被初始化为 0 ， 如果是指针类型就被初始化为nullptr，如果是bool类型，就被初始化成false 0）
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << (int)funcsun(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << (int)funcsun(&navl[0], &navl[2]) << std::endl;
        // -2044967296
        std::cout << (int)funcsun(&navc[0], &navc[2]) << std::endl;
        // 38
        
        // 上面这3行调用编译器会实例化出如下3个函数
        // int funcsun<int>(const int*, const int*)
        // int funcsun<int>(const int*, const int*)
        // char funcsun<char>(const char*, const char*)
        
        
    }
}
```

然后，上面这些执行结果后两行都因为类型问题，导致最后结果溢出，而出现错误，那么如何解决这种结果值溢出的问题？

这里我们可以引入一个新的类型参数U，来传入合适的结果类型作为返回值类型

```c++
namespace _nmsp2
{
    template<typename T, typename U>
    U funcsun(const T* begin, const T* end)
    {
        U sum{};    //  零初始化（T如果是数值型类型，那就被初始化为 0 ， 如果是指针类型就被初始化为nullptr，如果是bool类型，就被初始化成false 0）
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << funcsun<int, int>(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << funcsun<int, int64_t>(&navl[0], &navl[2]) << std::endl;
        // 2250000000
        std::cout << funcsun<char, int>(&navc[0], &navc[2]) << std::endl;
        // 294
        
    }
}
```

通过传递两个类型模板参数，把计算溢出问题解决了，但是这种解决方法并不太方便，因为每次调用这个函数模板的时候，都需要额外指定返回值类型参数，显得麻烦，所以。基于这种考量，这里打算使用fixed traits模板来进行改进

```c++
namespace _nmsp3
{
    // fixed traits 泛化版本
    template<typename T>
    struct SunmFiexTraits;  // 不需要实现代码，因为不需要该版本进行实例化
    
    // 各个版本的固定萃取（fixed traits）
    // 1）给进来char 类型，返回int类型
    template<>
    struct SunmFiexTraits<char>
    {
        using sumT = int;   // 类型别名sumT为int类型，也就是返回类型为int类型
    };
    
    // 2）给进来int类型，返回的是__int64/long long/int64_t
    template<>
    struct SunmFiexTraits<int>
    {
        using sumT = int64_t;   // 类型别名sumT为int64_t类型，也就是返回类型为int64_t类型
    };
    
    // 3）其他给进来的是某个类型，返回的是另外一个类型。。。。。
    // 可以扩展出任意多个特化版本
    
    template<typename T>
    auto funcsun(const T* begin, const T* end)
    {
        using sumT = typename SunmFiexTraits<T>::sumT;
        // 给进来一个类型T，返回一个类型（这就是固定萃取的应用）
        sumT sum{};
    
        for(;;)
        {
            sum += (*begin);
            if(begin == end)
            {
                break;
            }
            ++begin;
        }
        
        return sum;
        
    }
    
    
    void func()
    {
        int navi[] = {10, 100, 1, 1000};
        int navl[] = {100000000, 150000000, 2000000000};
        char navc[] = "abc";    // ascii 码 ： 97， 98， 99
        
        std::cout << funcsun(&navi[0], &navi[3]) << std::endl;
        // 1111
        std::cout << funcsun(&navl[0], &navl[2]) << std::endl;
        // 2250000000
        std::cout << funcsun(&navc[0], &navc[2]) << std::endl;
        // 294
    }
}
```

总结来说：固定萃取类模板的作用就是通过某个类型，得到另外一个类型

## 迭代器范例

迭代器分为5类：

- 1）输出型迭代器 struct output_iterator_tag
- 2）输入型迭代器 struct input_iterator_tag
- 3）前向迭代器 struct forward_iterator_tag : input_iterator_tag
- 4）双向迭代器 struct bidirectional_iterator_tag : forward_iterator_tag
- 5）随机访问迭代器 struct random_access_iterator_tag :bidirectional_iterator_tag

vector容器就是随机访问类型迭代器
list容器-双向迭代器

对于随机访问迭代器，如果想让迭代器位置往后跳5个元素，那么可以直接加一个数字5（+5）就能达到往后跳5个元素的效果

对于双向迭代器，比如list，如果想让迭代器位置往后跳5个元素，那么就需要每次往后跳1个元素，循环5次才能达到跳5个元素的效果

这种类似的操作在不同类型的迭代器中，完全不同。所以正确的区分迭代器种类（类型）就显得十分重要

```c++
namespace _nmsp1
{
    // 输出某个容器对应的迭代器种类
    void _display_category(random_access_iterator_tag mytag)
    {
        std::cout << "random_access_iterator_tag" << std::endl;
    }
    
    void _display_category(bidirectional_iterator_tag mytag)
    {
        std::cout << "bidirectional_iterator_tag" << std::endl;
    }
    
    void _display_category(forward_iterator_tag mytag)
    {
        std::cout << "forward_iterator_tag" << std::endl;
    }
    
    void _display_category(output_iterator_tag mytag)
    {
        std::cout << "output_iterator_tag" << std::endl;
    }
    
    void _display_category(input_iterator_tag mytag)
    {
        std::cout << "input_iterator_tag" << std::endl;
    }
    
    template<typename T>
    void display_category(T iter)
    {
        typename iterator_traits<T>::iterator_category cagy;    // 萃取机
        // 这行代码就用到了c++标准库中写好的固定萃取类模板，通过这个类模板就能将迭代器对应的种类提取出来
        _display_category(cagy);
        
        if (typeid(typename iterator_traits<T>::iterator_category) == typeid(random_access_iterator_tag))
        {
            std::cout << "萃取到随机访问迭代器" << std::endl;
        }
    }
    
    
    void func()
    {
        display_category(vector<int>::iterator());
        // random_access_iterator_tag
        display_category(list<int>::iterator());
        // bidirectional_iterator_tag
        display_category(map<int, int>::iterator());
        // bidirectional_iterator_tag
        
        // random_access_iterator_tag
        // 萃取到随机访问迭代器
        // bidirectional_iterator_tag
        // bidirectional_iterator_tag
        
        // 这里迭代器后面的()表示创建了一个临时的迭代器类型的对象
        // 此时display_category中的T就被推断为 _Vector_iterator 或者 _List_iterator 或者 _Map_iterator类型
        // 此时，display_category中的代码行 typename iterator_traits<T>::iterator_category cagy; 
        // 就等价于 iterator_traits<_Vector_iterator>::iterator_category cagy;
        // 又因为在 固定萃取类模板 （iterator_traits）定义中，有类似如下代码
        // using iterator_category = typename IterT::iterator_category;
        // 因此就等价于
        // _Vector_iterator::iterator_category
        // 因为_Vector_iterator中存在using定义
        // using iterator_category = random_access_iterator_tag;
        // 这样就得到了容器的类型 random_access_iterator_tag
        // 最终typename iterator_traits<T>::iterator_category cagy;
        // 就等价于： random_access_iterator_tag (这里只拿vector容器进行举例)
    }
}
```

如何通过固定萃取技术从容器中萃取出对应的迭代器种类

```c++
class _List_iterator    // list容器中对应的迭代器，是一个类或者一个类模板
{
public:
    using iterator_category = bidirectional_iterator_tag;   // 代表list容器的迭代器属于哪个种类的一个类型别名
};

template<typename T>
class list
{
public:
    using iterator = _List_iterator;
};
```

有了这些代码，就不难想象
list<int>::iterator -----> 对应的就是 -----> _List_iterator::iterator_category
然后不就是 bidirectional_iterator_tag，这样就得到了list容器的迭代器种类

同理。vector 容器的迭代器也是一样的萃取方法

如何找迭代器种类

- 1）在容器中，标示出所属的迭代器种类
- 2）通过 一个 fixed trait（固定萃取）类模板 iterator_traits 来进行类型萃取

```c++
 // 固定萃取模板
    // 这里给进来的 类型模板参数 是一个迭代器（迭代器类型）
    template<typename IterT>
    struct iteratorTraits
    {
        // ----............
        using iterator_category = typename IterT::iterator_category;
        // ,......
    };
```



## 通过容器（数组）类型萃取元素类型范例

### 用GetEleType类模板进行常规实现

```c++
namespace _nmsp1
{
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType;
    
    // vector特化版本
    template<typename T>
    struct GetEleType<std::vector<T>>
    {
        using type = T;
    };
    
    // list特化版本
    template<typename T>
    struct GetEleType<std::list<T>>
    {
        using type = T;
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    void func()
    {
        std::cout << "vector<double> 元素类型 = " << typeid(GetEleType<vector<double>>::type).name() << std::endl;
        // vector<double> 元素类型 = double
        std::cout << "list<string> 元素类型 = " << typeid(GetEleType<list<string>>::type).name() << std::endl;
        // list<string> 元素类型 = std::string
        
        std::cout << "float[24] 元素类型 = " << typeid(GetEleType<float[24]>::type).name() << std::endl;
        // float[24] 元素类型 = float
        std::cout << "float[24] 元素数量 = " << GetEleType<float[24]>::size << std::endl;
        // float[24] 元素��量 = 24
    }
}
```



### 引入函数模板PrintEleType

```c++
namespace _nmsp2
{
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType;
    
    // vector特化版本
    template<typename T>
    struct GetEleType<std::vector<T>>
    {
        using type = T;
    };
    
    // list特化版本
    template<typename T>
    struct GetEleType<std::list<T>>
    {
        using type = T;
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    // 引入函数模板PrintEleType
    template<typename T>
    void PrintEleType(const T& Container)
    {
        std::cout << "容器或者数组的元素类型为 = " << typeid(typename GetEleType<T>::type).name() << std::endl;
    }
    
    void func()
    {
        vector<double> vb;
        list<int> li;
        float sc[34]; 
        PrintEleType(vb);
        // 容器或者数组的元素类型为 = double
        PrintEleType(li);
        // 容器或者数组的��素类型为 = int
        PrintEleType(sc);
        // 容器或者数组的元素类型为 = float
    }
}
```



### GetEleType类模板的改进

```c++
namespace _nmsp3
{
    
    // GetEleType代码改进
    // 通过GetEleType类模板进行常规实践
    // 泛化版本
    template<typename T>
    struct GetEleType
    {
        // 改造泛化版本，实现对容器类型的参数T的元素类型获取
        using type = typename T::value_type;    // 针对容器
    };
    
    // 数组特化版本
    // 需要萃取出元素个数
    // 这里严格来说，还是参数范围上的偏特化，T[size] 相较于 T，显然是参数范围缩小，
    // 所以这里不要看多了一个非类型模板参数，就是参数数量上的偏特化，不是这样的
    template<typename T, std::size_t Size>
    struct GetEleType<T[Size]>
    {
        using type = T;
        static const std::size_t size = Size;
    };
    
    // ---------------------
    // 定义别名模板
    template<typename T>
    using GetEleType_T = typename GetEleType<T>::type;
    
    // ---------------------
    // 引入函数模板PrintEleType
    template<typename T>
    void PrintEleType(const T& Container)
    {
        std::cout << "容器或者数组的元素类型为 = " << typeid(GetEleType_T<T>).name() << std::endl;
    }
    
    void func()
    {
        vector<double> vb;
        list<int> li;
        float sc[34]; 
        PrintEleType(vb);
        // 容器或者数组的元素类型为 = double
        PrintEleType(li);
        // 容器或者数组的��素类型为 = int
        PrintEleType(sc);
        // 容器或者数组的元素类型为 = float
        
        std::cout << "vector<double>的元素类型 = " << typeid(vector<double>::value_type).name() << std::endl;
        // vector<double>的元素类型 = double
        
    }
}
```



## 引用类型的移除和增加

### 引用类型的移除

在C++标准库中，提供了一个 std::remove_reference，这是一个类模板，如果传递进来的模板参数是一个引用类型，那么该类模板会把这个引用部分给去掉

```c++
namespace _nmsp1
{
    // 引用类型的移除
    template<class T1, class T2>
    void print_is_same()
    {
        std::cout << "T1类型为：" << typeid(T1).name() << std::endl;
        std::cout << "T2类型为：" << typeid(T2).name() << std::endl;
        std::cout << "T1类型和T2类型是否相等？ " << std::is_same<T1, T2>() << std::endl;
        // std::is_same<>() 如果输出值为 1 ，表示两个值类型相等，如果输出值为0，表达两个值类型不等
        // is_same是标准库中用于判断两个类型是否相同的类模板
        // std::is_same<T1, T2>() 和 std::is_same<T1, T2>::value这两个写法的效果都是一样的
        // 这里加()就是产生了一个 std::is_same<T1, T2> 这个类型的临时对象，
        // 同时这个()也会调用该类模板的类型装换运算符，把这个对象转换成一个整形值来进行输出
        // 源码里就是对应 下面这段代码
        // constexpr operator value_type() const noexcept
        // {
        //     return value;
        // }
        
    }
    
    void func()
    {
        std::remove_reference<int>::type a;
        std::remove_reference<int&>::type b;
        std::remove_reference<int&&>::type c;
        
        print_is_same<decltype(a),decltype(b)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相�？ 1
        print_is_same<decltype(a),decltype(c)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相等？ 1
        print_is_same<decltype(b),decltype(c)>();
        // T1类型为：int
        // T2类型为：int
        // T1类型和T2类型是否相等？ 1
        
        
        // ------------------------------
        // 在c++14中，std::remove_reference定义了大量别名(比如)
        // template<class T>
        // using remove_reference_t = remove_reference<T>::type;
        
    }
}

```

我们也可以自己实现一个简单的类型移除类模板

```c++
namespace _nmsp2
{
    // 泛化版本
    template<typename T>
    struct RemoveReference
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&>
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&&>
    {
        using type = T;
    };
    
    // ------------------------------
    // 定义别名模板
    template<typename T>
    using RemoveReferenceT = RemoveReference<T>::type;
    
    void func()
    {
        int&& a = 12;
        int b = 32;
        int &c = b;
        
        RemoveReferenceT<decltype(a)> a2 = 90;
        RemoveReferenceT<decltype(c)> c2 = 220;
        
        using boost::typeindex::type_id_with_cvr;
        std::cout << "a2 = " << type_id_with_cvr<decltype(a2)>().pretty_name() << std::endl;
        // int
        std::cout << "c2 = " << type_id_with_cvr<decltype(c2)>().pretty_name() << std::endl;
        // int
    }
}
```



### 引用类型的增加

根据给定的类型来创建一个左值或者右值引用

1. ​    std::add_lvalue_reference类模板（给进来一个类型，用于返回该类型的左值引用类型）
2. ​    std::add_rvalue_reference类模板（给进来一个类型，返回该类型的右值引用类型）
3. ​    std::is_lvalue_reference和std::is_rvalue_reference类模板，用于判断是否是左值引用类型或者右值引用类型

    ```c++
    namespace _nmsp3
    {
        // 引用类型的增加
        // 根据给定的类型来创建一个左值或者右值引用
        // std::add_lvalue_reference类模板（给进来一个类型，用于返回该类型的左值引用类型）
        // std::add_rvalue_reference类模板（给进来一个类型，返回该类型的右值引用类型）
        // std::is_lvalue_reference和std::is_rvalue_reference类模板，用于判断是否是左值引用类型或者右值引用类型
        
        void func()
        {
            int a =13;
            int i = 90;
            std::add_lvalue_reference<decltype(a)>::type b = a;
            std::cout << "b是否是左值引用类型 = " << std::is_lvalue_reference<decltype(b)>::value << std::endl;
            // b是否是左值引用类型 = 1
            
            std::add_rvalue_reference<decltype(a)>::type c = std::move(a);
            std::cout << "c是否是右值引用类型 = " << std::is_lvalue_reference<decltype(c)>::value << std::endl;
            // c是否是右值引用类型 = 0
            
            using btype = std::add_lvalue_reference_t<int>; // _t 表示是一个模板别名
            std::cout << "btype 是否和类型 int& 类型相等？" << std::is_same<int&, btype>() << std::endl;
            // btype 是否和类型 int& 类型相等？1
            
            using ctype = std::add_rvalue_reference_t<int>;
            std::cout << "ctype是否是左值引用类型" << std::is_lvalue_reference<ctype>::value << std::endl;
            // ctype是否是左�引用类型0
            
            std::cout << "ctype是否是右值引用类型" << std::is_rvalue_reference<ctype>::value << std::endl;
            // ctype是否是右值引用类型1
            
            std::add_rvalue_reference_t<int&> ccl = i;
            // ccl类型为int&， 这里用到了引用折叠 & + && = &
            std::add_rvalue_reference_t<int&&> ccr = 63;
            // ccr类型为int&&，这里也用到了引用折叠 && + && = &&
        }
    }
    ```

自己实现一个类似std::add_rvalue_reference和std::add_lvalue_reference的类模板

```c++
namespace _nmsp4
{
    template<typename T>
    struct AddLvalueReference
    {
        using type = T&;  
    };
    template<typename T>
    using AddLvalueReference_t = typename AddLvalueReference<T>::type;
    // 实际上这种写法可以直接简化成, 就不用定义 AddLvalueReference
    // using AddLvalueReference_t = T&;
    
    template<typename T>
    struct AddRvalueReference
    {
        using type = T&&;  
    };
    template<typename T>
    using AddRvalueReference_t = typename AddRvalueReference<T>::type;
    
    
    void func()
    {
        int anew = 100;
        AddLvalueReference_t<decltype(anew)> bnew = anew;   // bnew 的类型 int&
        
        int&& anew2 = 12;
        AddLvalueReference_t<decltype(anew2)> bnew2 = anew; // 引用折叠之后，bnew2的类型int&
    
        
        int ar = 23;
        AddRvalueReference_t<decltype(ar)> dgd = 2424; // dgd 的类型 是 int&&
        
        int&& arr = 34;
        AddRvalueReference_t<decltype(arr)> ggg = 3523; // ggg 的类型是 int&&
        
        int aa = 24;
        int& argv = aa;
        AddRvalueReference_t<decltype(argv)> tttt = aa;
        // 引用折叠之后， tttt的类型变成了 int& 左值引用类型（这与这个类模板的语义不符合，这就是错误的使用导致的后果）
        
    }
}
```



## const修饰符的移除

```c++
namespace _nmsp1
{
    // 这里我们自己实现的这个RemoveConst和标准库里提供的std::remove_const类似
    
    // const修饰符的移除
    template<typename T>
    struct RemoveConst
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveConst<const T>
    {
        using type = T;
    };
    // 根据需要可以增加其他特化版本
    
    template<typename T>
    using RemoveConst_t = typename RemoveConst<T>::type;
    
    void func()
    {
        RemoveConst_t<const int> m = 23;
        m = 3535;
    }
}
```



## 退化（decay）技术

退化（decay）技术，某些类型一旦传递给函数模板，或者通过函数模板来进行某些类型参数的推断，推断出来的类型会产生退化

退化，就是把类型中的一些修饰符给丢弃了（const int -> int），对于const int来说，这就是一种退化

const 修饰符，&， &&修饰符被丢弃，数组类型变成指针类型，函数名变成函数指针等等这些都是类型退化的表现

c++标准库中： std::decay， 该类模板的作用就是能够把一个类型退化掉

```c++
namespace _nmsp2
{
    template<class T>
    void testType(T tmpv)
    {
        std::cout << "---------------------------------begin------------------------------" << std::endl;
        
        using boost::typeindex::type_id_with_cvr;
        
        std::cout << "T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
        
        std::cout << "tmpv = " << type_id_with_cvr<decltype(tmpv)>().pretty_name() << std::endl;
        
        std::cout << "---------------------------------endl-------------------------------" << std::endl;
    }
    
    void testFunc()
    {
        
    }
    
    
    void func()
    {
        const int a = 15;
        const int& b = a;
        const int&& c = 124;
        int arr[5] = {1, 2, 3, 4, 5};
        
        testType(a);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(b);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(c);
        // ---------------------------------begin------------------------------
        // T = int
        // tmpv = int
        // ---------------------------------endl-------------------------------
        
        
        testType(arr);
        // ---------------------------------begin------------------------------
        // T = int*
        // tmpv = int*
        // ---------------------------------endl-------------------------------
        
        testType(testFunc);
        // ---------------------------------begin------------------------------
        // T = void (*)()
        // tmpv = void (*)()
        // ---------------------------------endl-------------------------------
        
        
        // --------------------------------------------------------------------
        
        std::decay<const int&>::type nb = 1214;
        std::cout << "nb的类型为： " << typeid(decltype(nb)).name() << std::endl;
        // nb的类型为: int
    }
}
```

如何实现类似std::decay这样的代码？

需要前置代码 `RemoveConst`和`RemoveReference`

```c++
// 无边界数组
extern int g_array[];

namespace _nmsp3
{
    // const修饰符的移除
    template<typename T>
    struct RemoveConst
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveConst<const T>
    {
        using type = T;
    };
    // 根据需要可以增加其他特化版本
    
    template<typename T>
    using RemoveConst_t = typename RemoveConst<T>::type;
    // --------------------------------------------------------------------
    
    // 泛化版本
    template<typename T>
    struct RemoveReference
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&>
    {
        using type = T;
    };
    
    // 特化版本
    template<typename T>
    struct RemoveReference<T&&>
    {
        using type = T;
    };
    
    // 定义别名模板
    template<typename T>
    using RemoveReference_t = RemoveReference<T>::type;
    // --------------------------------------------------------------------
    
    // 实现std::decay
    
    template<typename T>
    struct RemoveCR:RemoveConst<typename RemoveReference<T>::type> // 继承，然后把const和引用都去除
    {};
    // typename RemoveReference<T>::type
    // 这段代码是用来把类型参数中的引用给去掉，然后执行得到的结果仍然是类型
    // 然后在把去掉了引用类型的类型作为RemoveConst模板的模板参数，放到尖括号中
    // 
    // 然后因为继承关系，RemoveCR继承了RemoveConst,所以，RemoveCR就也有type这么个类型成员
    // 这个type类型成员，就是最初那个去掉了引用，去掉了const修饰符的类型
    // 然后，再通过别名模板简化书写
    
    template<typename T>
    using RemoveCR_t = typename RemoveCR<T>::type;
    
    // --------------------------------------------------------------------
    // 当然，我们也可以直接写别名模板，不用写RemoveCR这个类模板
    // template<typename T>
    // using RemoveCR_tt = RemoveConst_t<typename RemoveReference_t<T>>;
    
    
    // --------------------------------------------------------------------
    // 泛化版本
    template<class T>
    struct Decay:RemoveCR<T>
    {
        // 当前泛化版本的Decay只能做到把const和引用给去掉，这还不够
    };
    
    // 特化版本1：处理数组 把有边界数组变成指针
    // 该特化版本不继承任何父类
    // 而且我们这里的数组是有边界数组（数组是有大小的）
    template<class T, std::size_t Size>
    struct Decay<T[Size]>
    {
        using type = T*;
    };
    
    // 无边界数组：
    // 某个.cpp源文件中定义一个全局数组，int g_array[10];，数组类型是int[10]
    // 然后在另一个文件中需要用到这个全局量（在这个源文件中就变成一个无边界数组了，他的类型就是 int[]）
    // extern int g_array[]; // 外部变量声明
    
    // 特化版本2：处理数组 把无边界数组变成指针
    // 该特化版本不继承任何父类
    // 这个特化版本是针对无边界数组
    template<class T>
    struct Decay<T[]>
    {
        using type = T*;
    };
    
    // --------------------------------------------------------------------
    void testFunc2()
    {
        std::cout << "testFunc2（）执行了" << std::endl;
    }
    // 函数类型：由函数返回值类型和函数参数类型决定，与函数名关系不大
    // 所以，本函数的函数类型 void()
    // 可以利用函数指针指向某种函数类型。若要指向testFunc2，那么函数指针的类型就应该是
    // void(*)()
    // void (*P)() = testFunc2;    // 定义一个函数指针p指向testFunc2
    // p(); 
    // --------------------------------------------------------------------
    
    // 行数名退化为函数指针
    template<typename T, typename... Args>
    struct Decay<T(Args...)>    // 返回类型T，参数类型是一包参数类型Args...
    {
        using type = T(*)(Args...);
    };
    
    // --------------------------------------------------------------------
    // 定义别名模板
    template<typename T>
    using Decay_t = typename Decay<T>::type;
    
    
    
    void func()
    {
        RemoveCR_t<const int &&> rc = 235;  // rc int类型
        rc = 3535;
        
        int ar[2] = {1, 2};
        Decay<decltype(ar)>::type myarray;
        std::cout << "myarray的类型为： " << typeid(decltype(myarray)).name() << std::endl;
        // myarray的类型为： int*
        
        Decay<decltype(g_array)>::type nboerArray;
        std::cout << "nboerArray的类型为： " << typeid(decltype(nboerArray)).name() << std::endl;
        // nboerArray的类型为： int*
        
        // 如果不为函数名退化成函数指针 写一个Decay的特化版本，观察一些测试代码
        // 发现，如果不写退化代码，那么给进去什么类型，返回的就是什么类型，不会退化为函数指针
        Decay<decltype(testFunc2)>::type rfunc;
        // 表面上看,这行代码定义了一个函数类型的变量,但是实际上我们把它理解成函数声明更好
        std::cout << "rfunc 类型为 ：" << typeid(decltype(rfunc)).name() << std::endl;
        // rfunc 类型为 ：void(void)
        // 添加了函数名的退化代码后
        // rfunc 类型为 ：void(*)(void)
        
    }
}
```

