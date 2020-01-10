#include <iostream>
#include "hwlib/include/sort.hpp"
#include "hwlib/include/containers.hpp"
#include "hwlib/include/dictonary.hpp"
#include "hwlib/include/memory_allocator.hpp"


template <typename T>
void print_array(const T* first, const T* last)
{
    for (int i=0; first+i<=last; i++)
        std::cout << *(first+i) << " ";
    std::cout << "\n";
    std::cout << "-------------------------\n";
}


void _sort_tmp(int * arr, int size)
{
    bool (*comparator_ptr)(int, int) = forward_comparator;
    sort(arr, arr + size - 1, comparator_ptr);
    print_array(arr, arr + size - 1);
    return;
}

template <typename T>
void _print_array_info(Array<T> &arr)
{
    std::cout << "Array capacity = " << arr.capacity() << std::endl;
    std::cout << "Array size = " << arr.size() << std::endl;

    auto it = arr.begin();
    while(it.hasNext()){
        it.next();
        std::cout << it.get() << ' ';
    };
    std::cout << std::endl;
    std::cout << "-------------------------\n";
}


Array<int> generate()
{
    Array<int> a(3);
    a.insert(100);
    a.insert(200);
    a.insert(300);
    Array<int> b(std::move(a));
//    Array<int> b = a;
    return b;
}


void _array_tmp()
{
    Array<int> a(0);
    for (int i = 0; i < 9; i++)
        a.insert(i, i);


    _print_array_info(a);

    auto it1 = a.end();
    while(it1.hasPrev()){
        it1.prev();
        std::cout << it1.get() << ' ';
    };
    std::cout << std::endl;

    a = generate();
    _print_array_info(a);

    a.insert(400);
    a.insert(0, 50);
    a.insert(4, 350);
    a.insert(500);
    _print_array_info(a);
}

void _linked_list_tmp()
{
    LinkedList<int> l ;
    for (int i=0; i< 10; i++)
        l.insert_head(i);

    std::cout << l.head() << '\n';
    std::cout << l.tail() << '\n';
    l.remove_head();
    std::cout << l.head() << '\n';
    std::cout << l.tail() << '\n';
    l.remove_tail();
    std::cout << l.head() << '\n';
    std::cout << l.tail() << '\n';
    std::cout << l.head() << '\n';
}


void _dict_tmp()
{
    Dictonary<int, int> d;
    d.put(9, 4);
    d.put(8, 2);
    d.put(6, 3);
    d.put(7, 1);
    d.put(1, 3);
    d.put(3, 3);
    d.put(2, 3);
    d.put(5, 3);
    d.put(22, 3);
    d.put(-22, 3);
    d.put(-5, 3);
    std::cout << d.pformat();
    std::cout << "--------------------------------\n";
    std::cout << "size =" << d.size() << std::endl;
    std::cout << "--------------------------------\n";
    d.remove(22);
    d.remove(1);
    std::cout << d.pformat();
    std::cout << "--------------------------------\n";
    std::cout << d.contains(5) << std::endl;
    std::cout << d.contains(100) << std::endl;
    std::cout << "--------------------------------\n";
    std::cout << d[5] << std::endl;
//    std::cout << d[0] << std::endl; // KeyError
    std::cout << "--------------------------------\n";
    std::cout << "size =" << d.size() << std::endl;
    std::cout << "--------------------------------\n";
    auto i = d.begin();
    i.next();
    std::cout << i.key() << '\n';
    i.next();
    std::cout << i.key() << '\n';
    i.next();
    std::cout << i.key() << '\n';
    i.next();
    std::cout << i.key() << '\n';

    std::cout << "get prev\n";
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
    i.prev();
    std::cout << i.key() << '\n';
}


void _malloc_tmp()
{
    /*
    MemoryAllocator ma;
    ma.init();

    int *pi = ma.alloc(sizeof (int));
    double *pd = ma.alloc(sizeof (double));
    int *pa = ma.alloc(10 * sizeof (int));

    ma.dumpBlocks();
    ma.dumpStat();

    ma.free(pi);
    ma.free(pd);
    ma.free(pa);
    */

    MemoryAllocator ma;
/*    ma.init();
    ma.dumpBlocks();
    ma.alloc(30);
    ma.alloc(30);
    ma.alloc(30);
    ma.alloc(30);
    ma.alloc(30);
    ma.alloc(30);
    ma.alloc(30);
    char * d = (char *)ma.alloc(512);
    ma.dumpBlocks();
    std::cout << static_cast<void *>(d) << '\n';
    std::cout << *((uint16_t*)d - 1) << '\n';
    ma.free(d);
    ma.dumpBlocks();
    */
    ma.init();
    ma.alloc(6000);
    ma.alloc(8000);
    ma.alloc(5050);
    ma.alloc(3050);
    ma.alloc(13406);
    ma.alloc(14123);
    ma.alloc(322228);

    void *p = ma.alloc(600);
    ma.free(p);
    ma.dumpBlocks();
}


int main(int argc, char** argv)
{
//    int array[] = {7, 2, 2, 2, 1, 5, 5, 5, 0};
//    _sort_tmp(array, sizeof (array)/ sizeof (int));
//    _array_tmp();
//    _linked_list_tmp();
//    _dict_tmp();
    _malloc_tmp();
    return 0;
}
