#include <iostream>
#include <cstdlib>
#include <complex>

using std::cout;

//测试类A
class A
{
public:
    int m_id;

    A() : m_id(0) { cout << "default ctor. this = " << this << "   m_id = " << m_id << '\n'; }
    A(int i) : m_id(i) { cout << "call ctor. this = " << this << "   m_id = " << m_id << '\n'; }
    ~A() { cout << "call dtor. this = " << this << "   m_id = " << m_id << '\n'; }
};

int main()
{
    //分配一个512字节的内存块，然后释放
    void *p1 = malloc(512);
    free(p1);

    //创建一个complex对象
    std::complex<int> *p2 = new std::complex<int>;
    delete p2;

    //使用operator new申请内存，对operator new不熟悉需要去翻一下C++基础.md中关于new和delete那一节的内存
    //关系到C++new表达式的展开，内部会调用::operator new()，我们还可以去重载这个new函数
    void *p3 = ::operator new(512);
    ::operator delete(p3);

    //下面使用C++标准库提供的分配器接口，实际上，我在ubuntu上书写代码，使用的全是GNUC的编译器。
    //所以不再对其他编译器的写法做声明
    void *p4 = std::allocator<int>().allocate(5);
    std::allocator<int>().deallocate((int *)p4, 5);

    //测试array new 和array delete程序
    const int SIZE = 3;
    A *buf = new A[SIZE]; // array new会调用三次构造函数，但是目前array new 无法指定构造函数，只能调用默认的构造函数
    A *temp = buf;
    cout << "buf = " << buf << "  temp = " << temp << '\n';
    for (int i = 0; i < SIZE; ++i)
        new (temp++) A(i); // placement new 显式调用A的构造函数，这个到后面会详细解释
    cout << "buf = " << buf << "  temp = " << temp << '\n';

    delete[] buf; //这里看到，delete时，是从2,1,0这样的顺序进行删除
}