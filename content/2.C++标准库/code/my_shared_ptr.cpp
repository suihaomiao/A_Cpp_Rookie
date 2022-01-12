#include <iostream>
#include <algorithm>
using std::cout;

template <class T>
class my_shared_ptr
{
private:
    T *px;
    long *pn;

public:
    my_shared_ptr(T *p) : px(p) { (*pn)++; }
    ~my_shared_ptr()
    {
        if (*pn == 0)
            delete px;
    }
    T *operator->() const
    {
        return px;
    }
};

class foo
{
public:
    void method() {}
};

int main()
{
    my_shared_ptr<foo> sp(new foo);
    sp->method();
}