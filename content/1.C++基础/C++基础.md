# C++基础

## 指针与内存
C++中不可避免会用到指针，指针和内存是密不可分的，所以理解指针也要理解内存；
在本篇讲到的指针均为原始指针（即和C用到的指针相同），而不是智能指针，智能指针会放到标准库中进行学习
先下一个结论：
**指针就是一个地址，一个由整数组成表示内存的地址，不管它指针的类型，它都是一个整数，都是一个地址**
```C++
int main(){
    int val = 8;
    //&是一个解地址符，意思是向操作系统询问变量val的地址；
    //然后把这个地址给了ptr
    //注意，这里只是吧ptr声明成了void*类型，这样编译是可以通过的
    //因为，上面的结论：指针只是一个整数而已，跟它指向的类型无关；
    //后面会解释，为什么我们需要告诉编译器指向的内存的类型
    void* ptr = &val

    //*是取地址符，即通过*可以拿到指针指向的内存
    //当我们试图去访问这个指针指向的内存的时候，我们会发现出错了
    //LOG(*ptr)         //erro，为什么会出错，我们继续往下看

    //刚才我们试图去取指针指向的内存时，出错了；
    //因为编译器不知道在取内存的时候应该取多大的，C++中有很多数据类型，例如：
    //char-1个字节；short-2个字节；int-4个字节；double-8个字节
    //在取地址的时候应该取多少呢？编译器不知道，所以我们需要告诉编译器，这个指针指向的内存有多大
    int* ptr2 = &val;
    LOG(*ptr2)  //成功打印出8，也就是val的值
}
```
从上面的例子，可以明白指针有两个非常重要的部分，需要我们告诉编译器
1. 指针指向地址：需要告诉编译器该指针指向的地址，就像网购买东西，需要告诉卖家邮寄的地址，才能将商品邮寄到家一样
2. 指针指向内存的大小：还需要告诉编译器指向的内存大小，即指向的数据类型是什么。**因为在将来需要用指针访问数据时，编译器才能知道每次取得的内存大小是多少。**

总结：
指针可以理解为一个整数变量(int)，该整数存储的是内存中的地址；在声明时，可以不明确指针的类型（告诉编译器指向的内存大小）和指向地址（整数的值），但是在用到指针访问内存的时候，如果这两个部分缺少一个就会产生错误；一般来说，指针类型不明确可以在编译阶段发现，但是指针指向的地址不赋值却使用指针一般在运行时出现，这就是大名鼎鼎的段错误，还有一种说法是访问了野指针（野指针可以理解为指针指向了不应该访问的内存，例如nullptr或者刚刚释放的内存空间）。

## 引用
引用是C++特有的（相比于C语言）
直接下结论：
**引用是变量的一个别名，其底层可以理解为一种指针**
引用常用在函数的形参和实参传递中：
```C++
void Increment1(int value){value++;}
void Increment2(int* value){ (*value)++; }
void Increment3(int& value){value++;}

int main(){
    //Increment让传入的value+1；上述一共三个，分别是值传递，指针传递和引用传递
    int value1 = 5;
    int value2 = 5;
    int value3 = 5;
    //显然值传递不会让value1改变
    Increment1(value1);
    //指针传递后，value2=6；
    Increment2(&value2);
    //引用传递后，value3=6；引用和指针差不多，但是使用起来引用更像是一种别名
    //引用的使用场景更多发生在函数形参的部分，可以通过传递引用的方式来改变栈上的变量
    Increment3(value3);

    LOG(value1);
    LOG(value2);
    LOG(value3);
}
```
**函数返回值用引用，会提升运行效率**，但什么情况下不可以返回值用引用
**不可以传递局部变量的引用！！！！**这一点很好理解，就像不可以返回局部变量的指针一样。
```C++
inline complex comple::add(const comple& c1, const comple& c2){
    comple c3;
    c3 = c1+c2; //假设已经重载了+操作符
    return c3;  //此时不能返回引用，因为c3是在函数中被声明和赋值的，函数结束后，该变量会被释放掉
}

inline complex& complex::add2(complex& ths, const complex& c2){
    ths.re += c2.re;
    ths.im += c2.im;
    return *ths;    //这里可以返回引用，因为ths是外面已经定义的变量，我们在这个变量基础上进行改变
    //此时返回引用效率更高
}
```
pass by value和pass by reference，函数在声明时，形参并不能同时存在value形式和reference形式
如果想要改变这个情况，可以选择在函数后加const（意味着，不改变成员变量的值），因为const是函数签名的一部分，而返回值不是。
```C++
//下面两个函数不能同时存在，因为这两个函数在编译器的函数签名相同
double imag(const double& im){}
double imag(const double im){}
```

