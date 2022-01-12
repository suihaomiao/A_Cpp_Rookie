#include <iostream>
#include <cstdlib>
#include <string>
using std::cout;
using std::string;

class foo
{
public:
    foo(const int id) : m_id(id)
    {
        cout << "call int ctor. this = " << this << "   id = "
             << m_id << '\n';
    }
    foo()
    {
        cout << "default ctor. this = " << this << "   id = "
             << "0\n";
    }
    ~foo()
    {
        cout << "call dtor. this = " << this << "  id = " << m_id << '\n';
    }
    //声明成静态的；
    //因为在new表达式时，对象还没有创建，但是需要先分配内存给指针，所以声明成静态的才能调用成功
    static void *operator new(size_t size);
    static void operator delete(void *pdead, size_t size);
    static void *operator new[](size_t size);
    static void operator delete[](void *pdead, size_t size);

private:
    // int-4字节，long-8字节，string-指针，8字节。
    //一共是20个字节
    int m_id;
};

static void *foo::operator new(size_t size)
{
    //多分配四个字节，没用
    foo *p = (foo *)malloc(size);
    // cout << sizeof(*p) << '\n';
    cout << "call foo::operator new...\n";
    return p;
}

static void foo::operator delete(void *pdead, size_t size)
{
    free(pdead);
    cout << "call foo::operator delete...\n";
}

static void *foo::operator new[](size_t size)
{
    //
    foo *p = (foo *)malloc(size);
    cout << "call foo::operator new[] ...\n";
    return p;
}

static void foo::operator delete[](void *pdead, size_t size)
{
    free(pdead);
    cout << "call foo::delete[]... \n";
}

int main()
{
    foo *p1 = new foo();
    // foo *p2 = new foo(2);
    foo *pArray = new foo[5];
    //可以通过::new的形式强制调用global形式的new，即编译器为我们提供的new
    foo *p3 = ::new foo();
    //打印一些信息
    // cout << "sizeof(foo) = " << sizeof(foo) << '\n';
    // cout << "sizeof(*p1) = " << sizeof(*p1) << '\n';
    // cout << "sizeof(*p2) = " << sizeof(*p2) << '\n';

    delete p1;
    delete[] pArray;
    //::delete和::new一样，强制调用global的delete
    ::delete p3;
    // delete p2;
}