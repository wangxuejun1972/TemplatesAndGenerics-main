/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <tuple>

// 元组的基本概念
// boost库中有一个tuple，然后在c++11中，把这个tuple放到标准库中

// tuple容器(元组), 是表示元组容器, 是不包含任何结构的,快速而低质(粗制滥造, quick and dirty)的, 可以用于函数返回多个返回值;

// tuple容器, 可以使用直接初始化, 和"make_tuple()"初始化, 访问元素使用"get<>()"方法, 注意get里面的位置信息, 必须是常量表达式(const expression);

// 可以通过"std::tuple_size<decltype(t)>::value"获取元素数量; "std::tuple_element<0, decltype(t)>::type"获取元素类型;

// 如果tuple类型进行比较, 则需要保持元素数量相同, 类型可以比较, 如相同类型, 或可以相互转换类型(int&double);

// 无法通过普通的方法遍历tuple容器, 因为"get<>()"方法, 无法使用变量获取值;

// 通俗来讲，tuple是一个容器，里面可以装各种不同类型元素的数组/容器

namespace _nmsp1
{
  // 简单使用
  void func()
  {
    std::tuple<float, int, std::string> mytuple(12.4f, 23, std::string("string"));
    // 这就是一个tuple。一堆各种类型数据的组合
    // 虽然创建元组的时候，可以创建任意类型，任意数量的元素，但是一旦元组创建好了之后，元组的的数量和类型就固定下来了

    // get 是一个函数模板
    std::cout << std::get<0>(mytuple) << std::endl;
    // 12.4
  }
}

// tuple 的实现放在这个命名空间中
// 首先，tuple的实现要用到可变参类模板，那可变参参数包的展开，有如下几种展开方式
// 1）通过递归继承方式展开 （标准库中的展开方式是递归继承方式，通过观察源码可以看到）
// 2）通过递归组合方式展开 （那这里自己实现就采用递归组合方式进行参数包展开）
// 递归组合方式比起递归继承方式的缺点：占用的内存可能会相对多一点（一般也就是一个字节，就是涉及到一个“空白基类优化【EBCO】”）

namespace tpl
{
  // 该类模板是用来判断一个类型T中是否有成员函数 myfunc_NIL （展开来讲就是判断该类型T是否是TuplE类）
  // 泛化版本
  template <typename T, typename U = std::void_t<>>
  struct HasNameFunc : std::false_type
  {
  };

  // 特化版本
  template <typename T>
  struct HasNameFunc<T, std::void_t<decltype(std::declval<T>().myfunc_NIL())>> : std::true_type
  {
  };

  // ----------------------------------------------------------------------------------

  // 泛化版本
  template <typename... Types>
  class TuplE;

  // 特化版本1
  template <typename First, typename... Others>
  class TuplE<First, Others...>
  {
  public:
    First first;
    TuplE<Others...> others;

  public:
    // 构造函数1,支持创建空的TuplE对象 tpl::TuplE<int, char> a;
    // 这种构造方式（虽然你参数类型哪里指定了2个，但是实际传递给构造函数的（）里并没有填写实参）
    TuplE() {}

    // 构造函数模板， 支持 tpl::TuplE<float, int, std::string> mytuple(12.4f, 23, std::string("string"));
    // 尖括号里给类型，圆括号里给数据的构造方式
    // C_First：代表一个元素类型，然后C_Others代表其余一包元素类型
    // template<typename C_First, typename... C_Others>
    // TuplE(C_First&& parf, C_Others&&... paro)
    //     : first(std::forward<C_First>(parf))        // first/parf代表第一个元素
    //     , others(std::forward<C_Others>(paro)...)   // others/paro代表其余元素
    //     // others的类型 TuplE<...>这个类型

