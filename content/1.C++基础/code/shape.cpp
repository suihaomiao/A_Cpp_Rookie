#include <iostream>

using namespace std;

class shape
{
public:
    virtual void draw() const = 0;              //纯虚函数
    virtual void error(const std::string &msg); //普通虚函数
    int objectID() const;                       //普通函数
};

class Rectangle : public shape
{
public:
    double x, y;
};

int main()
{
    cout << "hello world!" << endl;
}