总结：
引用可以认为是变量的别名，改变引用就是改变其代表的变量；
引用相比于指针，显得不那么灵活，例如：
1. 引用必须初始化（必须告诉编译器该引用是什么，`int &ref`这样是编译不过的）
2. 引用不能改变代表的变量，可以理解其底层是一个const point，必须指向某块内存

## C++中的static关键字
static关键在C++中，可分为两种情况：
1. static在class（struct）内
在class内static声明的变量，该变量会存储到静态内存区（不算在实例内存中），即所有实例共享这一个变量实体（甚至不需要将类实例化也可以访问该变量）
static修饰成员函数，会使得该函数可以不通过实例化可以调用，但static函数只能访问static变量（是因为static函数没有被编译器传入this指针）
**static成员变量，在需要所有实例共享一份内存时，是非常有用的**

2. static在class（struct）外
接着，来看static在class外的情况，这一点和C语言很像，先说结论：
**static在class外声明变量或函数后，会使得编译器在编译这个cpp文件时仅把该变量存放在此cpp文件产生的中间文件中，说人话即该变量或函数仅对当前cpp文件可见**
补充：
- 在多个cpp文件编程中，变量声明成全局变量，在别的cpp文件如果需要使用，需要带上extern声明，提示编译器有这样一个在外面定义的变量
- 多cpp文件编程中，函数为全局函数，在别的cpp文件如果需要使用，直接声明该函数即可，extern关键字可省略
- 最后，全局变量不太好，尽量少用。static关键字的使用原则，类似于class中private的原则
- 在类中声明了static变量，在给变量初值时，需要在class外面赋值，如下：
```C++
class Account{
public:
static double m_rate;
static void set_rate(const double& x){m_rate = x;}
};
//这里m_rate需要在class外面赋值（定义或者叫初始化）
//给不给初值都是可以的
double Account::m_rate = 8.0;
```

## enum
enum是一个枚举类型，其中每个元素均是整数，默认从0开始；当需要用到整数，但其值有具体含义时，可以用enum增加代码的可读性。
在C++中enum可以定义在class中，这使得该enum称为该class命名空间的专属，即解决了C语言中enum枚举无法重复的痛点。
具体用法，在./code/enum.cpp。enum在实际编程中，随处可见，用法必须掌握
```C++
class Log{
public:
    //可以指定enum中每个元素的大小，使用unsinged char可以减小内存使用
    enum Level : unsinged char
    {
        LevelError
    };
};
```
## C++的inline函数
inline(内联函数)可以某种程度代替宏的用法。**由编译器决定，添加了inline关键字的函数可以在编译阶段展开，降低调用函数的开销**
这里的展开是由编译器决定的，至于编译器要不要展开，可以简单理解为简单的函数，可以展开，复杂的函数无法展开。
另外，inline函数可以在调用时进行类型检查，而宏只是简单的文本展开，因此inline在大多时候还是比宏好用的。
**如果在class内定义成员函数，编译器会视这些函数为inline！！**


