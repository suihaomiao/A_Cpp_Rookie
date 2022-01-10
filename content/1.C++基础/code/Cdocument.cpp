#include <iostream>

using namespace std;

class Cducoment
{
public:
    void OnFileOpen()
    {
        //这是算法，每个cout表示一个实际动作
        cout << "dialog ..." << endl;
        cout << "check file status ..." << endl;
        cout << "open file ..." << endl;
        Serialize();
        cout << "close file ..." << endl;
        cout << "update all views ..." << endl;
    }

    virtual void Serialize(){};
};

class CMydoc : public Cducoment
{
public:
    virtual void Serialize()
    {
        //只有应用本身才知道怎么读取文件
        cout << "CMydoc::Serialize ...." << endl;
    }
};

int main()
{
    //创建子类对象
    CMydoc openfile;
    //通过子类对象调用父类函数，其中Serialize会调用子类实现的函数
    openfile.OnFileOpen();

    //将不会有Serialize相关的信息，因为父类的Serialize函数是空的
    Cducoment base;
    base.OnFileOpen();

    //打印的信息如下
    //在关键的Serialize步骤，会调用子类的方法，因为openfile被声明成子类的类型，如果是父类的对象调用OnFileOpen将不会有关键的Serialize步骤
    //     dialog ...
    // check file status ...
    // open file ...
    // CMydoc::Serialize ....
    // close file ...
    // update all views ...
}