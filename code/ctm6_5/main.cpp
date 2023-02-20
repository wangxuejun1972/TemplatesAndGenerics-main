/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby,
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <boost/type_index.hpp>

namespace _nmsp1
{
  void tsFunc1(int &j)
  {
  }

  void tsFunc2(const int &j)
  {
  }

  void tsFunc3(int &&j)
  {
  }

  void tsFunc4(const int &&j)
  {
  }

  void tsFunc5(int j)
  {
  }

  void func()
  {
    // 左值。右值、左值引用，右值引用

    int i = 10;
    // i 是左值，10是右值

    // 然后左值引用，谈到引用，就必须有一个地址符（&）
    int &j = i;
    // j 就是一个左值引用，那么给这个左值引用赋值的时候（也就是等号右侧），必须也得是一个左值
    // int& j = 10; // 这个就不行，因为左值引用只能接左值，而10是右值，不能用一个右值给左值引用j赋值

    // 同理，当左值引用作为一个函数形参的时候，那么你传递的实参就得是一个左值
    tsFunc1(i);
    // tsFunc1(10); // 语法错，10 是右值，不可以作为实参传递给左值引用形参

    // 所以记住：左值引用只能绑定到左值上，编译器会且只会为const引用 （const &）开绿灯，可以让其绑定左值，也可以让其绑定右值
    // const例外情况（加上const，如下就没有语法错）

    const int &k = 10;
    tsFunc2(100);

    // 右值引用
    int &&l = 10;
    // l就是一个右值引用，给右值引用赋值的时候（也就是等号右侧），必须也得是一个右值
    // int&& l = i; // 这个就不行，因为右值引用只能接右值，i是左值，不能用一个左值给右值引用l赋值

    // 同理，当右值引用作为一个函数形参的时候，那么你传递的实参就得是一个右值
    tsFunc3(100);
    // tsFunc3(i);  // 语法错，i是左值，不可以作为实参传递给右值引用形参

    // 所以记住：右值引用只能绑定到右值上

    int m = i;
    // 这里m是一个左值引用还是右值引用？
    // 既不是左值引用，也不是右值引用，引用必须得有(&)地址符
    // 那既然他这里既不是左值，又不是右值，那么这里你无论给一个左值还是右值进行赋值，都不会报错。都是可以的
    // 展开了讲，如果你把这个m作为函数形参，那么你调用这个函数的时候，无论传递的是左值还是右值，都没有问题
    tsFunc5(12);
    tsFunc5(i);
  }

}

namespace _nmsp2
{
  // std::move究竟干了什么
  // std::move 类型装换函数，他的能力就是把一个左值转成右值，如果原来是一个右值，那么没有变化
  // 我们可以认为std::move的结果是一个右值引用类型

  void func()
  {
    int i = 10;
    // int&& j = i;    // 报语法错
    int &&j = std::move(i);
    // 将左值转为右值，就可以将i赋给j了
    // 但是注意，这个时候，你把左值i转成右值，然后赋给右值引用j，就相当于把i绑定到j上了
    // 那个这个时候，如果j发生变化，也就相当于i发生变化
    j = 100;
    std::cout << i << std::endl;
    // 100
  }
}

namespace _nmsp3
{
  // std::forward 也是一个类型装换函数（函数模板）
  // 那他和std::move有什么区别？
  // 首先使用上，他比std::move更复杂
  // std::forward首先他会看后面尖括号<>中的内容，尖括号中如果是一个右值引用，那么std::forward就能保证最终结果是右值
  // 如果是左值，那么最终就保证是左值。
  // 所以,std::forward根本不会管你圆括号（实参）里是什么类型，他只看你尖括号中是什么类型就给你转成什么类型

  // std::forward有两个明显的作用
  // 1）提高程序运行效率
  // 2）用于完美转发

  void func()
  {
    int i = 19;
    int &&m = std::forward<int &&>(i);
    // std::forward首先他会看后面尖括号<>中的内容，这里是一个int&&，右值引用类型，
    // 根据右值引用类型绑右值，std::forward在这里的使命就是保证std::forward<int&&>(i)
    // 这一串最终的结果是右值

    // int&& o = std::forward<int&>(100);
    // 语法错，因为 std::forward<int&>(100) 把100转成左值了，然后把左值赋给右值引用就编译报错

    // int& p = std::forward<int&&>(100);
    // 同理一样，因为 std::forward<int&&>(100) 把100转成右值了，然后把右值赋给左值引用就编译报错

    int &&q = std::forward<int>(i);
    // std::forward<>里int 既不是左值引用，也不是右值引用，那么这种情况下，编译器就会把它当做右值引用类型来看待
    // 所以这里就会把i转成右值，然后给右值引用q赋值，所以没问题

    std::string str1 = "abc";
    std::string str2 = str1;
    // 通过str1这个字符串来构造str2（执行string的拷贝构造函数）
    // str2构造完成后，他里面会保存1分“abc”，然后str1中也保存一份“abc”
    // 这两份“abc”占用的是不同的内存

    std::string str3 = std::forward<std::string &&>(str1);
    // 因为，使用std::forward<std::string&&>(str1),这个变成了右值引用类型，就导致调用的是string类的移动构造函数
    // 虽然这里也是通过str1来构造str3，但是这里用到了std::forward，他会把整个这个东西转成右值引用类型
    // 这个时候如果变成右值引用类型，那么在构造str3的时候，就会调用string类的移动构造函数
    // 移动构造函数执行，就会把str1中的“abc”，移动到str3中去了，然后str3中保存的是str1中的那个“abc”
    // 那么这个时候，str1中的那个“abc”就不见了
    std::cout << "str1 ---- " << str1 << std::endl;
    // str1 ----
    // 可以看到，并不会打印str1(str1中内容已经变空)
  }

}

