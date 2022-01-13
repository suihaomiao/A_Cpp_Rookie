#include <iostream>
#include <bitset>
#include <algorithm>
#include <tuple>
#include <string>
using namespace std;

//例子1，写一个万能打印函数
void printX() {}

template <typename T, typename... Types>
void printX(const T &firstArg, const Types &...args)
{
    //如果需要知道args里面有多少个参数，可以用sizeof...(args)
    cout << firstArg << endl;
    cout << "sizeof...(args)" << sizeof...(args) << endl;
    printX(args...);
}

//例子2，用variadic template重写printf()
void myPrintf(const char *s)
{
    while (*s)
    {
        //有可能++s已经超过字符串长度了
        if (*s == '%' && *(++s) != '%') //这里并没有对字符串的边界做检查，只是简单的demo
            throw std::runtime_error("invalid format string: missing arguments");
        cout << *s++;
    }
}

template <typename T, typename... Args>
void myPrintf(const char *s, T value, Args... args)
{
    while (*s)
    {
        if (*s == '%' && *(++s) != '%') //这里并没有对字符串的边界做检查，只是简单的demo
        {
            cout << value;
            myPrintf(++s, args...);
            return;
        }
        cout << *s++;
    }
}

//例子4，用variadic template 实现maximum
int maximum(int n)
{
    return n;
}

template <typename... Args>
int maximum(int n, Args... args)
{
    return std::max(n, maximum(args...));
}

//例子5，需要按照格式输出不同的元素，例如：
// 7.5 hello bitset<16>377 42，这几个元素输出成
//[7.5,hello,bitset<16>377,42]

template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE
{
    static void print(ostream &os, const tuple<Args...> &t)
    {
        // get<IDX>(t)会拿到tuple里面的IDX的东西
        os << get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
        PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t);
    }
};

template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...>
{
    static void print(ostream &os, const tuple<Args...> &t)
    {
    }
};

template <typename... Args>
ostream &operator<<(ostream &os, const tuple<Args...> &t)
{
    os << '[';
    PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
    return os << ']';
}

//例子6，实现一个简单tule，体现递归继承的魅力
//在不断的递归继承中，获取到父类的数据，然后将不同的数据类型打包在一起
template <typename... Values>
class myTuple;

template <>
class myTuple<>
{
};

template <typename Head, typename... Tail>
class myTuple<Head, Tail...> : private myTuple<Tail...> //这里对父类的私有继承，tail...就是剩下的类型的元素
{
    typedef myTuple<Tail...> inherited;

protected:
    Head m_head;

public:
    //构造函数
    myTuple() {}
    myTuple(Head v, Tail... vtail)
        : m_head(v), inherited(vtail...) {} // inherited(vtail...)
    //展开之后是myTuple<Tail...>(vtail...)，这样看起来是一个创建一个临时对象
    //但是注意，我们这里在是列表初始化，所以这里其实调用父类的构造函数，只是父类是一个模板类，还是可变的模板类

    //这两个函数需要简单解释一下
    // head()会返回当前的数据，这个很容易理解
    //只是这里用到了返回类型后置，而且我们用了decltype，让编译器帮我们去推断返回的类型
    //实际上类型我们已经知道了，就是Head，所以head()还可以写成
    // Head head(){return m_head;}
    auto head() -> decltype(m_head) { return m_head; }
    // tail返回的是父类的内存数据，为什么呢？
    //因为我们返回了*this，这是子类的内存，返回后因为类型是inherited，这是继承的父类的类型
    //会发生强制类型转换，还记得嵌入式指针吗？差不多是一个道理
    //那么返回的就是父类内存引用了。
    //还有一点，因为父类的构造函数总是比子类先调用，所以按照下面调用的顺序，在内存分布里面string会在float上面，float会在int的上面
    //非常巧妙的设计，tail()，就可以拿到我们想要的指针（或者说叫内存）c/c++太美妙了！
    inherited &tail() { return *this; }
};

int main()
{
    //例子1，万能ptrint()测试
    // printX('c', "hello world!", bitset<16>(377), 42, 100L, 3.1415926);

    //例子2，重写printf测试
    // int *pi = new int;
    // myPrintf("%d\t%s\t%p\t%f and this is myPrintf()...\n", 15, "This is Ace.", pi, 3.1415926);
    // delete pi;

    //例子3，max可以传入任意的参数比大小。
    //但是，实际上这里并没有用variadic Template的技术
    // cout << max({57, 48, 60, 100, 20, 18}) << endl;

    //例子4，手动实现max
    // cout << maximum(57, 48, 60, 100, 20, 18) << endl;

    //例子5，打印tuple
    // cout << make_tuple(7.5, std::string("hello"), 42) << endl;

    //例子6，自己实现的简单tuple
    myTuple<int, float, std::string> t(41, 6.3, "nico");
    cout << "sizeof(t) = " << sizeof(t) << endl;
    cout << t.head() << endl;
    cout << t.tail().head() << endl;
    cout << t.tail().tail().head() << endl;
}