## C++的构造函数
在实例化对象时，会调用的一种初始化函数；该函数不会有返回值！
其他部分，比如默认实参都和普通函数一样。
特殊部分：
1. initialization list 列表初始化，推荐一定要用到这样的特性
2. 在继承关系下，调用子类的构造函数，会默认调用父类的构造函数
3. 构造函数可以重载，重载后编译器会为每个构造函数生成唯一的函数标识符，但重载的构造函数不能产生歧义，如下：
```C++
    complex(double r = 0, double i = 0) : re(r), im(i){}
    //下面的构造函数和上面的构造函数会发生歧义，编译器会提示错误
    complex() : re(0), im(0) {}
```
这样两个构造函数，会产生歧义，在调用`complex c1`时会提示`call of overloaded ‘complex()’ is ambiguous`
4. 构造函数一般不会放入private区域，因为这样的话，外界不能创建对象；但在某些设计模式下，会这样做，例如单例模式，单例模式是在整个程序运行阶段只有一个实例存在，不允许对该对象进行多次实例化
```C++
class Singleton{
public:
    static Singleton& GetInstance(){
        static Singleton s;
        return s; 
    }
private:
    Singleton();
};
```
这样的代码，外界无法直接调用构造函数对Singleton进行实例化，仅能通过GetInstance方法得到实例。但在GetInstance中Singleton实例化成static，即不管调用多少次GetInstance，都只会有一份s在静态变量区，即满足了整个程序运行阶段只有一个实例存在。

特殊的构造函数：
- 拷贝构造函数（copy constructor）
    如果类中有指针，必须要手动实现拷贝构造函数
- 移动构造函数
    移动构造涉及到右值，**暂时不讨论**
- 拷贝复制函数（operator=）
    如果类中有指针，必须要手动实现拷贝复制函数(该函数不属于构造函数，但是还是在这里说明)
    如果没有手动写该函数，编译器会默认生成一个拷贝复制函数，但是关于指针的部分只会把指针的地址拷贝过来（浅拷贝），相当于两个实例中的指针指向了同一个内存，这会造成危险。例如，如果一个实例执行了析构函数，释放了内存，另一个实例中的指针，会变成野指针。
    所以需要手动写一个自己的拷贝复制函数，将原来实例的内存数据一一拷贝过来，生成一个新的指针（这个叫做深拷贝）。具体见mystring.cpp

## C++的析构函数
一般来说，没有指针的类可以不需要写析构函数，但是有指针的类需要写构造函数（因为指针成员数据，一般会指向堆上的空间，需要手动释放）


## C++的虚函数
虚函数是C++中面向对象


## C++的访问控制
C++中的访问控制：
在面向对象中的程序设计中会涉及到class中的访问控制，访问控制只是帮助程序员更好组织代码不涉及到运行效率等。
关键字public：
即公开的，可以通过实例访问数据或调用成员函数
关键字private：
即私人的，不可以通过实例访问数据或调用成员函数
**通常来说，数据部分是private，成员函数大部分是public**
class和struct的在访问控制上的区别：
class默认的区域是private
struct默认区域是public

友元函数：
可以在class中声明一个friend函数作为友元函数，友元函数可以访问私有成员变量和函数
同一个class的各个实例之间互为友元对象。


## C++的const关键字
const在C++中变得很复杂，const修饰指针，函数会发生不一样的效果。

- const修饰成员函数，变成常量成员函数，直接说结论：
const修饰成员函数后会告诉编译器该成员函数不会改变该类的数据。
**补充：当成员函数的const和非const版本都同时存在时，const object只能调用const版本，non-const object只能调用non-const 版本。**


## C++的操作符重载
在**成员函数的**操作符重载时，编译器会把操作符重载函数的原型自动加上一个this指针，例子`complex.cpp`中
```C++
//操作符重载
complex &complex::operator+=(const complex &r)
{
    return __doapl(this, r);
}
//编译器会将上述函数将翻译成：
//complex &complex::operator+=(this,const complex &r)
//this指针会指向调用这个函数的那个实例
```
注意到+=操作符重载在返回值上会继续返回`complex &`，此举是为了连续调用，例如
`c1 += c2 += c3`，这样的调用才能满足需求。

