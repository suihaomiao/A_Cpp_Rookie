#include <iostream>
#include <cstdio>
#include <vector>
#include <ext/pool_allocator.h>

using namespace std;

class Test
{
public:
    long m_i;
    long m_j;

    struct Obj // 相当于在类中定义了一个结构体
    {
        struct Obj *pNext; // 这个pNext是一个嵌入式指针
        //
    };
};

void func()
{
    Test myTest;
    cout << sizeof(myTest) << endl; // 16字节，结构体只是在类中定义，并不是类中有结构体的成员变量
    myTest.m_i = -1;                //这里让其内存为0xffffffff
    myTest.m_j = -1;
    Test::Obj *pTemp;                              // 定义一个指针
    pTemp = (Test::Obj *)&myTest;                  // pTemp 指向对象myTest 的首地址
    pTemp->pNext = nullptr;                        //这部分让myTest的前8个字节全是0，因为目前在64位系统下运行
    cout << "myTest.m_i = " << myTest.m_i << endl; //从内存角度来说，m_i就等于0；
    cout << "myTest.m_j = " << myTest.m_j << endl; // m_j = -1，因为只是占用了前面的8个字节
    // 因为Test::Obj 结构体中只有一个指针，pTemp 指向的是Test::Obj 里面内容的首地址
    // 而其中的内容只有pNext，所以此时pTemp 和 pTemp->pNext 所指向的是同一块位置
    // pTemp->pNext = nullptr;相当于把myTest 的前4个字节初始化为 00 00 00 00
    // 如果将pTemp->pNext 指向下一块待分配的内存，就实现了空间内存块的链接
}

//在64位操作系统下，如果类的大小小于8呢？嵌入式指针不能生效的情况，怎么办？
void func2()
{
    vector<int, __gnu_cxx::__pool_alloc<int>> a;
    // a应该是大小24,应为vector内部用了三根指针去管理内存
    cout << sizeof(a) << endl;
    for (int i = 0; i < 1000000; ++i) //循环100万次，去push_back元素
    {
        a.push_back(i);
        cout << "&a[i] = " << &a[i] << "\ti = " << i << endl;
        //从上一步的输出可以看到，每一个元素相隔4个字节。
        //无法解释，因为在64位系统下，一个指针8个字节，怎么在一个4个字节的内存上把它当成指针用呢？
    }
}

int main()
{

    // int a = 0x12345678;
    // int *pa = &a;
    // char *pc = (char *)&a;

    // printf("pc = %x\n", pc);     //pc = e4e3f174
    // printf("pa = %x\n", pa);     //pa = e4e3f174
    // printf("*pc = %x\n", *pc);   //*pc = 78
    // printf("*pa = %x\n", *pa);   //*pa = 12345678

    func();

    func2();
}