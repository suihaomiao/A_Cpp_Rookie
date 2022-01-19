# vector
## vector简介
C—++标准库提供了一个动态长度的数组类型，不断push数据，容器的长度可以持续增长。
就像数组一样，vecor的元素在内存中都是按照顺序排列的，所以我们可以按照数组的offset的方式来访问数据([])，但是不像普通数组，vector的size可以动态变化

通过源代码看到，vector里面没有具体的数据，容器通过三根指针来管理分配器分配的一堆内存，分别为：
- start:指向第一个元素的地址
- finish：指向最后一个元素的地址
- end_of_storage：指向分配的内存末尾
通过三根指针的操作，可以快速访问到任意位置的元素

## vector的成员及方法
- 构造函数
1. 默认构造函数，构造一个空的容器，里面没有元素
```C++
explicit vector (const allocator_type& alloc = allocator_type());
```

2. 声明个数的构造函数
```C++
//这个函数虽然没有指定，但是如果是int等整数类型的话，会默认帮我们构造成全是0的容器
explicit vector (size_type n);
         vector (size_type n, const value_type& val,
                 const allocator_type& alloc = allocator_type());
```

3. 范围构造函数，根据传入的迭代器的的范围来构造vector,且可以自己指定分配器
```C++
//
template <class InputIterator>
  vector (InputIterator first, InputIterator last,
          const allocator_type& alloc = allocator_type());
```

4. 拷贝构造函数，可将元素类型和分配器类型一起拷贝
```C++
vector (const vector& x);
vector (const vector& x, const allocator_type& alloc);
```

5. 移动构造函数，将x的元素搬运到新的容器中，其实只是把指针赋值过来，内存没有变化
```C++
vector (vector&& x);
vector (vector&& x, const allocator_type& alloc);
```

6. 列表初始化构造容器，检测到大括号，编译器会将大括号内的元素转化成一个array来初始化容器
```C++
vector (initializer_list<value_type> il,
       const allocator_type& alloc = allocator_type());
```

- 析构函数，摧毁容器中的各种元素，释放容器本身分配的内存，还会调用容器元素的析构函数，释放元素内可能占用的内存

- 拷贝复制函数
```C++
//左值版本
vector& operator= (const vector& x);
//右值版本
vector& operator= (vector&& x);
//列表初始化
vector& operator= (initializer_list<value_type> il);
```

- 迭代器
迭代器类型太多，就简单描述两个重要的就好
1. begin();根据原理，会返回start指针指向的内存，只是返回的是迭代器，迭代器是一种泛化的指针（可以简单理解为指针）
2. end();根据原理，会返回finish指针指向的内存，同样，返回的是迭代器

- 容量
1. size();对于`vector<T> a`,返回的是容器已存放的元素大小，注意，因为vector会动态分配内存，可能分配的内存比已存放的元素所使用的的内存要大；其实返回的应该是：`(start-finish)/sizeof(T)`
2. max_size();返回最大可存放元素个数：这是个非常大的数，可以理解为vector容器能容纳的最大元素个数
3. resize(size_t n);将容器重新分配空间，因为vector必须是连续空间，所以原来的内存将还给操作系统；但是内部的元素会copy过来；
```C++
void resize (size_type n);
void resize (size_type n, const value_type& val);   //该函数可接受val
```
这里有几种情况：
如果n小于现在的容器的大小，只会copy前n个元素，超出的部分将丢失
如果n大于现在的容器大小，编译器会保留本身容器的元素，超出的部分会用默认值(比如对于int 是0来代替)，如果指定了值val将会用val代替
4. capacity():将返回目前能存在的最大元素个数：`(start-end_of_storage)/sizeof(T)`;
5. empty()：返回是否是空容器，如果是返回true，反之返回false
6. reserve(size_t n):请求更改容器的容量，即capacity；在vector中容量会随着元素个数增加而增加，但是我们可以手动更改容器的容量
如果n大于容器现在的容量，则会扩容容器的容量到n，超出的元素将会用默认值代替
其他情况下，容器不会改变。
总之，该函数不会更改现在容器内的元素，也不会改变现有容器的size()
7. shrink_to_fit():将容器的容量更改到size大小；
总之，该函数不会更改现在容器内的元素，也不会改变现有容器的size()

- 元素访问
1. [n]：类似于数组的访问元素形式，在目前的编译器下测试，capacity() = 20;但是，访问第300个元素，依然没有报错
2. at(n):和1类型，不过at进行边界检查，不会报错
3. front():返回第一个元素，是直接返回元素本身的引用，不像begin()返回迭代器。**如果容器是空，调用该接口会发生错误**
4. back():返回最后一个元素，解释和front一样
5. data()：返回一个指针，指向首元素，就像C语言的数组一样操作即可。

- 修改元素
1. assign():将新元素赋值给容器，代替现在的内容，并自动修改容器大小。
```C++
//传入迭代器版本
template <class InputIterator>
  void assign (InputIterator first, InputIterator last);
//传入个数和指定值版本
void assign (size_type n, const value_type& val);
//列表初始化版本
void assign (initializer_list<value_type> il);
```
2. push_back(T):将新元素添加到容器尾部 
3. pop_back():移除最后一个元素
4. insert():插入元素
```C++
//在迭代器位置，插入单个元素，返回指向插入元素的迭代器
iterator insert (const_iterator position, const value_type& val);
//在迭代器位置，插入多个相同元素，返回指向第一个元素的迭代器
iterator insert (const_iterator position, size_type n, const value_type& val);
//在迭代器位置，插入[first,last)范围的元素，返回指向插入的first迭代器
template <class InputIterator>
iterator insert (const_iterator position, InputIterator first, InputIterator last);
//单个元素插入的右值版本
iterator insert (const_iterator position, value_type&& val);
```
5. erase():删除元素，可以单出单个元素或者范围元素,返回的是position
```C++
//返回position，因为原来position的元素被删除了，剩下的元素会复制到该位置，即原来position+1的元素
iterator erase (const_iterator position);
iterator erase (const_iterator first, const_iterator last);
```
注：在用迭代器遍历vector时，erase删除元素，会导致迭代器指向下一个元素，然后for循环进行++时，会跳过该元素，此时需要特殊处理一下。
6. swap(vector)：交换vector和本身容器的元素，其实此时只是交换了三根指针，内存没有交换
7. clear()：清空元素，释放内存，将size和capacity都置0；
8. emplace():插入元素，和insert功能类似，但是emplace是一个右值版本，所以会去“偷”原来的资源
```C++
//可变模板参数
template <class... Args>
iterator emplace (const_iterator position, Args&&... args);
```
9. emplace_back():在容器末尾添加元素，这是一个右值版本，意思是建议编译器尽量把它当做右值来看待；实际上push_back(&&)右值版本调用的就是emplace_back;
经过测试，实际上，传给emplace_back一个左值，他也不会强制去“偷”资源，它还是会复制一份到容器末尾

