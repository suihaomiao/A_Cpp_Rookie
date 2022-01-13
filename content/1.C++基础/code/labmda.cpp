#include <iostream>
#include <set>
#include <string>

using std::cout;
using std::set;
using std::string;

class Person
{
public:
    string lastname;
};

int main()
{

    int id = 0;
    auto f1 = [id]() mutable
    {
        cout << "id = " << id << '\n';
        ++id;
    };
    auto f2 = [&id]() mutable
    {
        cout << "id = " << id << '\n';
        ++id;
    };

    auto f3 = [id]() mutable
    {
        cout << "id = " << id << '\n';
        ++id;
        static int x = 5;
        int y = 6;
        //还可以返回
        // return x;
    };

    id = 42;
    f1();                                  // id=0;
    f1();                                  // id=1;
    f1();                                  // id=2;
    cout << "outsize id = " << id << '\n'; // id=42;
    //因为id是值传递，在f1定义时，id=0，所以只是把0这个值拷贝过来了。然后，我们看看引用传递
    //虽然是指传递，但是在连续调用f1()后，id在f1内部的值也在变化，这和我们想象中的函数的值传递不太一样

    f2();                                  // id=42;
    f2();                                  // id=43;
    f2();                                  // id=44;
    cout << "outsize id = " << id << '\n'; // id=45;
    //发现在值传递后，id在后面的变化后，labmda里面的id也会跟着变

    //实现以下，labmda当做仿函数的作用
    auto cmp = [](const Person &p1, const Person &p2) -> bool
    {
        return p1.lastname < p2.lastname;
    };
    //这个地方需要特别注意，在coll(cmp)，这里面的cmp一定不要省略
    //省略了会出问题，但是目前编译和运行都没有报错；应该是set内部进行处理
    //实际上，作为比较的和判断的依据，最好还是写仿函数
    set<Person, decltype(cmp)>
        coll(cmp);
    //是因为set的源代码修改了，set的默认构造函数=default，用的是编译器默认构造的那个
    set<Person, decltype(cmp)>
    coll2();
}