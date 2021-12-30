# list
## list简介
list是C++STL提供的一种**循环**双向链表，容器中的元素在内存中顺序依靠内部指针规定，而并非内存上的连续
和array，vector和deque等内存上连续的容器相比，如果获取到了操作元素的迭代器，list提供更好的插入、删除和移动的时间复杂度，而上述提到的容器，查询会更快。
此外STL还提供一种前序list-forward_list，此链表仅能向前遍历，不过内存使用上会更小，速度会更快。

## list成员及方法
- list构造函数
STL提供的构造函数非常多
1. `explicit list (const allocator_type& alloc = allocator_type());`
默认构造函数，构造一个空的容器，没有任何元素
用法：`list<int> l;`
2. `explicit list (size_type n);`或
```C++         
list (size_type n, const value_type& val,
                const allocator_type& alloc = allocator_type());
``` 
构造一个n个元素大小的，值为val（如果提供val的话）的链表
用法：`list<int> l(10)`或`list<int> l(10,0)`
3. 
```C++
template <class InputIterator>
  list (InputIterator first, InputIterator last,
         const allocator_type& alloc = allocator_type());
```
使用迭代器构造，构造出来的list和原有的list[first,last)**内容和顺序**均相同。
用法：
```C++
    list<int> l1(10,1);
    list<int> l2(l1.begin(), l1.begin()+5);
```
l2仅是l1的前5个元素
4. 
```C++
    list (const list& x);
    list (const list& x, const allocator_type& alloc);
```
拷贝构造函数，将链表x的所有元素全部拷贝过来。注意，方式1中是将分配器也一起拷贝，方式二可以自己定义分配器
用法：
```C++
    list<int> l1(10,1);
    list<int> l2(l1);
```
5. 
```C++
list (list&& x);
list (list&& x, const allocator_type& alloc);
```
移动构造函数，传入右值时调用（传入左值将调用拷贝构造），用法和用途与拷贝构造相同
6. 
```C++
list (initializer_list<value_type> il,
       const allocator_type& alloc = allocator_type());
```
列表初始化构造，像vector一样用{}传入数据即可。
用法：`list<int> l{1,2,3,4,5,6,7};`

- 析构函数
将list摧毁

- operator=
等号重载，重载了三次
1. 拷贝
2. 移动
3. 列表初始化
具体概念才考构造函数
```C++
// assignment operator with lists
#include <iostream>
#include <list>

int main ()
{
  std::list<int> first (3);      // list of 3 zero-initialized ints
  std::list<int> second (5);     // list of 5 zero-initialized ints

  second = first;
  first = std::list<int>();

  std::cout << "Size of first: " << int (first.size()) << '\n';
  std::cout << "Size of second: " << int (second.size()) << '\n';
  return 0;
}
```

- 迭代器
仅介绍两种，实在是太多了
1. begin():返回一个指向list第一个元素的迭代器
2. end()：返回一个指向list末尾的迭代器，**并不是指向最后一个元素，而是指向最后一个元素的下一个元素，可以理解为null的元素**

- 容量
1. empty：测试容器是否为空：`l.empty()`如l为空返回true，反之返回false
2. size：返回容器中实际存储元素数量：`l.size()`，如果有10个元素及返回10；
3. max_size：返回容器此时可以储存元素最大值.**由于分配器的存在，当元素不断插入时，若插入元素溢出，分配器会自动扩大链表空间，所以很可能出现size ！= max_size的情况**

- 元素访问
1. front：返回链表第一个元素
2. back：返回链表最后一个元素

