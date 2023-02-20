# 奇异递归模板模式(Curiously Recurring Template Pattern)

本篇短文将简短的介绍奇异递归模板模式(Curiously Recurring Template Pattern，**CRTP**)，CRTP是C++模板编程时的一种惯用法（idiom）：把派生类作为基类的模板参数。更一般地被称作**F-bound polymorphism。**

1980年代作为F-bound polymorphism被提出。Jim Coplien于1995年称之为CRTP。

CRTP在C++中主要有两种用途：

- **静态多态（static polymorphism）**
- **添加方法同时精简代码**

## **1.静态多态**

先看一个简单的例子：

```cpp
#include <iostream>
using namespace std;

template <typename Child>
struct Base
{
	void interface()
	{
		static_cast<Child*>(this)->implementation();
	}
};

struct Derived : Base<Derived>
{
	void implementation()
	{
		cerr << "Derived implementation\n";
	}
};

int main()
{
	Derived d;
	d.interface();  // Prints "Derived implementation"

	return 0;
}
```

这里基类Base为模板类，子类Drived继承自Base同时模板参数为Drived，基类中有接口interface而子类中则有接口对应实现implementation，基类interface中将this通过static_cast转换为模板参数类型，并调用该类型的implemention方法。由于Drived继承基类时的模板为Drived类型所以在static_cast时会转换为Drived并调用Drived的implemention方法。*(注意这里采用的时static_cast而不是dynamic_cast，因为只有继承了Base的类型才能调用interface且这里是向下转型，所以采用static_cast是安全的。*)

通过CRTP可以使得类具有类似于虚函数的效果，同时又没有虚函数调用时的开销(虚函数调用需要通过虚函数指针查找虚函数表进行调用)，同时类的对象的体积相比使用虚函数也会减少(不需要存储虚函数指针)，但是缺点是无法动态绑定。

下面是关于静态多态的第二个例子：

```cpp
template<typename Child>
class Animal
{
public:
	void Run()
	{
		static_cast<Child*>(this)->Run();
	}
};

class Dog :public Animal<Dog>
{
public:
	void Run()
	{
		cout << "Dog Run" << endl;
	}
};

class Cat :public Animal<Cat>
{
public:
	void Run()
	{
		cout << "Cat Run" << endl;
	}
};

template<typename T>
void Action(Animal<T> &animal)
{
	animal.Run();
}

int main()
{
	Dog dog;
	Action(dog);

	Cat cat;
	Action(cat);
	return 0;
}
```

这里Dog继承自Animal且模板参数为Dog，Cat继承自Animal且模板参数为Cat，Animal，Dog，Cat中都声明了Run，而Animal中的Run是通过类型转换后调用模板类型的Run方法实现的。在Action模板函数中接收Animal类型的引用(或指针)并在其中调用了animal对象的Run方法，由于这里传入的是不同的子类对象，因此Action中的animal也会有不同的行为。

## **2.添加方法，减少冗余**

假设现在我们需要实现一个数学运算库，我们需要支持Vector2，Vector3，Vector4...等类型，如果我们将每个类分别声明并实现如下：

```cpp
//Vec3
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3() = default;

	Vector3(float _x, float _y, float _z);

	inline Vector3& operator+=(const Vector3& rhs);
	inline Vector3& operator-=(const Vector3& rhs);
	//....
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
//....

//Vec2
struct Vector2
{
	float x;
	float y;

	Vector2() = default;

	Vector2(float _x, float _y);

	inline Vector2& operator+=(const Vector2& rhs);
	inline Vector2& operator-=(const Vector2& rhs);
	//....
};

inline Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
inline Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
//....
```

我们会发现需要为每个类型都实现+=, -= ,++ , -- , + , -等运算符重载，而且每个类型的一些运算符，行为都很类似，而且可以使用其他的运算符进行实现，比如+=, -=, ++, --都可以采用+，-运算符进行实现。这时我们就可以采用CRTP抽离出这些共同的类似方法，减少代码的冗余：

```cpp
template<typename T>
struct VectorBase
{
	T& underlying() { return static_cast<T&>(*this); }
	T const& underlying() const { return static_cast<T const&>(*this); }

	inline T& operator+=(const T& rhs) 
	{ 
		this->underlying() = this->underlying() + rhs;
		return this->underlying();
	}

	inline T& operator-=(const T& rhs)
	{
		this->underlying() = this->underlying() - rhs;
		return this->underlying();
	}
	
	//.....
};

struct Vector3 : public VectorBase<Vector3>
{
	float x;
	float y;
	float z;

	Vector3() = default;

	Vector3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

inline Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	Vector3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

inline Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	Vector3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}
//......

int main()
{
	Vector3 v0(6.0f, 5.0f, 4.0f);
	Vector3 v2(4.0f, 5.0f, 6.0f);

	v0 += v2;
	v0 -= v2;

	return 0;
}
```

通过把+=， -=等操作放到基类中并采用+ ,-运算符实现，这样一来所有继承自VectorBase的类，只要其定义了+，-运算符就可以自动获得+=, -=等运算符，这样大大的减少了代码中的冗余。