namespace _nmsp4
{
  // 万能引用（转发引用）

  // 形参时右值引用类型，只能接右值
  // void myfunc(int&& tmpv)
  // {}

  // 形参时左值引用类型，只能接左值
  // void myfunc(int& tmpv)
  // {}

  // 在这个函数模板中，tmpv就是一个万能引用类型(T必须是一个类型模板参数，然后T前面不能加任何修饰符)
  // 即：你在调用这个函数模板的时候，你传递的实参既可以是左值，又可以是右值
  template <class T>
  void myfunc(T &&tmpv)
  {
    using boost::typeindex::type_id_with_cvr;
    std::cout << "T = " << type_id_with_cvr<T>().pretty_name() << std::endl;
    std::cout << "tmpv的类型 = " << type_id_with_cvr<decltype(tmpv)>().pretty_name() << std::endl;
  }

  template <class T>
  class TmpClass
  {
  public:
    // 注意这里，虽然这个构造函数他的参数类型也是T&&,但是这里并不是万能引用类型
    // TmpClass(T&& tmpvalue)
    // {
    //     std::cout << "TmpClass::TmpClass()执行了" << std::endl;
    // }

    // 那我们把这个构造函数重写，改造成构造函数模板
    // 那这个时候，这里的tmpvalue就是万能引用类型
    template <class U>
    TmpClass(U &&tmpvalue)
    {
      std::cout << "TmpClass::TmpClass()执行了" << std::endl;
    }
  };

  void func()
  {
    int i = 10;
    int &j = i;
    int &&k = 100;

    myfunc(i);
    // T = int&
    // tmpv的类型 = int&
    myfunc(j);
    // T = int&
    // tmpv的类型 = int&
    myfunc(k);
    // T = int&
    // tmpv的类型 = int&
    myfunc(100);
    // T = int
    // tmpv的类型 = int&&

    TmpClass<int> a(10); // 这行可以
    // TmpClass::TmpClass()执行了

    // TmpClass<int> b(j);     // 但是这行并不行（这也就证明了TmpClass的构造函数哪里，并不是万能引用类型）
    TmpClass<int> b(j); // 将构造函数改造成构造函数模板，就变成了万能引用，那这个时候就可以正常构造了
                        // TmpClass::TmpClass()执行了
  }
}

namespace _nmsp5
{
  // 完美转发
  // 设想一下，有一个函数，他的形参是一个右值引用
  // 那如果我们要调用这个函数的话，实参就必须是一个右值
  void lastfunc(int &&tmpv)
  {
  }

  // 这个时候添加一个中间转发函数，把调用的实参传递给这个中间转发函数，然后再由这个转发函数调用我们上面的这个函数
  // 按照经验，这种转发函数一般都是设计成函数模板，然后该函数的形参类型是一个万能引用类型，以便该函数可以转发无论是左值还是右值
  template <class T>
  void middleFunc(T &&tmpvalue)
  {
    // lastfunc(tmpvalue);
    // 但是这个时候就会报错，编译器报错无法将T转换成T&&
    // 不难理解，因为tmpvalue虽然他是一个万能引用类型，然后我们传递进来的是一个右值，这个时候会引用折叠成右值引用
    // 虽然在这里 tmpvalue 是以右值引用类型，但是他本身 tmpvalue 是一个左值
    // (一个变量，他必然是一个左值，只是从类型角度来讲，他可以是一个左值引用，也可以是一个右值引用，也可以不是任何一个引用类型)，
    // 然后我们调用 lastfunc的时候，就相当于把一个左值绑定到了一个右值引用上，那编译器必然会报错

    // 那这个时候就需要将这行调用改成完美转发
    lastfunc(std::forward<T>(tmpvalue));
  }

  void func()
  {
    int i = 10;
    int &j = i;
    int &&k = 100;

    lastfunc(10);

    middleFunc(10);
  }
}

int main()
{
  // _nmsp1::func();
  // _nmsp2::func();
  // _nmsp3::func();
  _nmsp4::func();

  return 0;
}
