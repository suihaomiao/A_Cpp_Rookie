#include <iostream>

static int val = 10;

int val2 = 10;

//加上static表示该函数仅该cpp文件可见（private概念），不加static可以理解为全局函数，当然在class外的函数都是全局函数
static void func(){
    std::cout << "hello world!" << std::endl;
}

void func2(){
    std::cout << "hello world!2" << std::endl;
}
