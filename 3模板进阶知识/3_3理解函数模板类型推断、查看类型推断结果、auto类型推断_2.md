# 理解函数模板类型推断

## 万能引用类型

```c++
namespace _nmsp3
{
    
    // 函数模板
    template<typename T>
    void getTypeByBoost(T&& tmprv) // 万能引用类型
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
    }
    
    void func()
    {
        int i = 100;        // i的类型int
        const int j = i;    // j的类型const int
        const int &k = i;   // k的类型const int&
        
        getTypeByBoost(i);
        // ------------------------------begin----------------------------
        //  T 类型 = int&
        //  tmprv 类型 = int&
        
        
        getTypeByBoost(j);
        // ------------------------------endl-----------------------------
        // ------------------------------begin----------------------------
        //  T 类型 = int const&
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        
        getTypeByBoost(k);
        // ------------------------------begin----------------------------
        //  T 类型 = int const&
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        getTypeByBoost(120);
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int &&
        // ------------------------------endl-----------------------------
        
    }

}
```



## 传值方式

如果实参是引用类型，则引用部分会被忽略，T不会被推导为引用类型，除非手工指定为引用类型
getTypeByBoost<int&>(k); // 类似这种）（不建议这样写代码）
若实参是const类型，则const会被忽略，T不会被推导为const类型（毕竟传值方式产生的是新副本，原来的const属性不重要）
    

```c++
namespace _nmsp4
{
    
    // 函数模板
    template<typename T>
    void getTypeByBoost(T tmprv) // 传值方式
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        
        
    }
    
    void func()
    {
        int i = 100;        // i的类型int
        const int j = i;    // j的类型const int
        const int &k = i;   // k的类型const int&
        
        getTypeByBoost(i);
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int
        
        
        getTypeByBoost(j);
        // ------------------------------endl-----------------------------
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int
        // ------------------------------endl-----------------------------
        
        
        getTypeByBoost(k);
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int
        // ------------------------------endl-----------------------------
        
        getTypeByBoost(120);
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int
        // ------------------------------endl-----------------------------
        
        
        // 手动指定（不建议这样写代码）
        int& m = i;
        getTypeByBoost<int&>(m);
        // ------------------------------begin----------------------------
        //  T 类型 = int&
        //  tmprv 类型 = int&
        // ------------------------------endl-----------------------------
        
        // 传递指针进去
        char mystr[] = "LiKe";
        const char* const p = mystr;    // 第一个const表示p指向的目标中的内容不能通过p改变，第二个cosnt表示p指向mystr以后，不能再指向其他内容（p的指向不能变）
        getTypeByBoost(p);
        // ------------------------------begin----------------------------
        //  T 类型 = char const*
        //  tmprv 类型 = char const*      
        // ------------------------------endl-----------------------------
        // 传递给函数模板后，第二个const没有了，第一个const被保留
        // 那这也就说明p指向的内容不能通过p改变，但是p的指向却可以改变
        // 也就是通过传值方式进入到函数模板内部后，tmprv指向的内容不能通过tmprv改变，但是tmprv可以指向其他内容
        // tmprv(也就是实参p）的常量性被忽略，tmprv(也就是实参p）所指向内容的常量性会被保留
        // 即在模板函数内：tmprv = nullptr; 这个代码是可以的
        // tmprv = "ppppp"; 这个代码是不行的，编译报错，不能给常量赋值
        
        // 结论：如果传递的是const char*或者const char[]，那么这个const会被保留
    }

}
```



## 传值方式的引申---std::ref与std::cref

C++本身有引用（&），为什么C++11又引入了std::ref（或者std::cref）？**使用std::ref可以在模板传参的时候传入引用，否则只能传值。主要是考虑函数式编程（如std::bind）在使用时，是对参数直接拷贝，而不是引用**。

C++11以后，对于函数模板实参，可以让调用者决定是传值还是传引用。当函数模板声明为值传递方式传递参数，调用者可以使用std::cref()和std::ref()来以引用方式传递参数，他们在头文件<functional>中声明。

```cpp
template<typename T>
void printT(T arg)
{...}

std::string s = "hello";
printT(s);  // pass by value
printT(std::cref(s));  // pass s "as if by reference"
```

