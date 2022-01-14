#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>

using namespace std;

//完美转发测试
//左值版本
void process(const int &x)
{
    cout << "process(const int &x): x = " << x << endl;
}

//右值版本
void process(const int &&x)
{
    cout << "process(const int &&x): x = " << x << endl;
}

//在forward函数内再调用x，forward函数只写了右值版本而已，所以只能将右值传入，否则会报错
void forward1(int &&x)
{
    cout << "forward(int &&x): x = " << x << endl;
    process(x);
}

// forward的完美转发版本
void forward2(int &&x)
{
    cout << "forward(int &&x): x = " << x << endl;
    process(std::move(x));
}

class Mystring
{
private:
    char *m_data;
    size_t m_len;
    //内存拷贝-深拷贝过程
    void m_init_data(const char *s)
    {
        m_data = new char[m_len + 1];
        memcpy(m_data, s, m_len);
        m_data[m_len] = '\0';
    }

public:
    static size_t DCtor; //累计defult-cot呼叫次数
    static size_t Ctor;  //累计cotr呼叫次数
    static size_t CCtor; //累计拷贝构造会叫次数
    static size_t CAsgn; //累计拷贝复制呼叫次数
    static size_t MCtor; //累计拷贝复制呼叫次数
    static size_t MAsgn; //累计移动构造呼叫次数
    static size_t Dtor;  //累计析构呼叫次数

    //默认构造函数
    Mystring() : m_data(nullptr), m_len(0) { ++DCtor; }

    //构造函数
    Mystring(const char *s) : m_len(strlen(s))
    {
        ++CCtor;
        m_init_data(s);
    }

    //拷贝函数
    Mystring(const Mystring &str)
    {
        //深拷贝
        ++CCtor;
        m_init_data(str.m_data);
    }

    //移动构造函数
    Mystring(Mystring &&str) noexcept //要加noexcept
        : m_data(str.m_data), m_len(str.m_len)
    {
        ++MCtor;
        //重置str的数据，因为以后不会再用它了
        str.m_len = 0;
        str.m_data = nullptr; //一定要把该指针设置为null，因为
    }

    //拷贝复制
    Mystring &operator=(const Mystring &str)
    {
        ++CAsgn;
        //自我赋值检查！！！！！非常重要
        if (this != &str)
        {
            if (m_data)
                delete m_data;
            m_len = str.m_len;
            m_init_data(str.m_data);
        }
        return *this;
    }

    //移动复制
    Mystring &
    operator=(Mystring &&str) noexcept
    {
        ++MAsgn;
        if (this != &str) //自我赋值检查
        {
            if (m_data)
                delete m_data;
            m_len = str.m_len;
            m_data = str.m_data;
            str.m_len = 0;
            str.m_data = nullptr; //偷资源后，需要设置为Nullptr
        }
        return *this;
    }

    //析构函数
    ~Mystring()
    {
        ++Dtor;
        if (m_data)
            delete m_data;
    }

    bool operator<(const Mystring &rhs) const
    {
        return string(this->m_data) < string(rhs.m_data);
    }
    bool operator==(const Mystring &rhs) const
    {
        return string(this->m_data) == string(rhs.m_data);
    }

    char *get() const { return m_data; }
};

size_t Mystring::DCtor = 0; //累计defult-cot呼叫次数
size_t Mystring::Ctor = 0;  //累计cotr呼叫次数
size_t Mystring::CCtor = 0; //累计拷贝构造会叫次数
size_t Mystring::CAsgn = 0; //累计拷贝复制呼叫次数
size_t Mystring::MCtor = 0; //累计拷贝复制呼叫次数
size_t Mystring::MAsgn = 0; //累计移动构造呼叫次数
size_t Mystring::Dtor = 0;  //累计析构呼叫次数

namespace std
{
    template <>
    struct hash<Mystring>
    {
        //测试unordered_map
        size_t operator()(const Mystring &s) const noexcept
        {
            //借用现成的hash
            return hash<string>()(string(s.get()));
        }
    };
}

#include <typeinfo>
template <typename T>
void output_static_data(const T &myStr)
{
    cout << typeid(myStr).name() << "--" << endl;
    cout << "DCtor = " << T::DCtor
         << "\tCtor = " << T::Ctor
         << "\tCCtor = " << T::CCtor
         << "\tCAsgn = " << T::CAsgn
         << "\tMCtor = " << T::MCtor
         << "\tMAsgn = " << T::MAsgn
         << "\tDtor = " << T::Dtor
         << endl;
}

template <typename M, typename NM>
void test_moveable(M c1, NM c2, const long &value)
{
    char buf[10];

    //测试moveable
    typedef typename iterator_traits<typename M::iterator>::value_type Vtype;
    clock_t timeStart = clock();
    for (long i = 0; i < value; ++i)
    {
        snprintf(buf, 10, "%d", rand());
        auto ite = c1.end();
        c1.insert(ite, Vtype(buf));
    }
    cout << "construction, milli-second:" << (clock() - timeStart) << endl;
    cout << "c1.size() = " << c1.size() << endl;
    output_static_data(*(c1.begin()));

    M c11(c1);
    M c12(std::move(c1));
    c11.swap(c12);
}

int main()
{
    //完美转发测试
    int x = 1;
    process(x);  // process的左值调用
    process(2);  // process的右值调用
    forward1(3); // forward的右值调用
    forward2(3);
    //调用结果如下：
    // process(const int &x): x = 1
    // process(const int &&x): x = 2
    // forward(int &&x): x = 3
    // process(const int &x): x = 3
    // forward传入的是右值，但是在内部调用process的时候调用的是左值版本

    //测试Mystring
    // test_moveable(vector<Mystring>(), vector<string>(), 3000000);
}