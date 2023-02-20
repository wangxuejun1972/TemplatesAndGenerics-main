# C++ 空基类优化

# 1简介:

空基类优化,即当存在空的基类子对象时,可以将其默认规定的占位字节忽略

# 2使用环境和注意点:

## (2.1)

空基类优化是由于,对于一个空类对象而言,为了保证其地址的唯一性,所以其大小必大于等于1以保证占据相应的地址,但是对于基类而言,特别是,对于C++中的standard layout(在[这里](https://www.jianshu.com/p/cf350108270f))由于要保证标准导出,并且可以直接通过[reinterpret_cast](https://zh.cppreference.com/w/cpp/language/reinterpret_cast)转型为其第一个非静态数据成员,又由于其特性,导致其基类必为不含非静态数据成员(即为空),从而必须要求使用空基类优化

## (2.2)

优化的条件即为,基类为空,且基类不存在vtp而且其派生类的第一个非静态数据成员不为当前继承的空基类,那么就会发生优化

## (2.3)

注意,优化仅发生在以上条件,在其他位置(非第一个非静态成员),存在空基类数据成员时,不发生优化,遵循唯一地址和内存对齐规定

## (2.4)

在C++20起存在属性no_unique_address即不使用地址位置,那么此时不管空类对象在那个位置,都将被优化掉.

------

例子:



```cpp
#include<iostream>
class Base{
    constexpr static int i = 4;
};
class C1:Base{
    int i = 0;
};
class C2{
    int i = 0;
    Base foo;
    //C++20 可以使用[[no-unique_address]]Base foo;    此时,sizeof 就为4.
};
class C3:Base{
    Base foo;
    int i = 0;
};
int main(){
    using std::cout;
    cout<< sizeof(Base)<<" "<<sizeof(C1)<<" "<<sizeof(C2)<<" "<<sizeof(C3)<<std::endl;
    return 0;
}
```