    // 修改本构造函数模板，让它只有在第一个参数类型不是TuplE类型的时候才有效
    template <typename C_First, typename... C_Others, typename = std::enable_if_t<!HasNameFunc<C_First>::value>>
    TuplE(C_First &&parf, C_Others &&...paro)
        : first(std::forward<C_First>(parf)) // first/parf代表第一个元素
          ,
          others(std::forward<C_Others>(paro)...) // others/paro代表其余元素
                                                  // others的类型 TuplE<...>这个类型
    {
      // std::cout << "TuplE构造函数模板执行了， first = " << first << std::endl;
      // 但是考虑到不是所有类型都能通过cout输出，为了避免编译报错，这里就不打印first了
      std::cout << "TuplE特化版本1构造函数模板执行了" << std::endl;
    }

    // 这里初始化列表中用到了完美转发（std::forward在这里就可以起到提高程序运行效率的作用）

    // ----------------------------------------------------------------------------------
    // 为了支持拷贝构造，这里给TuplE新增一个拷贝构造函数
    TuplE(const TuplE<First, Others...> &tmpv) : first(tmpv.first), others(tmpv.others)
    // TuplE(TuplE<First, Others...>& tmpv):first(tmpv.first), others(tmpv.others)
    {
      std::cout << "TuplE特化版本1的拷贝构造函数执行了" << std::endl;
    }

    // 拷贝构造函数模板
    template <typename C_First, typename... C_Others>
    TuplE(const TuplE<C_First, C_Others...> &tmpv) : first(tmpv.first), others(tmpv.others)
    {
      std::cout << "TuplE特化版本1的拷贝构造函数模板执行了" << std::endl;
    }

  public:
    void myfunc_NIL()
    {
      // HasNameFunc 这个类模板是用来判断某个类中是否存在某个成员函数
      // 然后，在这里我们可以通过这个类模板来判断一个类型中是否存在 myfunc_NIL这个成员函数
      // 如果存在，则认为这个类型是TuplE类型
    }
  };

  // 特化版本2：空元组，不需要存储任何类型，支持 tpl::TuplE<> a; 这构造方式
  template <>
  class TuplE<>
  {
  public:
    // 构造函数
    TuplE()
    {
      m_sign = 0; // 随便给一个值，方便调试
      std::cout << "TuplE特化版本2构造函数执行了 " << std::endl;
    }

    // 拷贝构造函数
    TuplE(const TuplE<> &)
    {
      m_sign = 100;
      std::cout << "TuplE特化版本2拷贝构造函数执行了" << std::endl;
    }

    int m_sign;
    // 引入的目的仅仅是为了设置断点，调试用
  };

  // -----------------------------------------------------------------------------------
  // 实现获取tuple中元素的get接口
  // 泛化版本
  template <int index>
  class TPLGetIdx
  {
  public:
    template <typename First, typename... Others>
    static auto myget(const TuplE<First, Others...> &tmpv) // 这里auto用的很好
    {
      return TPLGetIdx<index - 1>::myget(tmpv.others);
    }
  };

  // 特化版本
  template <>
  class TPLGetIdx<0>
  {
  public:
    template <typename First, typename... Others>
    static auto myget(const TuplE<First, Others...> &tmpv) // 这里auto用的很好
    {
      return tmpv.first;
    }
  };

  // 借助上面这个类模板实现get功能
  template <int index, typename... Types>
  auto TPLGet(const TuplE<Types...> &tmpv)
  {
    return TPLGetIdx<index>::myget(tmpv);
  }

  // -----------------------------------------------------------------------------------
  // std::make_tuple的实现
  // c++标准库中有一个std::make_tuple，这个是一个函数模板，他的功能就是给定一堆值然后创建一个tuple对象
  // make_tuple有一个使用限制，就是必须已知元素数量才能使用
  // auto mytu = std::make_tuple<12.5f,100,std::string("abc")>;  // 3个元素

}

// MakeTuple演示了类型推导技术，函数模板可以通过实参中给进来的元素值推导出元素类型
// 这样创建元组就会很方便，不用我们手动去指定元素类型，直接让编译器根据实参进行类型推导
template <typename... Types>
auto MakeTuple(Types &&...Args) // 形参时万能引用（转发引用）
{
  // 退化（decay）
  return tpl::TuplE<std::decay_t<Types>...>(std::forward<Types>(Args)...);
}