在有多个类型存在相同方法，且这些方法可以借助于类的其他方法进行实现时，均可以采用CRTP进行精简代码。

参考：

1.[The Curiously Recurring Template Pattern (CRTP)](https://link.zhihu.com/?target=https%3A//www.fluentcpp.com/2017/05/12/curiously-recurring-template-pattern/)

2.[The cost of dynamic (virtual calls) vs. static (CRTP) dispatch in C++](https://link.zhihu.com/?target=https%3A//eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c)

3.[https://zh.wikipedia.org/wiki/%E5%A5%87%E5%BC%82%E9%80%92%E5%BD%92%E6%A8%A1%E6%9D%BF%E6%A8%A1%E5%BC%8F](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/%E5%A5%87%E5%BC%82%E9%80%92%E5%BD%92%E6%A8%A1%E6%9D%BF%E6%A8%A1%E5%BC%8F)





**PS:附加std::enable_shared_from_this相关内容**

假如在c++中想要在一个已被shareptr管理的类型对象内获取并返回this，为了防止被管理的对象已被智能指针释放，而导致this成为悬空指针，可能会考虑以share_ptr的形式返回this指针，代码实现如下：

```cpp
struct Bad
{
    // 错误写法：用不安全的表达式试图获得 this 的 shared_ptr 对象
    std::shared_ptr<Bad> getptr() {
        return std::shared_ptr<Bad>(this);
    }
    ~Bad() { std::cout << "Bad::~Bad() called\n"; }
};

int main()
{
	{
		// 错误的示例，每个 shared_ptr 都认为自己是对象仅有的所有者
		std::shared_ptr<Bad> bp1 = std::make_shared<Bad>();
		std::shared_ptr<Bad> bp2 = bp1->getptr();
		std::cout << "bp2.use_count() = " << bp2.use_count() << '\n';
	}

	return 0;
}
```

但是上面的写法是**完全错误**的，因为share_ptr内部存储了两个指针，一个指向被管理对象，另一个指向控制块。控制块内存有删除器，占有被管理对象的 `shared_ptr` 的数量，涉及被管理对象的 `weak_ptr` 的数量等信息，一旦占有被管理对象的shared_ptr的数量减少至0，被管理的对象就会通过删除器被释放（控制块会等到weakptr计数器也清0时才会释放）。上面的代码中由于在返回this的sharedptr时，又通过this指针构造了一个shared_ptr，这样就会导致有两个shared_ptr通过不同的控制块，管理相同的对象。一旦其中一个shared_ptr释放了所管理的对象，那么另一个shared_ptr将会变成非法的。

而正确的写法应该是让需要返回this指针的类，继承std::enable_shared_from_this模板类，同时模板参数为该类的类型：

```cpp
struct Good: std::enable_shared_from_this<Good> // 注意：继承
{
    std::shared_ptr<Good> getptr() {
        return shared_from_this();
    }
};
```

至于为什么要这样做，可以参见以下的伪代码：

```cpp
template<class D>
class enable_shared_from_this {
protected:
    constexpr enable_shared_from_this() { }
    enable_shared_from_this(enable_shared_from_this const&) { }
    enable_shared_from_this& operator=(enable_shared_from_this const&) {
        return *this;
    }

public:
    shared_ptr<T> shared_from_this() { return self_; }
    shared_ptr<T const> shared_from_this() const { return self_; }

private:
    weak_ptr<D> self_;

    friend shared_ptr<D>;
};

template<typename T>
shared_ptr<T>::shared_ptr(T* ptr) {
    // ...
    // Code that creates control block goes here.
    // ...

    // NOTE: This if check is pseudo-code. Won't compile. There's a few
    // issues not being taken in to account that would make this example
    // rather noisy.
    if (is_base_of<enable_shared_from_this<T>, T>::value) {
        enable_shared_from_this<T>& base = *ptr;
        base.self_ = *this;
    }
}
```

enable_shared_from_this类使用了CRTP的写法，类中存储了一个weak_ptr<T>，使用这个weak_ptr<t>初始化shared_ptr<T>时，如果这个类型是否继承自enable_shared_from_this，则会使shared_from_this中构造的shared_ptr<T>共享weak_ptr<T>指向的对象的所有权(即从weak_ptr构造的shared_ptr和构造weak_ptr的shared_ptr共享控制块)([std::shared_ptr::shared_ptr - cppreference.com](https://link.zhihu.com/?target=https%3A//zh.cppreference.com/w/cpp/memory/shared_ptr/shared_ptr) 参见构造函数11)，这样就可以保证shared_from_this返回的shared_ptr的内存安全，不会像第一个例子那样出现悬空指针。

(enable_shared_from_this中的weak_ptr是通过shared_ptr的构造函数初始化的，所以必须在shared_ptr构造函数调用之后才能调用shared_from_this，而且不能对一个没有被shared_ptr接管的类调用shared_from_this，否则会产生未定义行为)