- 修改
1. assign:将新内容分配给链表，**会替换当前链表内容，并对应修改其大小max_size和size**
```C++
//范围分配，传入两个迭代器（或者指针）
template <class InputIterator>
  void assign (InputIterator first, InputIterator last);
//类似于构造函数，构造一个n个元素，值为val的链表
void assign (size_type n, const value_type& val);
//类似于列表初始化
void assign (initializer_list<value_type> il);
```
总之，调用assign会将原有的内容全部替换，谨慎使用。
2. emplace_front：构造一个节点并将其插入到beginning，从头部开始插入节点.C++11以后引入
3. push_front：同emplace_front
4. pop_front：删除第一个节点
5. emplcade_back：从后面插入一个元素。C++11以后引入
6. push_back：同emplace_back
7. pop_back：删除最后一个节点
8. emplace：
```C++
template <class... Args>
  iterator emplace (const_iterator position, Args&&... args);
```
在position插入元素（前插法），元素构造是通过allocator_traits::construct（萃取技术）来实现的（此处有点复杂，待后面再完善），用法如下：
```C++
// list::emplace
#include <iostream>
#include <list>

int main ()
{
  std::list< std::pair<int,char> > mylist;

  mylist.emplace ( mylist.begin(), 100, 'x' );
  mylist.emplace ( mylist.begin(), 200, 'y' );

  std::cout << "mylist contains:";
  for (auto& x: mylist)
    std::cout << " (" << x.first << "," << x.second << ")";

  std::cout << '\n';
  return 0;
}
```
输出为：
`mylist contains: (200,y) (100,x)`
mylist.emplace ( mylist.begin(), 100, 'x' );此句后面的100和'x'是用于构造`pair<int, char>`

9. insert：同emplace（据说emplace效率更高）
10. erase：删除某个元素
```C++
//删除某个位置position节点
iterator erase (const_iterator position);
//删除从first到lase的节点，遵循左闭右开原则[first,last)
iterator erase (const_iterator first, const_iterator last);
```
11. swap
```C++
//同stl中的swap，将本链表和x内容交换，size可以不一样
void swap (list& x);
```
12. resize：如果n小于当前容器大小，算法会保留前n个元素；反之，则会扩充至n，至于本身容器size到n之间的数，如果指定了val，会自动赋值；如果没有指定，元素的值只会初始化
```C++
//将链表大小调整为n
void resize (size_type n);
//将链表大小调整为n，并将所有元素赋值为val
void resize (size_type n, const value_type& val);
```
13. clear：清空链表，将size置为0；

- operator操作
1. splice：原意为拼接，在这里意味着将元素传输到链表指定位置，传输后，原链表中的元素将被删除
```C++
//将整个链表x传输到this链表的指定位置position中。
void splice (const_iterator position, list& x);
void splice (const_iterator position, list&& x);
//将x中i指向的元素，传输到指定位置position
void splice (const_iterator position, list& x, const_iterator i);
void splice (const_iterator position, list&& x, const_iterator i);
//x中的[first,last)传输到容器position
void splice (const_iterator position, list&& x,
             const_iterator first, const_iterator last);
```
用法：
```C++
// splicing lists
#include <iostream>
#include <list>

int main ()
{
  std::list<int> mylist1, mylist2;
  std::list<int>::iterator it;

  // set some initial values:
  for (int i=1; i<=4; ++i)
     mylist1.push_back(i);      // mylist1: 1 2 3 4

  for (int i=1; i<=3; ++i)
     mylist2.push_back(i*10);   // mylist2: 10 20 30

  it = mylist1.begin();
  ++it;                         // points to 2

  mylist1.splice (it, mylist2); // mylist1: 1 10 20 30 2 3 4
                                // mylist2 (empty)
                                // "it" still points to 2 (the 5th element)
                                          
  mylist2.splice (mylist2.begin(),mylist1, it);
                                // mylist1: 1 10 20 30 3 4
                                // mylist2: 2
                                // "it" is now invalid.
  it = mylist1.begin();
  std::advance(it,3);           // "it" points now to 30

  mylist1.splice ( mylist1.begin(), mylist1, it, mylist1.end());
                                // mylist1: 30 3 4 1 10 20

  std::cout << "mylist1 contains:";
  for (it=mylist1.begin(); it!=mylist1.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  std::cout << "mylist2 contains:";
  for (it=mylist2.begin(); it!=mylist2.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
输出为：
mylist1 contains: 30 3 4 1 10 20
mylist2 contains: 2

2. remove：移除指定元素
```C++
void remove (const value_type& val);
```

3. remove_if：删除满足条件的所有元素，predicate可以传入函数指针，也可以是仿函数
```C++
template <class Predicate>
  void remove_if (Predicate pred);
```
用法：
``` C++
// list::remove_if
#include <iostream>
#include <list>