然而，std::cref()并不改变模板中参数的处理，而是它使用了一个技巧：将传入的参数s进行包装，使得它表现得像一个引用。事实上，它创建了一个对象，类型为`std::reference_wrapper<>`，指向原来的实参并以值方式传递该对象。该包装或多或少只支持一个操作：隐式类型转化为原来的类型，得到原来的对象（可以在引用包装上调用get()方法，将它作为函数对象） 。因此，对于传入的对象，任何时候你想要一个有效的操作，可以使用reference wrapper代替：

```cpp
//basics/cref.cpp
#include <functional>
#include <string>
#include <iostream>

void printString(std::string const& s)
{
    std::cout << s << '\n';
} 

template<typename T>
void printT(T arg)
{
    printString(arg);  // might convert arg back to std::string
}

int main()
{
    std::string s = "hello";
    printT(s);   // print s passed by value;
    printT(std::cref(s));  // print s passed "as if by reference"
}
```

最后一个调用以值方式传递对象`std::reference_wrapper<std::string const>`给参数arg，然后进一步进行传递，然后将其转化为潜在的类型：std::string。

注意：[编译器](https://www.zhihu.com/search?q=编译器&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"article"%2C"sourceId"%3A412220759})必须知道隐式转化为原来类型是必须的。出于这个原因，std::ref()和std::cref()通常只有在将对象通过[泛型代码](https://www.zhihu.com/search?q=泛型代码&search_source=Entity&hybrid_search_source=Entity&hybrid_search_extra={"sourceType"%3A"article"%2C"sourceId"%3A412220759})传递给非泛型函数时才能适用。比如，尝试直接输出泛型类型T的对象会失败，因为std::reference_wrapper<>没有输出运算：

```cpp
template<typename T>
void printV(T arg)
{
    std::cout << arg << '\n';
}
...
std::string s = "hello";
printV(s);  // OK
printV(std::cref(s)); // ERROR: no operator << for reference wrapper defined
```

以下方法也很会失败，因为不能比较reference wrapper和char const* 或者std::string：

```cpp
template<typename T1, typename T2>
bool isless(T1 arg1, T2 arg2)
{
    return arg1 < arg2;
}
...
std::string s = "hello";
if(isless(std::cref(s), "world")) ... // ERROR
if(isless(std::cref(s), std::string("world"))) ... // ERROR
```

给arg1和arg2一个通用类型T也不适用：

```cpp
template<typename T>
bool isless(T arg1, T arg2)
{
    return arg1 < arg2;
}
```

因为编译器当尝试从arg1和arg2中推断出T的类型会得到冲突类型。

因此，类`std::reference_wrapper<>`的效果使得“使用引用作为一流对象” ，你可以拷贝然后以值方式传递给函数模板。你也可以在类内部使用，比如在容器中保存指向对象的引用。但是你总是最终需要将其转化回潜在的类型。

传值方式的引申 ： std::ref与std::cref
当函数模板定义中使用传值方式进行参数传递时，可以使用std::ref或者std::cref以引用和常量引用来进行传值

```c++
namespace _nmsp5
{
    
    // 函数模板
    template<typename T>
    void getTypeByBoost(T tmprv) // 传值方式的引申 ： std::ref与std::cref
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        
        // 如果是使用std::ref传递，那么可以通过下面这种方式来修改传递进来的值
        // int &tv = tmprv;
        // tv = 10000;
        
        // 如果是std::cref，那么传递进来的相当于是const &，是不能进行值的修改的
    }
    
    void func()
    {
        int i = 100;        // i的类型int
        
        
        // std::ref和std::cref像对象包装器，编译器通过创建一个 classstd::reference_wrapper<T>类型的对象
        getTypeByBoost(std::ref(i));
        // ------------------------------begin----------------------------
        //  T 类型 = std::reference_wrapper<int>
        //  tmprv 类型 = std::reference_wrapper<int>
        // ------------------------------endl-----------------------------
        
        
        getTypeByBoost(std::cref(i));
        // ------------------------------begin----------------------------
        //  T 类型 = std::reference_wrapper<int const>
        //  tmprv 类型 = std::reference_wrapper<int const>
        // ------------------------------endl-----------------------------
        
        
    }

}
```



## 数组做实参

结论：对于数组作为实参，当函数模板参数为传值方式 (T)的时候，推断出来的就是指针类型，当函数模板参数为引用传值（T&）的时候，推断出来的就是数组类型

```c++
namespace _nmsp6
{
    
    // 函数模板
    template<typename T>
    // void getTypeByBoost(T tmprv) // 数组做实参 【这种T推断出来的是指针类型】
    void getTypeByBoost(T& tmprv) // 数组引用做形参 【这种推断出来的是数组类型】
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        

    }
    
    // 函数模板 (通过这种写法，还能取出数组的长度 length)
    template<typename T, unsigned L>    // L就是数组长度
    void getTypeByBoostAndLength(T (&tmprv)[L]) // 数组引用做形参
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        cout << "取出数组长度： " << L << endl;
        cout << "------------------------------endl-----------------------------" << endl;
        

    }
    
    void func()
    {
        const char mystr[] = "i love china";
        
        getTypeByBoost(mystr);
        // ------------------------------begin----------------------------
        //  T 类型 = char const*
        //  tmprv 类型 = char const*
        // ------------------------------endl-----------------------------
        
        // 值传递换成引用传值之后
        
        // ------------------------------begin----------------------------
        //  T 类型 = char const [13]
        //  tmprv 类型 = char const (&) [13]
        // ------------------------------endl-----------------------------
        
        getTypeByBoostAndLength(mystr);
        // ------------------------------begin----------------------------
        //  T 类型 = char const
        //  tmprv 类型 = char const (&) [13]
        // 取出数组长度： 13
        // ------------------------------endl-----------------------------
        
    }

}
```



## 函数名做实参

结论：对于函数名作为实参，当函数模板参数为传值方式 (T)的时候，推断出来的就是函数指针类型，void (*)()
当函数模板参数为引用传值（T&）的时候，推断出来的就是函数引用类型,void (&)()

```c++
namespace _nmsp7
{
    // 函数名做实参
    
    // 函数模板
    template<typename T>
    void getTypeByBoost(T tmprv) // 函数名做实参 【这种T推断出来的是指针类型， 函数指针类型】
    // void getTypeByBoost(T& tmprv) // 函数名形参 【这种推断出来的是引用类型，函数引用类型】
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        

    }
    
    void funcTT() {}
    
    
    void func()
    {
        getTypeByBoost(funcTT);
        // ------------------------------begin----------------------------
        //  T 类型 = void (*)()
        //  tmprv 类型 = void (*)()
        // ------------------------------endl-----------------------------
        
        // T改为传引用 T&
        
        // ------------------------------begin----------------------------
        //  T 类型 = void ()
        //  tmprv 类型 = void (&)()
        // ------------------------------endl-----------------------------        // 
        
    }

}
```



## 初始化列表做实参

```c++
namespace _nmsp8
{
    // 初始化列表做实参
    
    // 函数模板
    template<typename T>
    // void getTypeByBoost(T tmprv)    // 初始化列表做实参，这种写法不行
    // void getTypeByBoost(T& tmprv)    // 初始化列表做实参，这种写法也不行
    void getTypeByBoost(std::initializer_list<T> tmprv)  // 注意，初始化列表做实参，那么形参也要是初始化列表，不然编译器推断不出来会报错
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        

    }
    
    
    void func()
    {
        getTypeByBoost({1, 3, 5});
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = std::initializer_list<int>
        // ------------------------------endl-----------------------------
        
    }

}

```



## 总结

- 推断中，引用类型实参的引用类型等于不存在
- 万能引用，实参为左值或者右值，推断的结果不同
- 按值传递的实参，传递给形参时const属性不起作用，但是传递进去的指针则另当别论
- 数组或者函数类型在类型推断中会被看做是指针，这属于是类型退化。除非函数模板的形参是一个引用
- 初始化列表不能直接推断成std::initializer_list<T>类型，必须在函数模板的形参中明确指出使用

# auto类型常规推断

auto用于变量的自动类型推断。声明变量的时候根据初始化的类型自动为此变量选择匹配的类型，不需要程序员显示的指定类型

auto的自动类型推断发生在编译期间

auto定义变量的时候必须立即初始化，这样编译器才能推断出他的实际类型，这样编译器才能确定auto的类型以及整个变量的类型，然后在编译期间就可以用真正的类型替换掉auto这个类型占位符

auto的使用比较灵活，可以和指针，引用，const等限定符结合使用

auto推断出来后会代表一个具体类型，auto相当于函数模板推断中的类型参数T

## 传值方式（非指针，非引用）

auto后面直接接变量名，这叫传值方式

总结传值方式对auto类型的影响：会抛弃引用，const等限定修饰符

```c++
namespace _nmsp9
{

    void func()
    {
        using boost::typeindex::type_id_with_cvr;
        
        auto x = 10;
        cout << "x的类型 = " << type_id_with_cvr<decltype(x)>().pretty_name() << endl;
        // x的类型 = int
        
        const auto x2 = x;
        cout << "x2的类型 = " << type_id_with_cvr<decltype(x2)>().pretty_name() << endl;
        // x2的类型 = int const
        
        
        const auto& xy = x; // 这个auto不是传值方式
        cout << "xy的类型 = " << type_id_with_cvr<decltype(xy)>().pretty_name() << endl;
        // xy的类型 = int const&
        
        
        auto xy2 = xy;
        cout << "xy2的类型 = " << type_id_with_cvr<decltype(xy2)>().pretty_name() << endl;
        // xy2的类型 = int
    }
}

```



## 指针或者引用类型但不是万能引用

如果auto后面接一个&，这就叫引用类型.

总结传指针或者引用方式针对auto类型：不会抛弃const限定符，但是会抛弃引用。

```c++
namespace _nmsp9
{
     template<typename T>
    void tf(const T& tmprv)  // 注意,这里把auto替换成T，xy就相当于这里的tmprv
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
        

    }
    
    void func()
    {
        using boost::typeindex::type_id_with_cvr;
        
        auto x = 10;
        cout << "x的类型 = " << type_id_with_cvr<decltype(x)>().pretty_name() << endl;
        // x的类型 = int
        
        const auto x2 = x;
        cout << "x2的类型 = " << type_id_with_cvr<decltype(x2)>().pretty_name() << endl;
        // x2的类型 = int const
        
        
        
        const auto& xy = x; // 这个auto不是传值方式
        cout << "xy的类型 = " << type_id_with_cvr<decltype(xy)>().pretty_name() << endl;
        // xy的类型 = int const&
        tf(x);  // 然后为了推断出这里auto的类型和xy的类型，这里传递进去的实参是x，注意这里传递的是x
        // ------------------------------begin----------------------------
        //  T 类型 = int      =====》 auto：也就是int类型
        //  tmprv 类型 = int const& ====》xy：也就是int const&类型
        // ------------------------------endl-----------------------------
        
        auto xy2 = xy;
        cout << "xy2的类型 = " << type_id_with_cvr<decltype(xy2)>().pretty_name() << endl;
        // xy2的类型 = int
        
        auto& xy3 = xy;
        // 推断这里 xy3 = const int &, atuo = const int。因为针对引用类型：引用会被丢弃，const会被保留
        
        auto y = new auto(100);
        // 推断：y = int *， auto = int*. auto可以用于new操作符
        
        const auto* xp = &x;
        // 推断：xp = const int*, auto = int
        
        auto* xp2 = &x;
        // 推断：xp = int*, auto = int
        
        auto xp3 = &x;
        // 推断：xp = int*, auto = int*        xp3不声明为指针，也能推导出指针类型
    }
}
```



## 万能引用类型

auto里的万能引用类型与讲解函数模板类型推断中的万能引用情形完全相同，

```c++
namespace _nmsp10
{
    
    
    void func()
    {
        auto&& wnyy0 = 1212;
        // 1212 是右值，auto = int, wnyy0 = int&& (右值引用类型）
        
        int x = 100;
        auto&& wnyy1 = x;
        // 万能引用，x是左值，所以auto = int &， wnyy1 = int &
        // 这里发生了引用折叠
        
        const int& x2 = x;
        auto&& wnyy2 = x2;
        // 万能引用，x2类型 = const int&, wnyy2 = cosnt int &
        // 这里也发生了引用折叠
    }
}
```