**非成员函数的**操作符重载时，如果是二元操作（例如+，-等），需要将两个对象都传递进去。
```C++
inline complex operator +(const complex& x, const complex& y){
    return complex(x.real() + y.real(), x.imag() + y.imag() );
}
```
上述代码是普通函数进行操作符重载
- 这种情况下**绝对不可以返回引用**，因为return返回的是一个临时变量，离开这个函数，变量会被释放，不能返回引用
- return后面的构造函数形式，会创建一个临时变量，这是一个右值。返回后，会继续调用**拷贝函数**，然后该临时变量的声明周期仅能持续到接受对象的拷贝函数结束。

### C++的转换函数
转换函数放在操作符重载这一节，应为转换函数和操作符重载非常相似
```C++
//操作符重载
complex operator+(const compldex& x);
//转换函数，具体用法见fraction.cpp
fraction::operator double() const;
```
可以，看出两者都是`operator`的重载，区别在以下几点：
1. 转换函数不需要写，也不能写返回了类型，返回类型已经在函数名上了。
2. 转换函数必须是类成员，不能是全局函数，因为转换的本体是类的实例，且编译器会帮我们隐式调用

### C++的仿函数
仿函数也放在操作符重载上，因为形式上仿函数和操作符重载很像
```C++
//操作符重载
complex operator+(const compldex& x);
//仿函数
template <class T>
struct idetity{
    const T& 
        operator()(const T& x)const {return x;}
};
```
**仿函数就是实现了operator()的的类或者结构体。**即function-like classes，像函数的类。这种仿函数的作用是在STL算法的使用过程中，需要用到，例如：STL中的sort函数，可以对容器的元素进行排序，默认是升序排列；但是我们需要降序排列，可以通过传入一个仿函数，告诉sort函数来降序排列
```C++
class A{
    bool operator(const int a, const int b)const {
        return a<b;
   }
};
int main(){
    vector<int> x = {8,4,2,5,23,6,3};
    //这里在sort传入的A，可以指定排序的规则
    //实际上sort的原型是:template <class RandomAccessIterator, class Compare>
    //void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp);
    //第三个参数是传递一个比较规则，可以是函数指针，也可以是仿函数
    sort(x.begin(), x.end(), A);
}
```

## C++的new和delete
new会先分配内存，再调用构造函数
```C++
Complex* pc = new Complex(1,2);
//可以拆分为以下几步：
void* mem = operator new(sizeof(Complex));  //先调用malloc分配内存
pc = static_cast<Complex*>(mem);            //强制类型转换为complex*
pc->Complex::Complex(1,2);                  //调用构造函数
```
delete会先调用析构函数，再释放内存
```C++
mystring* ps = new mystring("hello world!");
delete ps;  //delete 动态分配的内存
//可以拆分为以下两步
mystring::~mystring(ps);    //调用析构函数
operator delete(ps);        //再把内存释放，内部调用的free
```
**在new了数组后[],必须要用delete[ ]，才能完整释放数据的内存**

重载new和delete，new和delete可以分解为几个步骤，我们可以重载operator new或者delete，也可以对表达式new和delete进行重载。
重载new时，第一个参数一定要是size_t，delete的第一个参数需要是void*
见new.cpp
**new的重载很复杂，暂时没学明白，继续找资料学习**

## C++的继承
Inheritance(继承)，表示is-a，即类A是类B，就可以说A是B的一种，即A is-a B
继承的语法就不用说的太多了。
注意，继承包含三种，public、private和protect
public继承后，会继承所有的成员变量和成员函数，但是**private的**成员只能通过public的接口才能访问。
private继承后，
protect继承后，

