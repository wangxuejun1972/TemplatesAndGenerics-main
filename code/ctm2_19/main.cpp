/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
C#, VB, Perl, Swift, Prolog, Javascript, Pascal, HTML, CSS, JS
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 混入（mixins）
// 是一种编程手法，用于描述类与类之间的关系，这种关系类似于多重继承，
// 但是看起来更像是颠倒过来的继承（基类继承派生类这种感觉）
// 混入的实现手段：把传入的模板参数单做类模板的父类

// 常规范例
// 引入混入手段来取代传统继承，这种混入手段看起来更像是把某些类混合到当前类中凑成一个更大的类

// 通过混入技术，可以方便的组合，自由的装配各种功能


// 用参数化的方式表达成员函数的虚拟性
// 所以Base类中myFunc的虚拟性可以通过指定Base的模板参数，然后根据参数类型里是否有同名虚函数来进行体现
// 所以其实可以看到：这种参数化表达成员函数的虚拟性并不是一种技术实现，而是一种设计理念
// 它体现的是开发者超越代码之上的一种开发智慧

// 一般来讲，如果一个类做父类，那么他应该有析构函数，并且这个析构函数一般都应该是虚析构
// 什么情况下父类可以没有析构函数，或者析构函数可以不为虚析构
// 首先明确一点，父类中有虚析构函数，往往是为了调用子类的析构函数
// 1）如果子类中没有析构函数（不需要再析构函数中释放任何new出来的数据）
// 2）代码中不会出现父类指针指向子类对象（多态）的情形

// 为了防止用父类指针指向子类对象，如何在代码层面来进行禁止呢？
// 为了防止用父类指针来new一个子类对象，可以把父类的析构函数用protected来修饰
// 这样你就能发现，虽然你可以用父类指针来new子类对象，但是你在delete的时候并不能delete
// 因为delete是要调用析构函数的，析构函数是protected，自然不能被调用
// 可以参考Boost库中的noncopyable类

namespace _nmsp1
{
    // 设计一个角色（role）类，代表玩家，有攻击力，防御力，血量这些数值
    class Role
    {
    
    public:
        // 构造函数
        Role():m_attack(0), m_defence(0), m_life(100) {}
        Role(double att, double def, double lif):m_attack(att), m_defence(def), m_life(lif) {}
        
    public:
        double m_attack;    // 攻击力
        double m_defence;   // 防御力
        double m_life;      // 血量
    };
    
    
    // 设计一个家族（宗门）类，里面包含若干角色
    // class Family
    // {
    // public:
    //     std::vector<Role> m_members;
    // };
    
    // 定义 NPC 分类： 0 士兵 1 商人 2 任务
    
    // 家族类改为类模板
    template<typename T>
    class Family
    {
    public:
        std::vector<T> m_members;
    };
    
    // NPC属性类
    struct npcattr
    {
        int m_sort; // NPC 分类： 0 士兵 1 商人 2 任务
        std::string m_lang; // npc自言自语
    };
    
    // NPC类
    class Role_NPC : public Role
    {
    public:
        // 构造函数
        Role_NPC() : Role(), m_sructAttr{0, ""} {}
        Role_NPC(double att, double def, double lif, int sort, std::string lang) 
        : Role(att, def, lif), m_sructAttr{sort, lang} {}
        
    public:
        npcattr m_sructAttr;
    };
    
    // 玩家属性系统 力量 敏捷 体质
    // 没升一级可以得到10个属性点， 可以把这些属性点加到对应的属性上去
    // 最终目的就是提高玩家角色的攻击力 防御力 生命力
    // 每往力量上加一点属性点， 攻击力  + 1.2
    // 每往敏捷上加一点属性点， 攻击力  + 2.5
    // 每往体质上加一点属性点， 攻击力  + 10.9
    
    // 引入玩家属性类
    struct playerattr
    {
        int m_strength; // 力量
        int m_agile;    // 敏捷
        int m_constitution; // 体质
    };
    
    // 玩家类
    class Role_play : public Role
    {
    public:
        Role_play() : Role(), m_structAttr{0, 0, 0} {}
        Role_play(double att, double def, double lif, int stg, int agi, int ctt)
        : Role(att, def, lif), m_structAttr{stg, agi, ctt} {}
        
    public:
        playerattr m_structAttr;
        
    };
    
    
    
    void func()
    {
        
    }
}

namespace _nmsp2
{
    // 改造为mixins混入
    
    
    
    
    // 定义 NPC 分类： 0 士兵 1 商人 2 任务
    
    // NPC属性类
    struct npcattr
    {
        int m_sort; // NPC 分类： 0 士兵 1 商人 2 任务
        std::string m_lang; // npc自言自语
    };
    
    // 玩家属性系统 力量 敏捷 体质
    // 没升一级可以得到10个属性点， 可以把这些属性点加到对应的属性上去
    // 最终目的就是提高玩家角色的攻击力 防御力 生命力
    // 每往力量上加一点属性点， 攻击力  + 1.2
    // 每往敏捷上加一点属性点， 攻击力  + 2.5
    // 每往体质上加一点属性点， 攻击力  + 10.9
    