int main()
{
  _nmsp1::func();

  int i = 100;

  tpl::TuplE<float, int, std::string> mytuple(12.5f, i, std::string("abc"));
  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本1构造函数模板执行了

  // tpl::TuplE<float, int, std::string>
  // 首先先将 float拆分出来 float 和 tpl::TuplE<int, std::string>
  // 然后继续将int拆分出来 int 和 tpl::TuplE<std::string>
  // 然后在将 std::string拆分出来 std::string 和 tpl::TuplE<>
  // tpl::TuplE<>
  // 然后递归回溯，依次执行的构造函数就是
  // TuplE特化版本2构造函数执行了 -> 构造空TuplE
  // TuplE特化版本1构造函数模板执行了 -> 构造 string
  // TuplE特化版本1构造函数模板执行了 -> 构造 int
  // TuplE特化版本1构造函数模板执行了 -> 构造 float

  std::cout << "------------------------------------------1----------------------------------------" << std::endl;

  tpl::TuplE<float, int, std::string> mytuple2(mytuple);
  // 我们这行代码的本意是想通过mytuple作为参数通过拷贝构造将mytuple2构造出来，但是发现，在掉用的时候报了编译错误
  // 是因为编译器在执行这块构造的时候，并没有调用我们提供的拷贝构造函数，而是去执行了构造函数模板
  // 那就在他执行构造函数模板的时候就报编译错误了
  // 主要就是在构造函数模板中的这个代码段引起的 first(std::forward<C_First>(parf))
  // 这里first类型，他应该是一个float类型，但是这里传递进来的实参mytuple，也就是这个parf,他的类型是 tpl::TuplE<float, int, std::string>
  // 怎么可以把 tpl::TuplE<float, int, std::string>这个类型赋值给float类型呢？
  // 所以就在这里进行了报错

  // 改造成std::enable_if后就能正常调用拷贝构造函数了
  // TuplE特化版本2拷贝构造函数执行了
  // TuplE特化版本1的拷贝构�函数执行了
  // TuplE特化版本1的拷贝构造函数执行了
  // TuplE特化版本1的拷贝��造函数执行了

  std::cout << "------------------------------------------2----------------------------------------" << std::endl;

  tpl::TuplE<double, int, std::string> mytuple3(mytuple);
  // 这里我们把float换成double，我们知道，float是可以转换成double的，而且double精度更高，但是这个时候就编译错误了
  // 因为 mytuple 类型和 tpl::TuplE<double, int, std::string> 类型不匹配导致
  // 那怎么解决这个问题呢？
  // 那这里就要在TuplE的特化版本1中引入拷贝构造函数模板了

  // 引入函数拷贝构造函数模板之后，成功执行（这时候就允许待拷贝对象和构造对象的类型不同也能正常进行构造了）
  // TuplE特化版本2拷贝构造函数执行了
  // TuplE特化版本1的拷贝构造函��执行了
  // TuplE特化版本1的拷贝构造函数执行了
  // TuplE特化版本1的拷贝构造函数模板执行了 *****

  // 拷贝构造函数模板永远不可能成为拷贝构造函数，当两个元素类型不同的时候，
  // 构造元素或者对象时使用的拷贝构造函数模板，而不是拷贝构造函数
  // 所以拷贝构造函数模板在这里必须得有，除了应付我们现在的这个情况，
  // 还有其他的一些情况因为你没有拷贝构造函数模板的存在而编译错误

  std::cout << "------------------------------------------3----------------------------------------" << std::endl;

  // 解决方法就是，使用 std::enable_if 或者 把拷贝构造函数中的const给去掉
  // （但是这种去掉const修饰符也会带来额外的问题，const修饰符其实是有额外的作用的，编译器会且只会为const修饰符开绿灯，让他可以绑定左值或者右值
  // 这里如果贸然的把拷贝构造函数中的const去掉的话，可能会照成一些兼容性问题）
  tpl::TuplE<float> tes1(12.5f);
  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数�板执行了
  std::cout << "------------------------------------------4----------------------------------------" << std::endl;
  tpl::TuplE<int, tpl::TuplE<float>> tes2(12, tes1);
  // 拷贝构造函数里的const去掉，执行结果：：
  // TuplE特化版本2拷贝构造函数执行了
  // TuplE特化版本1的拷贝构造��数执行了
  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数模板�行了
  // TuplE特化版本1构造函数模板执行了

  std::cout << "------------------------------------------5----------------------------------------" << std::endl;
  tpl::TuplE<int, tpl::TuplE<float>> tes3(12, tpl::TuplE<float>(12.34f));
  // 和上面一样，唯一的区别是我这里第二个实参我使用临时对象的方式传递，但是这个时候，就报错了
  // 这个报错的原因还是编译器本该掉一共拷贝构造函数的时候，又去调用了构造函数模板

  // 这种带尖括号的类型，比如元组里面套元组，这种在执行拷贝构造的时候，都会调用拷贝构造函数模板
  // 所以添加了拷贝构造函数模板之后，这里就不再报编译错了

  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数��板执行了
  // TuplE特化版本2拷贝构造函数执行了
  // TuplE特化版本1的拷贝构造函数模板执行了
  // TuplE特化版本1构造函数模板执行了

  std::cout << "------------------------------------------6----------------------------------------" << std::endl;
  // 同时去掉const还会有其他新问题，比如你用一个const修饰的TuplE去构造另一个TuplE也不行，也会报编译错误
  const tpl::TuplE<float> tes4(13.3f);
  tpl::TuplE<float> tes5(tes4);
  // 也报错了，报错原因也是编译器在本该调用拷贝构造函数的时候，调用了构造函数模板

  // 所以综合看来，把拷贝构造函数里的const去掉是一个十分不理智的行为（不提倡这么做）
  // 所以这里重点考虑使用std::enable_if来进行解决，就是通过std::enable_if来让不该出现构造函数模板的时候，
  // 就不让它出现，以免照成构造函数模板的出现取代了拷贝构造函数，
  // 那问题就在于std::enable_if的条件怎么写？
  // 其实这里也很简单，因为调用构造函数的时候，具体是调用拷贝构造函数还是调用构造函数模板，取决于第一个形参的类型
  // 如果第一个形参的类型他本身就是一个TuplE类型，那么这里就应该调用拷贝构造函数，否者的话就应该调用构造函数模板

  // 所以焦点问题就在于如何判断第一个形参的类型是否是TuplE类型

  // TuplE特化版本2构造函数执行��
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本2拷贝构造函数执行�
  // TuplE特化版本1的拷贝构造函数执行了

  std::cout << "------------------------------------------7----------------------------------------" << std::endl;
  std::cout << tpl::TPLGet<2>(mytuple) << std::endl;
  // abc

  std::cout << "------------------------------------------8----------------------------------------" << std::endl;
  auto tes6 = MakeTuple(12.5f, 100, std::string("xyz"));
  // TuplE特化版本2构造函数执行了
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本1构造函数模板执行了
  // TuplE特化版本1�造函数模板执行了
  std::cout << tpl::TPLGet<2>(tes6) << std::endl;
  // xyz

  // 当然我们现在的这个代码还不完善，比如TuplE中，嵌套TuplE类型，当元素数量超过一个的时候，构造函数模板的初始化列表就会出错
  // tpl::TuplE<int, double> vt1(100, 12.33);
  // tpl::TuplE<tpl::TuplE<int, double>> vt2(vt1);
  // 编译报错
  // 产生这个错误的原因就是拷贝构造函数模板中C_First类型和我们这里 vt1 的类型不匹配导致
  // 为了应对这个情况，我们还是需要通过std::enable_if来书写新的拷贝构造函数模板
  // 这里就不再进一步展开了

  return 0;
}
