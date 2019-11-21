#include <iostream>
#include "hwlib/include/sort.hpp"
#include "hwlib/include/containers.hpp"


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


int main(int argc, char** argv)
{
//    int array[] = {7, 2, 2, 2, 1, 5, 5, 5, 0};
//    _sort_tmp(array, sizeof (array)/ sizeof (int));
//    _array_tmp();
    _linked_list_tmp();
    return 0;
}
