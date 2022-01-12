#include <iostream>
#include <bitset>

using std::cout;

void print() {}

//注意到第二个参数typename后面的...
template <typename T, typename... Types>
// Types后面的...
void print(const T &firstArg, const Types &...args)
{
    cout << firstArg << '\n';
    //递归调用时的...
    print(args...);
}

int main()
{
    //通过可变模板参数，来实现一个万能打印
    print(7.2, "hello", 42);
}