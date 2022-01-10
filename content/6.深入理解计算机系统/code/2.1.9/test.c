#include <stdio.h>
#include <stdint.h>
//为测试移位运算
//左移、算术右移、逻辑右移

int main()
{
    int x = -7;                  // 0x?
    unsigned int y = 16;         // 0x1000
    unsigned int z = 1;          // 0x01
    unsigned int a = UINT32_MAX; // 0xffffffff
    printf("z 左移动3位:%x\n", z << 3);
    printf("y 右移动4位:%x\n", y >> 4);
    printf("x 右移动4位:%x\n", x >> 4);
    printf("a 左移34位:%x\n", a << 34); //实际上会自动取模？
}