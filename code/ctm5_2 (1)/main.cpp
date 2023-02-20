/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;
// 迭代器范例
// 迭代器分为5类：
// 1）输出型迭代器 struct output_iterator_tag
// 2）输入型迭代器 struct input_iterator_tag
// 3）前向迭代器 struct forward_iterator_tag : input_iterator_tag
// 4）双向迭代器 struct bidirectional_iterator_tag : forward_iterator_tag
// 5）随机访问迭代器 struct random_access_iterator_tag :bidirectional_iterator_tag

// vector容器就是随机访问类型迭代器
// list容器-双向迭代器

// 对于随机访问迭代器，如果想让迭代器位置往后跳5个元素，那么可以直接加一个数字5（+5）就能达到往后跳5个元素的效果
// 对于双向迭代器，比如list，如果想让迭代器位置往后跳5个元素，那么就需要每次往后跳1个元素，循环5次才能达到跳5个元素的效果

// 这种类似的操作在不同类型的迭代器中，完全不同。所以正确的区分迭代器种类（类型）就显得十分重要

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

// 如何通过固定萃取技术从容器中萃取出对应的迭代器种类
// class _List_iterator    // list容器中对应的迭代器，是一个类或者一个类模板
// {
// public:
//     using iterator_category = bidirectional_iterator_tag;   // 代表list容器的迭代器属于哪个种类的一个类型别名
// };

// template<typename T>
// class list
// {
// public:
//     using iterator = _List_iterator;
// };

// 有了这些代码，就不难想象
// list<int>::iterator -----> 对应的就是 -----> _List_iterator::iterator_category
// 然后不就是 bidirectional_iterator_tag，这样就得到了list容器的迭代器种类

// 同理。vector 容器的迭代器也是一样的萃取方法

// 如何找迭代器种类
// 1）在容器中，标示出所属的迭代器种类
// 2）通过 一个 fixed trait（固定萃取）类模板 iterator_traits 来进行类型萃取

namespace _nmsp2
{
    // 固定萃取模板
    // 这里给进来的 类型模板参数 是一个迭代器（迭代器类型）
    template<typename IterT>
    struct iteratorTraits
    {
        // ----............
        using iterator_category = typename IterT::iterator_category;
        // ,......
    };
    
    
    void func()
    {
        
    }
}

int main()
{
    _nmsp1::func();

    return 0;
}
