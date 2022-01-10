#include <iostream>

// val在test.cpp文件定义为static变量，此cpp文件是不可见的，所以这样的定义不会出错
int val = 5;
// val2是全局变量，需要加上val2的extern声明，否则会报错
//同样不可以去掉extern关键字，不然会报错，编译器提醒多重定义，原因是在test.cpp文件中已经定义了val2
extern int val2;

// func是不可以被main.cpp文件使用的，因为
//  void func();
void func2();
using namespace std;

#define LOG(x) cout << x << endl

class Test
{
public:
    static int x, y;
};
//这里需要声明一下static变量，因为static还是保留了原有的属性，即声明为static的变量可以“简单”认为是一种private权限
//定义在类里面的static变量也必须在类外声明一下，否则编译器报错。
int Test::x;
int Test::y;

int main()
{

    //打印val，val在当前cpp文件是一个全局变量，test.cpp中定义的val是一个static变量
    //所以当前的val=5；
    LOG(val); //打印5
    LOG(val2);
    //不可以调用func，因为func在test.cpp文件中声明成了static函数，类似于static变量
    // func();
    func2();

    Test test1;
    //这里是通过类实例来访问static变量
    test1.x = 10;
    test1.y = 20;
    //这里是通过类直接访问static变量，因为static变量不属于任何一个类实例，可以说所有的实例共享这一份内存
    //所以可以不通过类实例直接访问到该内存
    LOG(Test::x);
    LOG(Test::y);
}