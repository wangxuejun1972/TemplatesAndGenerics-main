# 理解函数模板类型推断、查看类型推断结果、auto类型推断

## 如何查看类型推断结果



## 理解函数模板类型推断

### 引用或指针类型

引用类型

```c++
// This file is a "Hello, world!" in C++ language by GCC for wandbox.
#include <iostream>
#include <cstdlib>
#include <boost/type_index.hpp>

using namespace std;


// 理解函数模板类型推断

// 看看函数模板的形参（tmprv）是如下类型时，编译器推断出的类型模板参数的类型T，以及最终的函数形参的类型（tmprv）
// T的类型不仅仅取决于实参100，还取决于tmprv的类型（const T&）有关

// 引用或者自制类型
// 根据结果得到一些结论
// 
    // 1) 若实参是引用类型，引用部分会被忽略，T不会被推导为引用类型 【这个需要记】
    // 2）当向引用类型形参传入const类型实参时，形参会成为const引用 【原来是一个引用，带上const就变成了const&】
    // 而且可以看到，实参的const属性会成为类型模板参数T类型推导的组成部分，所以不用担心在getTypeByBoost中能够修改原来的const属性的实参
    // 也就是说，不用担心模板函数里会不会不因为这个 & 就导致函数里会修改实参值了
    
    // 1）如果实参是引用类型，引用部分会被忽略，T不会被推导为引用类型 【这个需要记】
    // 2）T中的const没有了，因为函数模板的形参中出现了const，但是不管怎么说，只要实参中带了const修饰，形参中就一定会有const
    // 只不过是在T类型中还是在tmprv类型中而已
    
    // 通过下面的范例，总结模板形参编码技巧：
    // 1）模板形参中的引用，有两个作用，第一是可以通过对形参的修改来修改实参，第二是因为传递引用比传值效率高
    // 所以，一般来说，函数模板中的形参建议优先考虑 T& tmprv 这种格式的形参，这样写就不怕实参中的引用会被忽略掉，就可以在函数中通过修改形参达到修改实参的目的
    // 2）如果既想享受形参为引用带来的效率提升，又不希望形参会改动到实参，那就可以将模板参数设置为 常量引用 const T& tmprv；同时常量引用还有一个优点是可以传递进来常量实参
    // 3）如果tmprv是一个指针类型：
    // a）如果形参tmprv中没有const，那么实参中的const会被带到类型T中去
    // b）如果形参tmprv中有const，那么T类型中不会带const
    // c）a)这个结论其实对于引用类型形参也适用（T& 或者 const T&）

namespace _nmsp1
{
    // 函数模板
    template<typename T>
    // void getTypeByBoost(T& tmprv)
    void getTypeByBoost(const T& tmprv) // 将模板参数T从引用变为常量引用
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
    }

    void func()
    {
        int i = 18; // i 的类型是 int
        const int j = i;    // j 的类型是const int
        const int& k = i;   // k 的类型是const int&
        
        getTypeByBoost(199);    // 模板参数T为引用类型的时候，这样传值会报错、但是改为常量引用后就没问题
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        
        getTypeByBoost(i);  // i 的类型是 int
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int &
        // ------------------------------endl-----------------------------
        
        // 模板参数T改为常量引用后
        
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        getTypeByBoost(j);  // j 的类型是const int
        // ------------------------------begin----------------------------
        //  T 类型 = int const
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        // 模板参数T改为常量引用后
        
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------

        
        getTypeByBoost(k);  // k 的类型是const int&
        // ------------------------------begin----------------------------
        //  T 类型 = int const
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
        
        // 模板参数T改为常量引用后
        
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const&
        // ------------------------------endl-----------------------------
    }
}

namespace _nmsp2
{
    
    // 函数模板
    template<typename T>
    // void getTypeByBoost(T* tmprv)
    void getTypeByBoost(const T* tmprv) // 改成常量指针
    {
        cout << "------------------------------begin----------------------------" << endl;
        
        using boost::typeindex::type_id_with_cvr;
        cout << " T 类型 = " << type_id_with_cvr<T>().pretty_name() << endl;      // 显示T的类型
        cout << " tmprv 类型 = " << type_id_with_cvr<decltype(tmprv)>().pretty_name() << endl; // 显示tmprv的类型
        
        cout << "------------------------------endl-----------------------------" << endl;
    }
    
    void func()
    {
        int i = 100;
        const int *pi = &i;
        
        getTypeByBoost(&i);
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int*
        // ------------------------------endl-----------------------------
        
        // 改成常量指针
        
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const*
        // ------------------------------endl-----------------------------
        
        
        getTypeByBoost(pi);
        // ------------------------------begin----------------------------
        //  T 类型 = int const
        //  tmprv 类型 = int const*
        // ------------------------------endl-----------------------------
        
        // 改成常量指针
        
        // ------------------------------begin----------------------------
        //  T 类型 = int
        //  tmprv 类型 = int const*
        // ------------------------------endl-----------------------------
        
    }

}


int main()
{
    
    
   
    // _nmsp1::func();
    
    _nmsp2::func();
    
    

    return 0;
}

// GCC reference:
//   https://gcc.gnu.org/

// C++ language references:
//   https://cppreference.com/
//   https://isocpp.org/
//   http://www.open-std.org/jtc1/sc22/wg21/

// Boost libraries references:
//   https://www.boost.org/doc/


```



