#include <iostream>
#include <cmath>

using namespace std;

// template <typename T>
class complex
{
public:
    complex(double r = 0, double i = 0) : re(r), im(i)
    {
    }

    //下面的构造函数和上面的构造函数会发生歧义，编译器会提示错误
    // complex() : re(0), im(0) {}

    complex &operator+=(const complex &);
    bool operator==(const complex &);
    complex &operator*(const double n);
    //该<<操作符不能写成成员函数
    // ostream &operator<<(const complex &);

    double real() const { return re; }
    double imag() const { return im; }

private:
    double re, im;

    friend complex &__doapl(complex *, const complex &);
};

//操作符重载
complex &complex::operator+=(const complex &r)
{
    // this->re += r.re;
    // this->im += r.im;
    // return *this;
    return __doapl(this, r);
}

bool complex::operator==(const complex &r)
{
    return (this->re == r.real()) && (this->im == r.imag());
}

// 输出等需要用到std标准库里面的输出类型的重载，必须写成全局函数的形式，不能写成成员函数
// 第一个参数需要是std的类型即ostream
ostream &operator<<(ostream &os, const complex &x)
{
    return os << '(' << x.real() << ',' << x.imag() << ')';
}

//只能类中只能提供c1*5这样的函数重载，5*c1这样的只能写成全局的重载
complex &complex::operator*(const double n)
{
    this->re *= n;
    this->im *= n;
    return *this;
}

// complex &operator*(const double n, complex &ths)
// {
//     ths.real
// }

//友元函数
inline complex &__doapl(complex *ths, const complex &r)
{
    ths->re += r.re;
    ths->im += r.im;
    return *ths;
}

int main()
{
    complex c1(3, 4);
    complex c2(2, 1);
    c1 += c2;
    cout << c1.real() << endl;
    cout << c1 << c2 << endl;
    cout << (c1 == c2 ? 0 : 1) << endl;
    cout << c1 * 5 << endl;
    // cout << 5 * c1 >> endl;
}