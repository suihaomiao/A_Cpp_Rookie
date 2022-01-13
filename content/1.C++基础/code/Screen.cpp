#include <iostream>
#include <vector>
#include <ext/pool_allocator.h>

using namespace std;

class Screen
{
public:
    Screen(long x) : m_id(x){};
    long get() { return m_id; }

    static void *operator new(size_t);
    static void operator delete(void *pdead, size_t size);

private:
    Screen *next;
    long m_id;

    static Screen *freeStore;
    static const int screenChunk;
};

//需要对static的类变量，声明，且需要加上Sreen::域操作符
Screen *Screen::freeStore = nullptr;
const int Screen::screenChunk = 20; //这里的数字的意义在哪？
//实际上这个数字会告诉编译器，一次new的空间只能存20个对象
//但是如果继续new会再次开辟20个对象的空间出来，但是这个空间会因为链表而“连续”
//实际表现在，每20个对象之间是间隔16个字节，但是20和21个对象会间隔32个字节，因为cookie的存在
//但是，因为操作系统的并发性，往往没有这么连续，但是基本规律是这样的

void *Screen::operator new(size_t size)
{
    Screen *p;
    if (!freeStore)
    {
        //如果链表是空的，就申请一大块
        size_t chunk = screenChunk * size;
        //申请一大堆内存，数量是chunk
        freeStore = p = reinterpret_cast<Screen *>(new char[chunk]);
        //然后把这一大块的内存切片，当做linklist串联起来
        for (; p != &freeStore[screenChunk - 1]; ++p)
            p->next = p + 1;
        p->next = nullptr;
    }

    p = freeStore;
    freeStore = freeStore->next;
    return p;
}

void Screen::operator delete(void *p, size_t size)
{
    //将删除的对象，回到free list的前端
    (static_cast<Screen *>(p))->next = freeStore;
    freeStore = static_cast<Screen *>(p);
}

std::vector<int, __gnu_cxx::__pool_alloc<int>> a;

int main()
{
    //这里输出Screen类的大小
    //最开始将m_id声明成int，输出16；后来发现内存对齐的原因，因为这里的云服务器是64位操作系统
    //类Screen中有一个指针和一个int，指针是8个字节，所以内存对齐后，即使声明成int，也是16个字节，不如声明long
    cout << "sizeof(Screen) = " << sizeof(Screen) << '\n';

    const size_t N = 100;
    Screen *p[N];

    for (int i = 0; i < N; ++i)
        // p[i] = ::new Screen(i);
        p[i] = new Screen(i);

    //输出前30个pointer，比较其间隔
    //实际上没有想象的效果
    //实际上，我们开辟了24个Screen类的空间出来,new了100个？
    for (int i = 0; i < 100; ++i)
        cout << p[i] << "  i = " << i << '\n';
    for (int i = 0; i < N; ++i)
        delete p[i];
    // cout << sizeof(int) << '\n';
    //如果调用::new（全局的new），打印内存会发现间隔0x20，32个字节！！是因为有cookie的存在！！
}