继承情况下的构造和析构
- 构造时，由内而外
    Derived（子类）的构造函数先调用Base（父类）的默认构造函数，然后才执行自己的；注意，编译器会帮我们加代码，但是只能加默认的父类的构造函数，如果希望指定构造函数，需要自己像下方一样的格式书写
`Derived::Derived(...): Base(){...}`
- 析构时，由外而内
    Derived的析构函数首先执行自己，然后才调用父类的析构函数。
`Derived::~Derived(...){... ~Base();}`
**注意，父类的析构函数必须是虚函数！！！！！！**

### C++的虚函数
具体例子见shape.cpp
Template Method,父类的函数的流程中的某些步骤**声明成虚函数，延缓到子类去实现**，然后通过子类对象调用父类函数，父类函数中的关键步骤会根据调用的子类，来调用该子类的虚函数实现。
虚函数的实现：
C++的虚函数底层的代码实现是通过给类中加入一个指针(vptr)；指针会指向一个虚表（vtbl），虚表中存在的都是虚函数。虚表中存在这该类中的所有的虚函数，在调用时，会搜索该续表中指向的虚函数。
虚函数的调用过程如下：(*(p->vptr)[n])(p);n表示在声明函数时，编译器会根据我们书写的顺序对虚函数进行编号。

### C++动态绑定的三个条件
- 指针调用
    需要用指针调用函数
- 虚函数
    调用的函数还需要是虚函数
- 指针作向上转型up-cast
    这个不好理解，可以认为是父类指针指向子类对象。因为在类的设计图中父类一般画在上方，所以叫向上转型。


## C++的复合
composition（复合），表示has-a，即类A中有类B，就可以说A和B是复合关系，即A has B。
复合情况下的构造和析构
- 构造时由内而外
    container的构造函数首先调用，component的default构造函数然后再执行自己的;注意，编译器会帮我们加代码，但是只能加默认的component的构造函数，如果希望指定构造函数，需要自己像下方一样的格式书写
`Container::Container(...):Component(){...}`
- 析构由外而内
    Container的析构函数首先执行自己，然后才调用Component的析构函数
`Container::~Container(..){... ~Conponent();}`

## C++的委托
Delegation（委托），Composition by reference ，表示类A中有类B的指针，就可以说A和B是委托关系。
注：委托关系中的双方声明周期可能不一致，即可能类A消失了，但类B还存在。

习题：
1、如果子类Derived继承自父类Base，其子类还有一个Component复合关系，构造函数和析构函数如何调用执行。
2、如果父类有component的复合关系，子类继承自父类，构造函数和析构函数如何执行。

## C++的namespace
namespace是针对于工程开发中，可能会出现两个程序员声明了同一个变量，会产生编译错误。所以通过namespace包含变量和函数名，不同的namespace可以名称重复且编译器不会报错

## C++的数量不定的模板参数
C++允许使用数量不定的模板参数作为模板函数的输入，具体参考print.cpp

## C++的auto
auto会让编译器自动推断该变量的类型，因此，**auto必须要赋值**，否则只是声明变量，编译器不知道该怎么帮我们推断。

## C++的this指针
基于虚函数的多态情况下，理解this指针。
在调用成员函数时，编译器会自动帮我们传入一个参数this指针：
- 如果调用的是非虚函数，则是静态绑定，类的所有实例中的非虚函数都只存一份，所以指向的函数是固定的。
- 如果调用的是虚函数，则是动态绑定，会根据传入的this指针的不同，调用不同的函数。见Cdocument.cpp：
```C++
    //创建子类对象
    CMydoc openfile;
    //通过子类对象调用父类函数，其中Serialize会调用子类实现的函数
    openfile.OnFileOpen();
```
openfile是一个子类对象，虽然调用的是父类的OnFileOpen虚函数，但是传入的this指针是&openfile，所以会调用子类的函数。

补充：
static成员函数没有this指针，所以static成员函数不能访问类的非static数据。


