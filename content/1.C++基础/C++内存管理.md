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
这里会比较乱，是因为之前说到placement new 是不分配空间，将之前分配好的空间传入new函数，主动调用构造函数。但是，实际上我们说的重载placement new是重载operator new，然后通过placement new的形式去调用例如：
```C++
static void *foo::operator new(size_t size, long extra)
{
    cout << "call new with long ...\n";
    return (foo *)malloc(size + extra);
}
//调用new时
foo* p5 = new((long)100) foo();
//这样采用了placement new的形式，实际上是调用的void *foo::operator new函数，进行内存空间分配
//然后foo()这样调用了默认构造函数
```

我们可以重载 class menber operator()，写出多个版本，前提是每个版本的声明都必须有**独特的参数列表**，其中第一个参数必须是size_t（因为我们需要指定大小），其余参数可以任意指定。
具体例子见new.cpp

只有在plcament new调用构造函数跑抛出异常，才会调用起重载的delete。但是在GNUC4.9版本的情况下，抛出异常会直接导致程序中断；
按理来说，重载了一个operator new就应该再手动实现一个版本的operator delete，但是如果我们不实现的话，编译器也不会报错，所以重载时，需要程序员自觉实现

### 简单的内存池
具体操作见Screen.cpp，我们为Screen类开辟了内存池，避免了malloc分配空间时的浪费。这是第一个版本，这个版本中，除了数据m_id，还多声明了一根指针，作为内存池的链表的next指针，这造成了空间的浪费，所以衍生出第二版，见Airplane.cpp


### static allocator
内存池，目前还在类里面实现，但是如果类声明多了，代码难免重复了。所以，我们写一个allocator来统一分配内存，至于应用的class，不需要去操作内存分配，只是写好自己的代码即可。


## std::allocator
标准库中的分配器，是如何分配内存的。

### VC6的allocator、BC5的allocator和GNUC4.9的allocator
都没有对allocator的申请内存和释放内存做特殊处理，只是简单的malloc和free，没有进行良好的设计。因此每次申请内存都会带着cookie，导致在大量申请内存时，会浪费内存

