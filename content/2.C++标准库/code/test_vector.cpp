#include <iostream>
#include <vector>
#include <ext/pool_allocator.h>

using namespace std;

template <class T>
void print_vector(const vector<T> &a)
{
    for (const auto &x : a)
        cout << x << "  ";
    cout << endl;
}

int main()
{
    //默认构造函数，容器是空的，没有元素，但内部有三根指针来管理内存
    vector<int> one;
    //内部是三根指针，所以即使容器是空的，但是也有三根指针的大小
    cout << "sizeof(one) = " << sizeof(one) << endl;

    //构造一个容器，里面有10个元素；虽然我们没有指定元素，但是编译器会帮我们默认构造成0
    vector<int, __gnu_cxx::__pool_alloc<int>> two(10);
    // print_vector(two); // two里面的元素都是0
    //构造一个容器，里面有10个元素，且每个元素都是1
    vector<int> three(10, 1);
    print_vector(three); // three里面的元素都是1

    auto first = three.begin() + 3;
    auto last = three.begin() + 8;
    //用three的两个迭代器来构造vector,迭代器就不指定了
    vector<int> four(first, last);
    print_vector(four); //根据STL左闭右开原则，该容器只有5（8-3）个元素，8没有

    //拷贝构造函数
    vector<int> five(four);
    //下面的拷贝构造调用失败
    // vector<int> five(four, __gnu_cxx::__pool_alloc<int>);    //
    print_vector(five);

    //列表初始化构造一个tmp的容器
    vector<int> tmp = {
        1,
        2,
        3,
        4,
        5,
        6,
        7,
        8,
        9,
        10,
    };
    //然后吧tmp的东西全部搬运到six中来，这里调用std::move，将tmp转化成右值
    vector<int> six(std::move(tmp));
    print_vector(six); //此时的six是tmp原来的内存
    print_vector(tmp); //此时的tmp是空的容器

    //拷贝构造
    vector<int> seven = six;
    print_vector(seven); // six依然还保留自己的内存，只是内存全部拷贝到seven一份
    //拷贝移动
    vector<int> eight = std::move(seven);
    print_vector(eight); //获得了seven的内存
    print_vector(seven); // seven变空了
    print_vector(six);   // six还是没变

    six.resize(20);
    print_vector(six); //超出的部分用0来代替
    six.resize(5);     //不足原来容器的大小，只保留前n个
    print_vector(six);
    six.resize(10, 77);
    print_vector(six); //超出的部分因为指定了val，会用val来代替
    cout << "six.max_size() = " << six.max_size() << endl;
    cout << "six.capacity() = " << six.capacity() << endl;

    // reserve
    cout << "reserve testing ..." << endl;
    six.reserve(1);
    print_vector(six);
    cout << "six.capacity() = " << six.capacity() << endl;
    six.reserve(30);
    print_vector(six);
    cout << "six.capacity() = " << six.capacity() << endl;

    // shrink_to_fit
    cout << "shrink_to_fit testing ..." << endl;
    six.shrink_to_fit();
    print_vector(six);
    cout << "six.capacity() = " << six.capacity() << endl;

    //[]
    cout << "[] testing ..." << endl;
    six.resize(20); //此时six.size() = 10; six.capacity() = 20;
    print_vector(six);
    cout << "six.capacity() = " << six.capacity() << endl;
    cout << "six[9] = " << six[9] << endl;
    cout << "six[11] = " << six[11] << endl;
    cout << "six[300] = " << six[300] << endl;
    //不管n多大，始终没有报错。。。

    // element access ...
    cout << "element access  testing ..." << endl;
    cout << "six[2] = " << six[2] << endl;
    cout << "six.at(5) = " << six.at(5) << endl;
    cout << "six.front() = " << six.front() << endl;
    cout << "six.back() = " << six.back() << endl;
    int *p = six.data();
    cout << "p[2] = " << p[2] << endl;

    // modifiers
    cout << "modifiers testing ..." << endl;
    six.assign({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});
    print_vector(six); // 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
    three.assign(10, 7);
    print_vector(three);
    four.assign(six.begin(), six.begin() + 8);
    print_vector(four);
    four.push_back(100);
    print_vector(four);
    four.pop_back();
    print_vector(four);

    // insert
    cout << "insert testing ..." << endl;
    auto it = four.insert(four.begin() + 2, 34);
    print_vector(four); // 10  9  34  8  7  6  5  4  3
    cout << "*it = " << *it << endl;
    auto it2 = four.insert(four.begin() + 3, 2, 45);
    print_vector(four); // 10  9  34  45  45  8  7  6  5  4  3
    cout << "*it = " << *it2 << "\t*(--it) = " << *(--it2) << endl;

    // erase
    cout << "erase testing ..." << endl;
    auto it3 = four.erase(four.begin() + 2); //删除元素34
    print_vector(four);                      // 10  9  45  45  8  7  6  5  4  3
    cout << "*it3 = " << *it3 << endl;

    auto it4 = four.erase(four.begin() + 2, four.begin() + 4);
    print_vector(four); // 10  9  8  7  6  5  4  3
    cout << "*it4 = " << *it4 << endl;
    //删除four里面的双数元素
    //其实最好的方法，应该还是申请别的内存来存放新的数据，这样保证时间复杂度为O(n);
    //因为erase删除元素，后面的元素会依次搬运上来，这个过程会频繁调用元素的构造函数；
    auto i = four.begin();
    while (i != four.end())
    {
        if ((*i % 2) == 0)
        {
            four.erase(i);
            continue; //这里，因为比如第一个元素10，删除后，i会指向下一个元素9,9此时是begin的元素，所以这里直接continue进入下一次循环
        }
        ++i;
    }
    print_vector(four);

    // emplace
    cout << "emplace testing ..." << endl;
    four.emplace(four.begin(), 900);
    four.emplace(four.begin() + 3, 1000);
    print_vector(four);

    four.emplace_back(*six.begin());
    print_vector(four);
    print_vector(six);
}