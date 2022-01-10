#include <stdio.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    size_t i;
    for (i = 0; i < len; ++i)
    {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_int(int x)
{
    //这里有一个强制类型转换，将x的地址强制转换成byte_pointer，然后传入函数
    //注意这里并不会改变x的地址，只是告诉编译器，把它当成byte_pointer看待即可
    //这里就涉及到指针的两个要素，详细见C++基础的指针与内存章节
    show_bytes((byte_pointer)&x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer)&x, sizeof(float));
}

void show_pointer(void *x)
{
    show_bytes((byte_pointer)&x, sizeof(void *));
}

void test_show_bytes(int val)
{
    int ival = val;
    float fval = (float)ival;
    int *pval = &ival;
    const char *s = "abcdef";
    show_int(ival);
    show_float(fval);
    show_pointer(pval);
    show_bytes((byte_pointer)s, strlen(s));
}

int main()
{
    test_show_bytes(-7);
}