// a predicate implemented as a function:函数指针
bool single_digit (const int& value) { return (value<10); }

// a predicate implemented as a class:仿函数
struct is_odd {
  bool operator() (const int& value) { return (value%2)==1; }
};

int main ()
{
  int myints[]= {15,36,7,17,20,39,4,1};
  std::list<int> mylist (myints,myints+8);   // 15 36 7 17 20 39 4 1
  //传入函数指针
  mylist.remove_if (single_digit);           // 15 36 17 20 39
  //传入仿函数
  mylist.remove_if (is_odd());               // 36 20

  std::cout << "mylist contains:";
  for (std::list<int>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
4. unique：删除重复值的节点
```C++
//此版本没有传入参数，函数会保留第一个出现的元素节点，删除其他重复的部分
void unique();
//此版本传入的参数，可以定义“何为相等”，如果predicate返回true，函数将会把该节点删除
template <class BinaryPredicate>
  void unique (BinaryPredicate binary_pred);
```
用法：
```C++
// list::unique
#include <iostream>
#include <cmath>
#include <list>

// a binary predicate implemented as a function:
bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

// a binary predicate implemented as a class:
struct is_near {
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
};

int main ()
{
  double mydoubles[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
                       12.77, 73.35, 72.25, 15.3,  72.25 };
  std::list<double> mylist (mydoubles,mydoubles+10);
  
  mylist.sort();             //  2.72,  3.14, 12.15, 12.77, 12.77,
                             // 15.3,  72.25, 72.25, 73.0,  73.35

  mylist.unique();           //  2.72,  3.14, 12.15, 12.77
                             // 15.3,  72.25, 73.0,  73.35
  //此处传递的函数指针，告诉unique，整数部分相等即可认为相等；对于unique，12,.15和12.77认为是相等的
  mylist.unique (same_integral_part);  //  2.72,  3.14, 12.15
                                       // 15.3,  72.25, 73.0
  //两者相差5.0，
  mylist.unique (is_near());           //  2.72, 12.15, 72.25

  std::cout << "mylist contains:";
  for (std::list<double>::iterator it=mylist.begin(); it!=mylist.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
5. merge：将x合并到链表中，这里采用传输形式；传输方式，意味着x内的元素会被移除，size=0；
期间不调用元素的构造函数和析构函数，传输时，无论是左值还是右值，或者value_type是否支持移动构造，都将被转移。
```C++
//将x通过传输的方式合并到list链表中，调用该函数时，需将两个链表排序；
void merge (list& x);
//通过传入predicate-compare进行排序；comp应具有strict weak ordering（没懂啥意思）
//假设采用comp这种方式，例如一个简单的>，最后的结果能保证x中的个元素在最终的链表中是稳定的（即，相同元素的相对位置没有发生改变），原本链表中的各元素也是如此。
template <class Compare>
  void merge (list& x, Compare comp);
```
用法：
```C++
// list::merge
#include <iostream>
#include <list>

// compare only integral part:
bool mycomparison (double first, double second)
{ return ( int(first)<int(second) ); }

int main ()
{
  std::list<double> first, second;

  first.push_back (3.1);
  first.push_back (2.2);
  first.push_back (2.9);

  second.push_back (3.7);
  second.push_back (7.1);
  second.push_back (1.4);

  first.sort();
  second.sort();

  first.merge(second);
  //此时first = {1.4,2.2,2.9.3.1,3.7,7.1};
  //合并后的first还是按照原来的顺序排列，second的各元素按照从小到大的顺序穿插到first中，并且两个原链表中的各元素的相对位置没有发生改变
  // (second is now empty)

  second.push_back (2.1);

  first.merge(second,mycomparison);

  std::cout << "first contains:";
  for (std::list<double>::iterator it=first.begin(); it!=first.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}
```
6. sort：排序；list是有自己的sort，当容器有自己的sort时，不要在用std::sort；排序结果稳定
```C++
//默认为生序排列
void sort();
//当然可以通过传入仿函数的方式，来自定义排序原则
template <class Compare>
  void sort (Compare comp);
```

7. reverse：翻转链表；将链表整个翻转；没啥好说的
