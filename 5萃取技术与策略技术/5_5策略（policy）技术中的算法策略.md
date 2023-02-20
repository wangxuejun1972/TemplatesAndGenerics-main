# 策略（policy）技术中的算法策略

## 常规范例：普通策略类

```c++
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
    // 写一个适合求最小值的固定萃取类模板
    template<typename T>
    struct MinFixedTraits;
    
    template<>
    struct MinFixedTraits<int>
    {
        using sumT = int;   // 求最小值，结果类型与元素类型相同即可，为了名字统一调用（因为下面有用到），所以都叫sumT
        static sumT initValue() {return INT_MAX; }
        // 要求数组的最小值，这里可以返回一个最大值做初始值
        // INT_MAX 是系统定义的 整形最大值
    };
    
    // ----------------------------------------
    // 将求和操作（也就是算法）写成一个策略类
    struct sumPolicy
    {
        // 算法一般实现成静态成员函数模板
        // sumT 和值类型
        // T是元素类型
        template<typename sumT, typename T>
        static void algorithm(sumT &sum, const T& value)    // 策略类核心算法
        {
            sum += value;   // 求和
        }
    };
    // ----------------------------------------
    // 实现一个求最小值的策略类
    struct MinPolicy
    {
        // minT:最小值类型
        // T：数组元素类型
        template<typename minT, typename T>
        static void algorithm(minT &min, const T& value)
        {
            if (min > value)
            {
                min = value;
            }
        }
    };
    
    // ----------------------------------------
    // 给funcsum增加一个模板参数以进一步funcsum的灵活性
    template<typename T, typename U = SumFixedTraits<T>, typename Plicy = sumPolicy>
    auto funcsum(const T* begin, const T* end)
    {
        typename U::sumT sum = U::initValue();
        
        for(;;)
        {
            Plicy::algorithm(sum, *begin);
            if (begin == end)
                break;
            ++begin;
        }
        return sum;
    }
    // ----------------------------------------
    
    
    void func()
    {
        char mc[] = "abc";
        std::cout << (int)funcsum(&mc[0], &mc[2]) << std::endl;
        // 294
        
        
        int mi[] = {10, 34, 2, 6, 12, 5, 1, 90, 2, 52};
        std::cout << funcsum<int, SumFixedTraits<int>, MinPolicy>(&mi[0], &mi[9]) << std::endl;
        // 0
        // 看到结果是 0 ，并不是1，因为这里我们给sum符了一个初值0，所以它判断不到比0大的真正的最小值
        
        // 有两个解决方式，写一个新的固定萃取类模板来取代SumFixedTraits
        // 或者给 funcsum 这个函数模板新增一个非类型参数类接收 初值
        
        // MinFixedTraits
        std::cout << funcsum<int, MinFixedTraits<int>, MinPolicy>(&mi[0], &mi[9]) << std::endl;
        // 1
        
    }
}
```



## 常规范例：策略类模板

在上面这个范例中，无论是求和还是求最小值，都是普通的类，每个类中包含的是一个静态成员函数模板
其实也可以把这两个策略类写成一个类模板，
直接将这两个类的静态成员函数模板作为模板参数搬到类模板定义中作为模板参数即可

