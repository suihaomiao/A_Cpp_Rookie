#include <iostream>
#include <string.h>

using namespace std;

class mystring
{
public:
    //构造函数
    mystring(const char *cstr = 0);
    //拷贝构造函数
    mystring(const mystring &str);
    //拷贝函数
    mystring &operator=(const mystring &str);
    //析构函数
    ~mystring();
    //如果类中有指针，必须要实现上述三个拷贝构造，拷贝函数和析构函数

    char *get_c_str() const { return m_data; }

private:
    char *m_data;
};

inline mystring::mystring(const char *cstr)
{
    if (cstr)
    {
        //这里简单说一下strlen和sizeof用于字符串时的返回结果
        //对于字符串char *s = "hello"
        // strlen返回的是真正的字符串大下：strlen(s) == 5;
        // sizeof返回的是变量所占的内存大小(字符串是一个指针，指针要么4字节要么8字节):sizeof(s) = 8;
        m_data = new char[strlen(cstr) + 1];
        strcpy(m_data, cstr);
    }
    else
    {
        //没有指定数据
        m_data = new char[1];
        *m_data = '\0';
    }
}

mystring::mystring(const mystring &str)
{
    if (this == &str) //检测是否是自我赋值
        return;
    this->m_data = new char[strlen(str.m_data) + 1];
    strcpy(this->m_data, str.m_data);
}

mystring &mystring::operator=(const mystring &str)
{
    if (this == &str) //一定要写自我赋值检测！！！否则会出现野指针的问题，拷贝赋值函数一定要写这一段
        return *this;
    delete[] this->m_data;
    this->m_data = new char[strlen(str.m_data) + 1];
    strcpy(this->m_data, str.m_data);
    return *this;
}

inline mystring::~mystring()
{
    //如果有指针，需要手动释放数据
    delete[] m_data;
}

ostream &operator<<(ostream &os, const mystring &str)
{
    os << str.get_c_str();
    return os;
}

int main()
{
    mystring s1("hello arsenal!");
    mystring s2("hello world!");
    mystring s3;
    s3 = s1;
    mystring s4(s1);
    cout << s2 << endl;
    cout << s3 << "   &s3 = " << &s3 << endl;
    cout << s4 << "   &s3 = " << &s4 << endl;
    char *s = "12345678910";
    cout << "strlen(s) = " << strlen(s) << endl;
    cout << "sizeof(s) = " << sizeof(s) << endl;
}