### 更好的allocator
gunc编译器帮我们实现了一个更好的alloctor——`__pool_alloc`;这个不是标准库里面的allocator，想要使用它，需要：
`#include <ext/pool_allocator.h>`
使用说明：
```C++
std::vector<int, __gnu_cxx::__pool_alloc<int>> a;
```
在容器本来应该放入分配器的位置放入这个更好的分配器即可。
这是更好的分配器，因为它采用了内存池设计，避免频繁使用malloc浪费内存。其设计特点有以下几点：
#### __pool_alloc的源代码
__pool_alloc继承自下面这个类
```C++
    class __pool_alloc_base
    {
    protected://这些都是常量，建议常量还是用const的形式来定义
      enum { _S_align = 8 };
      enum { _S_max_bytes = 128 };
      enum { _S_free_list_size = (size_t)_S_max_bytes / (size_t)_S_align }; //128/8=16
      //关键的嵌入式指针设计
      union _Obj
      {
	union _Obj* _M_free_list_link;
	char        _M_client_data[1];    // The client sees this.
      };
      //_pool_alloc维护了一个链表，该链表长度为16；
      static _Obj* volatile         _S_free_list[_S_free_list_size];
      // Chunk allocation state.
      //内存池的管理指针
      static char*                  _S_start_free;  //池子的start指针
      static char*                  _S_end_free;    //池子的结束指针
      static size_t                 _S_heap_size;   //目前在堆上分配的大小
      //向上取整函数，目前该函数取整到8
      size_t
      _M_round_up(size_t __bytes)
      { return ((__bytes + (size_t)_S_align - 1) & ~((size_t)_S_align - 1)); }
      //该函数目前看不到，推测应该是，根据客户需要的内存字节大小，来推测属于哪个链表管理
      //例如，客户需要16个字节，应该用_S_free_list[1]来管理
      _GLIBCXX_CONST _Obj* volatile*
      _M_get_free_list(size_t __bytes) throw ();
        //线程相关的锁
      __mutex&
      _M_get_mutex() throw ();

      // Returns an object of size __n, and optionally adds to size __n
      // free list.refill会申请一大块内存，里面会调用_M_allocate_chunk
      void* _M_refill(size_t __n);
      
      // Allocates a chunk for nobjs of size size.  nobjs may be reduced
      // if it is inconvenient to allocate the requested number.
      char* _M_allocate_chunk(size_t __n, int& __nobjs);
    };

    //实际上的allocate和deallocate
template<typename _Tp> _GLIBCXX_NODISCARD _Tp*
__pool_alloc<_Tp>::allocate(size_type __n, const void*)
    {
    //pointer是之前typedef过得，即_TP*
      pointer __ret = 0;
      if (__builtin_expect(__n != 0, true)) //__builtin_expect是一种加快操作系统的技巧，没有太多逻辑意义
	{   //判断如果n!=0的，进入以下逻辑
        //这里可以看出allocator接受的是分配的元素个数，然后计算需要的内存大小
	  const size_t __bytes = __n * sizeof(_Tp);
        //如果分配的大小超过了128字节，就直接new一块内存给外面，因为内存池只管理8-128的字节大小
	  if (__bytes > size_t(_S_max_bytes))
	    __ret = static_cast<_Tp*>(::operator new(__bytes));
	  else
	    {   //此处开始在内存池中进行分配
        //先根据bytes大小计算落在哪根链表上
	      _Obj* volatile* __free_list = _M_get_free_list(__bytes);
            //如果该链表还没有分配到内存，就调用refill申请内存
	      _Obj* __restrict__ __result = *__free_list;
	      if (__builtin_expect(__result == 0, 0))
		__ret = static_cast<_Tp*>(_M_refill(_M_round_up(__bytes)));
	      else  //如果该链表下还有未使用的内存，就分配出去
		{
		  *__free_list = __result->_M_free_list_link;
		  __ret = reinterpret_cast<_Tp*>(__result);
		}
	      if (__ret == 0)
		std::__throw_bad_alloc();
	    }
	}
      return __ret;
    }

template<typename _Tp>
void __pool_alloc<_Tp>::deallocate(pointer __p, size_type __n)
    {
      if (__builtin_expect(__n != 0 && __p != 0, true))
	{
	  const size_t __bytes = __n * sizeof(_Tp);
	  if (__bytes > static_cast<size_t>(_S_max_bytes))  //如果不在字节大小管理范围内，就直接delete
	    ::operator delete(__p);
	  else
	    {   
        //如果在管理范围内，只是把还回来的内存，放在属于它的链表的头位置，并不会还给操作系统
	      _Obj* volatile* __free_list = _M_get_free_list(__bytes);
	      _Obj* __q = reinterpret_cast<_Obj*>(__p); //嵌入式指针，借用还回来的内存的前n个字节作为链表的next指针，只是这里的next指针名称叫做_M_free_list_link
            //这里就是把还回来的p放在链表的最前面
	      __q ->_M_free_list_link = *__free_list;
	      *__free_list = __q;
	    }
	}
    }
```

#### 嵌入式指针 embedded pointer
在分配器中的内存池设计中，有一点非常巧妙，即嵌入式指针；利用没有使用的内存的前n位，存在链表的next地址。
```C++
//关键的嵌入式指针设计
union _Obj
{
	union _Obj* _M_free_list_link;
	char        _M_client_data[1];    // The client sees this.
};
```
这里为什么可以当做嵌入式指针，见embedded.cpp

#### gunc的__pool_alloc实现的小细节
```C++
//这是一个向上取整的函数，_S_align=8；
//传入的参数bytes会向上取到8的倍数
size_t _M_round_up(size_t __bytes)
    { return ((__bytes + (size_t)_S_align - 1) & ~((size_t)_S_align - 1)); }
```
解析如下：
1. 例如传入的bytes=19，二进制为：10011(这里省略了前导0)
2. __bytes   + (size_t)_S_align - 1，其中_S_align=8，所以转换成二进制为：
10011(19) +  0111(7) = 11010;
3. ~(size_t)_S_align - 1)，转换成二进制为:
1000(前导所有都是1)
4. 上面两个再与一下：
(前导全是0)11010 & (前面全是1)11000 = x011000(24);

## malloc/free



