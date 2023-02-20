/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <functional>

using namespace std;

// std::function是可调用对象包装器
// 他是c++11新标准引入的类模板
// 注意他的类模板参数<>，void(int)，它不像函数指针一样，后面还有一个()，第一个是返回类型，第二个是参数类型，也就是说这里不需要可调用对象名

// 函数名，函数指针，函数引用，以及具有operator()成员函数的类对象以及lambda表达式都是可调用对象


// 通过范例可以感受到std::function的强大能力，接收各种可调用对象并进行实际调用

namespace _nmsp1  
{
    void myfunc(int tmpvalue)   // 普通函数
    {
        std::cout << "myfunc执行了， tmpvalue = " << tmpvalue << std::endl;
    }
    
    void callObjFunc(std::function<void(int)> cobj)
    {
        cobj(190);
    }
    
    void func()
    {
        callObjFunc(myfunc);
        // myfunc执行了， tmpvalue = 190
        callObjFunc([](int tmpvalue){
            std::cout << "lambda表达式做可调用对象, tmpvalue = " << tmpvalue << std::endl;
        });
        // lambda表达式做可调用对象, tmpvalue = 190
    }
}

namespace _nmsp2
{
    // 为了保存 CallFuncObj 类模板 中传递进来的可掉用对象
    // 这里需要引入一个CFObjHandler类模板，来创建可调用对象处理器
    // 可调用对象处理器 CFObjHandler 
    // 注意，该类模板的模板参数与CallFuncObj特化版本的模板参数是一样的
    template<typename T, typename... Args>
    class CFObjHandler
    {
    public:
        virtual T invoke(Args... args) const {};    // 虚函数，后续创建子类
    };
    
    // 该子类对于父类模板来说，增加了一个类型模板参数U，
    template<typename U, typename T, typename... Args>
    class CFObjHandlerChild : public CFObjHandler<T, Args...>
    {
    public:
    // 构造函数
    // 注意这里这个U不是万能引用，只有在函数模板中，才有万能引用，这里是右值引用做形参
    // 那这里为什么要右值引用？
    // 如果我们这里只对普通函数类型的可调用对象进行包装，那么这里使用左值引用和右值引用是没区别的
    // 但是如果还需要对lambda表达式进行可调用对象包装， 那么这里如果是左值引用，就会产生编译错误
        CFObjHandlerChild(U&& tmpfuncObj):functor(std::forward<U>(tmpfuncObj))
        {
            
        }
        
        virtual T invoke(Args... args) const    // 调用可调用对象
        {
            return functor(std::forward<Args>(args)...);
        }
        
    private:
        U functor;  // U 是一个可调用对象类型，functor用于保存可调用对象
    
    };
    
    
    // CallFuncObj 类模板，实现类似std::function的功能
    // 泛化版本 (这里因为不需要用到泛化版本，所以只进行了定义，没有进行实现)
    template<typename T>
    class CallFuncObj;
    
    // 特化版本
    // T代表的是可调用对象的返回值类型， Args代表的是给可调用对象提供一包参数
    // 为了能让CallFuncObj能够接收任意类型的可调用对象，我们需要给CallFuncObj增加一个public修饰的构造函数模板
    template<typename T, typename... Args>
    class CallFuncObj<T(Args...)>
    {
    public:
        // 构造函数模板
        template<typename U>
        CallFuncObj(U&& acobj)  // 可以接收各种可调用对象，函数对象，lambda表达式等
        {
            // CallFuncObj<void(double)> fn0 = funcDouble; 
            // 对于外面func函数中的这行调用，U的类型是void(&)(double)
            
            // 如果传递的不是普通的函数引用或者函数，而是传递一个lambda表达式来创建类对象的时候，
            // 那么对于lambda表达式的理解，就是传递进来了一种类类型，
            // 类类型的临时对象我们知道是右值引用类型
            // 所以你把一个右值引用传递给CFObjHandlerChild，如果CFObjHandlerChild里构造函数模板的U是左值引用&
            // 那么就相当于你把一个右值引用传递给了一个左值引用类型，类型就会不匹配
            
            
            // 该构造函数模板的形参，他是U&& 类型（可调用对象引用），然后这里acobj就是可调用对象
            // 那么这个可调用对象是需要保存起来的，因为后续再operator()中是需要调用该可调用对象的
        
            // 这个类型信息U在这里肯定是需要传递到CFObjHandler中去的，但是目前CFObjHandler的模板参数中只有T和Args信息
            // 并没有U这个消息
            // 权衡之下，决定创建一个CFObjHandler的子类模板。在子类模板中新增一个模板参数来传递这个类型参数U
        
            handler = new CFObjHandlerChild<U, T, Args...>(std::forward<U>(acobj));
            // 注意这里new了，就得提供析构函数进行释放
        }
        
        ~CallFuncObj()
        {
            delete handler;
        }
        
    public:
    // 重载圆括号，实现该类对象的可调用
        T operator()(Args... args) const
        {
            return handler->invoke(std::forward<Args>(args)...);
            // 这里使用了std::forward<>完美转发，以保持原参数的左值或者右值性
        }
    
    private:
        CFObjHandler<T, Args...> *handler;  // 可调用对象处理器
    
    };
    
    void callObjFunc2(CallFuncObj<void(int)> cobj)
    {
        cobj(1999);
        // 这种写法表示cobj是一个可调用对象，那么对于这种类对象做可调用对象的情况，就需要给
        // CallFuncObj新增一个operator()成员
        // 因为这行一调用，执行的就是该类模板的operator()成员函数
    }
    
    void myfunc(int tmpvalue)   // 普通函数
    {
        std::cout << "myfunc执行了， tmpvalue = " << tmpvalue << std::endl;
    }
    
    void funcDouble(double db)
    {
        std::cout << "funcDouble执行了， db = " << db << std::endl;
    }
    
    void func()
    {
        callObjFunc2(myfunc);
        // myfunc执行了， tmpvalue = 1999
        
        callObjFunc2([](int tempvalue){
            std::cout << "lambda表达式做可调用对象， tempvalue = " << tempvalue
            << std::endl;
        });
        // lambda表达式做可调用对象， tempvalue = 1999
        
        CallFuncObj<void(std::string)> str1 = [](std::string str){
            std::cout << "std::string形参类型的lambda表达式， str = " << str << std::endl;
        };
        
        str1("yangxinyu");
        // std::string形参类型的lambda表达式， str = yangxinyu
        
        CallFuncObj<void(double)> fn0 = funcDouble;
        // 针对本行代码，是需要创建CallFuncObj<void(double)>类型的对象的
        // 那么在执行该类模板CallFuncObj的构造函数模板的时候，U的类型是void(&)(double)
        // 左值引用类型（函数引用）
        // 然后使用功能std::forward<>传递给CFObjHandlerChild的时候，就仍然是左值类型
        // handler = new CFObjHandlerChild<U, T, Args...>(std::forward<U>(acobj));
        // 
        funcDouble(2342.32);
        // funcDouble执行了， db = 2342.32
        
        
    }
}

int main()
{
    // _nmsp1::func();
    
    _nmsp2::func();

    return 0;
}
