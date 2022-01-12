# C++内存管理

## C++的memory primitives 内存的基本工具
C++中可使用的内存分配工具：
|分配|释放|归属|可否重载|
|---|---|---|---|
|malloc()|free()|C语言函数|不可重载|
|new|delete|C++表达式|不可重载|
|::operator new()|::operator delete()|C++函数|可重载|
|allocator<T>::allocate()|allocator<T>::deallocate()|C++标准库|可自由设计并搭配容器使用|
以上几种的简单用法，见memory_using.cpp

### new和delete表达式
表达式new在实际过程中可以被分解为三步：（其中第一步和第二步可以合在一起）
1. 调用operator new()申请内存空间
2. 将指针转换成目标指针
3. 调用对象的构造函数
注意：这一切都是编译器帮我们生成的，不可以手动调用构造函数。但是，我们可以手动实现operator new()这个函数，来实现某些特性。

表达式delete在实际过程可以分别为两步：
1. 调用对象的析构函数
2. 调用operator delete释放内存

### array new和array delete
在创建数组时，需要调用array new 和array delete。
对于array new来说，会分别调用数次对象的构造函数。
```C++
//
complex* pca = new complex[3];
//此处需要特别注意，需要调用delete[]
delete[] pca;
```
对于array delete来说，使用了array new必须对应array delete，否则会造成内存泄漏.
**为什么会造成内存泄漏：**
首先对于普通的类，例如int或者complex这种没有指针对象的，或者说析构函数没有意义的类，只是调用delete不会造成内存泄漏，因为在free后，会把整个分配的空间都还给操作系统。
但是对于带指针的类，例如string这种析构函数有重要意义的，只是调用delete，只会调用一个对象的析构函数（该对象的析构函数内可能对之前动态分配的空间进行了delete），其他对象的析构函数没有调用，即其他对象动态分配的空间没有释放，这样才是造成了内存泄漏。

补充：
对于array new 和array delete，每次分配完内存，还会在实际分配的内存上加上一4个字节的内存，这段内存会记录本次分配了多少块，所以，在我们调用array delete时，我们不需要告诉编译器array new的时候分配了多少.


### placement new
placement new允许我们将对象构造于allcated memoray(已经分配好的内存)
```C++
//先分配好内存
char* buf = new char[sizeof(Complex) *3];
//再调用placement new，显式调用构造函数，将对象构建在buf上
Complex* pc = new(buf)Complex(1,2);
//最后将buf delete
delete[] buf;
//上面的placement new的代码，也可以翻译成下面这样：（注意，只是相当于，实际上不能这么写）
void* mem  = operator new (sizeof(Complex), buf);
//这个operator new相当于进行了重载形式
//不进行内存分配，如下，只是直接返回了p
//void* operator new(size_t size, void* p) {return p;}
pc = static_cast<Complex*>(mem);
pc->Complex::Complex(1,2);
```
实际上没有placement delete，因此placement new 没有分配内存，所以不需要placement delete；但是重载了placement new就需要对应写一个delete，也可以把这个delete叫做placement delete

重载placement new
我们可以重载 class menber operator()，写出多个版本，前提是每个版本的声明都必须有**独特的参数列表**，其中第一个参数必须是size_t（因为我们需要指定大小），其余参数可以任意指定。
具体例子见new.cpp

只有在plcament new调用构造函数跑抛出异常，才会调用起重载的delete。但是在GNUC4.9版本的情况下，抛出异常会直接导致程序中断；
按理来说，重载了一个operator new就应该再手动实现一个版本的operator delete，但是如果我们不实现的话，编译器也不会报错，所以重载时，需要程序员自觉实现
