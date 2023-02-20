# C++类中的枚举类型

​    在看effective c++的时候，其中第二条边指出。尽量使用const ,enum代替define。在写程序的时候，需要入参为设备类型，第一反应是枚举一个设备类型，并以名字命名。但是有一个问题挺困惑的，类中的枚举到底是以什么形式存在的？枚举变量需不需要分配内存？

  带着这个问题，咱们来做一个实验。

```c++
class test
{
public:
	enum type{One,Two, Three};
private:

};

int _tmain(int argc, _TCHAR* argv[])
{
	cout << sizeof(test)<<endl;
	return 0;
}
```

输出：1

​    想想其实也很容易明白，此时就是在类的内部声明了一个枚举类型。并没有声明这个枚举类型的变量。此时如果加上一个枚举类型的变量的声明，再来测试一下这个类的大小。其中枚举为公有的，我们试着用 类::enum值 调用该枚举值，看其是否能够被调用。

```c++
class test
{
public:
	enum type{One=2,Two, Three};
	test():t(One)
	{
	
	}
	type getT()
	{
		return t;
	}
private:
	type t;
};

int _tmain(int argc, _TCHAR* argv[])
{
	test s;
	cout << sizeof(test)<<endl;
	cout<<s.getT()<<endl;
	cout << test::Three<<endl;
	return 0;
}
```

输出: 4 2 2

此时类的大小是一个int型变量的大小。取得的枚举是一个整形。可以通过类域访问到枚举值。为什么通过 类::enum值可以访问枚举值？

  因为该枚举是一个常量，在编译的时候已经被放入了常量区。调用的时候因此不需要该枚举的变量也可以调用。

注意：枚举类型不具备名字空间的作用。在一个作用域中在，如果两个枚举类型的值相同，则会编译出错。

```c++
  struct CEType
           {
               enum EType1 { e1, e2 };
               enum EType2 { e1, e2 };
           };
```

e1,e2名字相同程序会报错。