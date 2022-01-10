#include <stdio.h>

int main()
{
    unsigned int x = 0x87654321;
    printf("x = %x\n", x);
    printf("最低位有效：%x\n", x & 0xff);
    //这个地方需要理解一下，怎么用位运算取补码：
    printf("除最低位，其他位置取补:%x\n", x ^ ~0xff);
    printf("最低位设置成1，其他不变:%x\n", x | 0xff);
}