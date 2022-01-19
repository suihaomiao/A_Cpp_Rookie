#include <iostream>
#include <memory>
#include <string>

using std::cout;

class Report
{
public:
    Report(const std::string &s) : str(s) { cout << "Report had Creat!\n"; }
    ~Report() { cout << "Report had Destroy!\n"; }
    void commit() { cout << str << '\n'; }

private:
    std::string str;
};

int main()
{
    {
        std::shared_ptr<Report> ps(new Report("this is shared_ptr!"));
        ps->commit();
    }

    {
        std::unique_ptr<Report> pu(new Report("this is unique_ptr!"));
        pu->commit();
    }
}