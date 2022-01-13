#include <iostream>

using namespace std;

#define LOG(x) cout << x << endl

void Increment1(int value)
{
    value++;
}
void Increment2(int *value) { (*value)++; }
void Increment3(int &value) { value++; }

int main()
{
    // Increment让传入的value+1；上述一共三个，分别是值传递，指针传递和引用传递
    int value1 = 5;
    int value2 = 5;
    int value3 = 5;
    //显然值传递不会让value1改变
    Increment1(value1);
    //指针传递后，value2=6；
    Increment2(&value2);
    //引用传递后，value3=6；引用和指针差不多，但是使用起来引用更像是一种别名
    //引用的使用场景更多发生在函数形参的部分，可以通过传递引用的方式来改变栈上的变量
    Increment3(value3);

    LOG(value1);
    LOG(value2);
    LOG(value3);
}