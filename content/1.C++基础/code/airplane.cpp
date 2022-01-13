#include <iostream>

using std::cout;

class airplane
{
public:
    unsigned long getMiles() { return rep.miles; }
    char getType() { return rep.type; }
    void set(unsigned long m, char t)
    {
        rep.miles = m;
        rep.type = t;
    }

    static void *operator new(size_t size);
    static void operator delete(void *pdead, size_t size);

private:
    struct airplaneRep
    {
        unsigned long miles;
        char type;
    };

    union
    {
        airplaneRep rep;
        airplane *next;
    };

    static const int BLOCK_SIZE;
    static airplane *headOfFreeList;
};

const int airplane::BLOCK_SIZE = 512;
airplane *airplane::headOfFreeList = nullptr;

void *airplane::operator new(size_t size)
{
    //如果大小有误，则转交给::operator new
    //应该是在发生继承后，造成类的大小不一致
    if (size != sizeof(airplane))
        return ::operator new(size);

    airplane *p = headOfFreeList;
    if (p)
    {
        //如果p是有效的
        headOfFreeList = p->next;
    }
    else
    {
        //如果freelist是空的，就申请开辟一块
        airplane *newBlock = static_cast<airplane *>(::operator new(BLOCK_SIZE * sizeof(airplane)));

        //将大块内存分割成小块
        for (int i = 1; i < BLOCK_SIZE; ++i)
            newBlock[i].next = &newBlock[i + 1];
        newBlock[BLOCK_SIZE - 1].next = 0; //链表结束
        p = newBlock;
        headOfFreeList = &newBlock[1];
    }
    return p;
}

void airplane::operator delete(void *pdead, size_t size)
{
    if (pdead == nullptr)
        return;
    if (size != sizeof(airplane))
    {
        ::operator delete(pdead);
        return;
    }
    //这一块着重理解一下
    airplane *carcass = static_cast<airplane *>(pdead);
    carcass->next = headOfFreeList;
    headOfFreeList = carcass;
}

int main()
{
    //之前看到的，airplane因为内存对齐的问题，应该是32个字节
    cout << sizeof(airplane) << '\n';
    const size_t N = 100;
    airplane *p[N];

    for (int i = 0; i < N; ++i)
        p[i] = new airplane;

    //随机测试airplane，看是否正常
    p[6]->set(10000, 'A');
    p[67]->set(1000, 'B');
    p[99]->set(200, 'C');

    //输出point，观察间隔
    for (int i = 0; i < 100; ++i)
        cout << "p[" << i << "] = " << p[i]
             << " i = " << i << '\n';

    for (int i = 0; i < N; ++i)
        delete p[i];
}