```c++
namespace _nmsp2
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
    // 写一个适合求最小值的固定萃取类模板
    template<typename T>
    struct MinFixedTraits;
    
    template<>
    struct MinFixedTraits<int>
    {
        using sumT = int;   // 求最小值，结果类型与元素类型相同即可，为了名字统一调用（因为下面有用到），所以都叫sumT
        static sumT initValue() {return INT_MAX; }
        // 要求数组的最小值，这里可以返回一个最大值做初始值
        // INT_MAX 是系统定义的 整形最大值
    };
    
    // ----------------------------------------
    // 将求和操作（也就是算法）写成一个策略类模板
    template<typename sumT, typename T>
    struct SumPolicy
    {
        // 算法一般实现成静态成员函数模板
        // sumT 和值类型
        // T是元素类型
        static void algorithm(sumT &sum, const T& value)    // 策略类核心算法
        {
            sum += value;   // 求和
        }
    };
    // ----------------------------------------
    // 实现一个求最小值的策略类模板
    template<typename minT, typename T>
    struct MinPolicy
    {
        // minT:最小值类型
        // T：数组元素类型
        static void algorithm(minT &min, const T& value)
        {
            if (min > value)
            {
                min = value;
            }
        }
    };
    
    // ----------------------------------------
    // 给funcsum增加一个模板参数以进一步funcsum的灵活性
    // T 是数组元素类型，
    // U 是固定萃取类模板
    // U::sumT 就是从类模板中提取出来的计算结果的类型
    // Plicy 是策略类模板，用于实现具体算法（求和，求最小值）
    template<typename T,
        typename U = SumFixedTraits<T>,
        template<class, class> class Plicy = SumPolicy> // 模板模板参数
    auto funcsum(const T* begin, const T* end)
    {
        typename U::sumT sum = U::initValue();
        
        for(;;)
        {
            
            Plicy<typename U::sumT, T>::algorithm(sum, *begin);
            if (begin == end)
                break;
            ++begin;
        }
        return sum;
    }
    // ----------------------------------------
    
    
    void func()
    {
        char mc[] = "abc";
        std::cout << (int)funcsum(&mc[0], &mc[2]) << std::endl;
        // 294
        
        
        int mi[] = {10, 34, 2, 6, 12, 5, 1, 90, 2, 52};
        std::cout << funcsum<int, SumFixedTraits<int>, MinPolicy>(&mi[0], &mi[9]) << std::endl;
        // 0
        // 看到结果是 0 ，并不是1，因为这里我们给sum符了一个初值0，所以它判断不到比0大的真正的最小值
        
        // 有两个解决方式，写一个新的固定萃取类模板来取代SumFixedTraits
        // 或者给 funcsum 这个函数模板新增一个非类型参数类接收 初值
        
        // MinFixedTraits
        std::cout << funcsum<int, MinFixedTraits<int>, MinPolicy>(&mi[0], &mi[9]) << std::endl;
        // 1
        
    }
}
```



## 萃取（trait）技术与策略（policy）技术的比较

两者之间的相同和不同之处

- 两种技术都像是一个中间件一样，夹在不同的代码之间，让代码调用更加灵活
- 萃取技术给进去一个类型，萃取出另外一个类型或者另外一个值（注重的是类型或者值），所以书写萃取类的时候一般都会包含一个静态成员变量，用于保存类型或者值
- 策略技术是给进去一个类型，萃取出一个算法或者是一个不同的功能实现（注重于动作或者行为），所以书写策略类的时候一般都会包含一个成员函数，用于实现动作
- 当然在某些项目中，也可能会出现用萃取实现某些算法，所以从这个角度来讲，萃取（trait）和 策略（policy）的区分其实没那么严格
- 萃取技术一般用一个类模板实现，通常会包含一个泛化版本和多个特化版本
- 策略技术用普通的类或者类模板都能实现



 trait模板与policy模板技术的比较：

-   （1）trait注重于类型，policy更注重于行为。
-   （2）trait可以不通过模板参数来传递，它表示的类型通常具有自然的缺省值（如int型为0），它依赖于一个或多个主参数，它 一般用模板来实现。
-   （3）policy可以用普通类来实现，也可以用类模板来实现，一般通过模板参数来传递。它并不需要类型有缺省值，缺省值通常是在policy中的成员函数中用一个独立的参数来传递。它通常并不直接依赖于模板参数。

  一般在模板中指定两个模板参数来传递trait和policy。而policy的种类更多，使用更频繁，因此通常代表policy的模板参数在代表trait的模板参数前面。
  标准库中的std::iterator_traits<T>是一个trait，可通过iterator_traits<T>::value_ type来引用T表示的具体类型。其实现也是用特化来获取各个具体的类型，有全局特化也有局部物化，如指针类型，引用类型等就只能通过局部特化为T*,T&来实现。