    // 引入玩家属性类
    struct playerattr
    {
        int m_strength; // 力量
        int m_agile;    // 敏捷
        int m_constitution; // 体质
    };
    
    
    // 设计一个角色（role）类，代表玩家，有攻击力，防御力，血量这些数值
    template<typename... T>
    class Role : public T...    // 把传入的模板单做该类模板的父类
    {
    public:
        // 构造函数
        Role(): T()..., m_attack(0), m_defence(0), m_life(100) {}
        Role(double att, double def, double lif):T()..., m_attack(att), m_defence(def), m_life(lif) {}
        
    public:
        double m_attack;    // 攻击力
        double m_defence;   // 防御力
        double m_life;      // 血量
    };
    
    
    // 家族类改为类模板
    // 这里也需要做一个调整，因为现在这里装载的是mixins混入的类型，所以这里模板参数该为可变参了类模板
    // 支持可变参类型
    template<typename... T>
    class Family
    {
    public:
        std::vector<Role<T...>> m_members;
    };
    
    // 有了Role这个可变参类模板，我们就可以通过混入的方式，把Role_NPC 和 Role_play 创造出来
    // 而不再需要继承原来的那个Role类，那种原始的继承方法来创建这两个新类
    
    using Role_NPC = Role<npcattr>;
    // Role_NPC 混入给人的感觉 class Role_NPC:public Role, public npcattr
    
    using Role_Play = Role<playerattr>;
    // Role_Play 混入给人的感觉 class Role_Play : public Role, public playerattr
    
    // 如果将来有业务需求新增一个既有npc属性又有玩家属性的角色, 可以直接这样进行混入
    using Role_mix = Role<npcattr, playerattr>;
    
    // 定义一个npc家族 (他这里面装的是什么呢？)
    // 装的是Role<npcattr>
    // 所以这就是为什么这里传递进去的类型参数要是 npcattr
    using Family_NPC = Family<npcattr>;
    
    
    
    void func()
    {
        Role_NPC npc1;
        npc1.m_attack = 15;
        npc1.m_defence = 10;
        npc1.m_life = 120;
        npc1.m_sort = 1;
        npc1.m_lang = "Are You OK?";
        
        
        Family_NPC npf1;
        npf1.m_members.push_back(npc1);
    }
}

namespace _nmsp3
{
    // 写一个可变参类模板，使用混入手法
    template<typename... T>
    class Base:public T...
    {
    public:
        void myFunc()
        {
            std::cout << "Base::myFunc()执行了" << std::endl;
        }
    };
    
    // 这个范例很简单，唯一需要注意的就是myFunc这个普通成员函数
    // 他看起来像是一个普通成员函数，光这么看的话看不出来他是一个虚成员函数
    // 但是这里面的关键问题是，Base是一个可变参类模板，他是带T这个可变参的
    // 这个模板参数T代表的是一包类型，
    // 并且由于这里使用了混入技术，所以T代表的这一包类型都会成为Base类模板的父类
    // 这就意味着一旦这一包类型中，有一个类包含了一个也叫myFunc的同名同参的虚函数的话
    // 那么显然这个子类，也就是这个Base类模板中这个myFunc就会变成虚函数
    // 也就是即使Base中的这个myFunc不是虚函数，只要他的父类中有一个同名同参的虚函数的对话，
    // 那么子类这个myFunc就会自动继承了父类那个virtual虚函数
    
    
    // 再创建一个类模板，作为Base的子类
    template<typename... T>
    class Derived:public Base<T...>
    {
    public:
        void myFunc()
        {
            std::cout << "Derived::myFunc()执行了" << std::endl;
        }
    };
    
    // Base这个类模板和Derived这个子类模板，他们都有myFunc这个成员函数，
    // 根据C++对象模型中提到的虚函数的特点，如果父类指针指向子类对象的时候，
    // 如果父类Base类的myFunc他是一个虚函数的话，那么我们用父类指针调用myFunc的时候
    // 调用的是子类的myFunc
    // 如果父类中的myFunc不是一个虚函数而是一个普通的函数，那么父类指针调用myFunc的时候
    // 执行的必然就是父类本身的myFunc
    // 如果一个函数在父类中是虚函数，即便你在子类中不用virtual修饰，他在子类中也依然是虚函数
    
    
    class A
    {
        
    };
    
    class Avir
    {
    public:
        virtual void myFunc()
        {
            std::cout << "Avir::myFunc()执行了" << std::endl;
        }
    };
    
    void func()
    {
        Base<A> *ba1 = new Derived<A>(); // 父类指针指向子类对象
        ba1->myFunc();
        // Base::myFunc()执行了
        // 因为A中没有虚函数，B中myFunc也不是虚函数，所以调用的是父类自己的myFunc
        
        Base<Avir> *ba2 = new Derived<Avir>();  // 父类指针指向子类对象
        ba2->myFunc();
        // Derived::myFunc()执行了
        // 因为有虚函数的存在，所以实际执行的是子类的myFunc函数
    }
    
}

namespace _nmsp4
{
    
    class A
    {
    protected:
        ~A()
        {
            
        }
    };
    
    class B : public A
    {};
    
    
    void func()
    {
        A *pa = new B();
        
        delete pa;
        // ‘_nmsp4::A::~A()’ is protected within this context
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
