#include <iostream>

using namespace std;

class fraction
{
public:
    explicit fraction(int num, int den = 1)
        : m_numerator(num), m_denominator(den) {}
    //转换函数
    operator double() const
    {
        cout << "double is called ...\n";
        // return static_cast<double>(m_numerator / m_denominator);
        //实际上应该先把两个int变量都转换成double，再相除，才能得到想要的结果
        return (double)m_numerator / (double)m_denominator;
    }

    // fraction operator+(const fraction &f)
    // {
    //     return fraction(2);
    // }

private:
    int m_numerator;   //分子
    int m_denominator; //分母
};

int main()
{
    fraction f(3, 5);
    double d = 4 + f;
    cout << d << '\n';
    // cout << double(f) << '\n';
    // cout << (double)3 / 5 << '\n';
    // fraction c = f + 2;
    // cout << c << '\n';
}