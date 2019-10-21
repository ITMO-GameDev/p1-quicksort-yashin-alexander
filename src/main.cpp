#include <iostream>
#include "hwlib/include/sort.hpp"


template <typename T>
void print_array(const T* first, const T* last)
{
    for (int i=0; first+i<=last; i++)
        std::cout << *(first+i) << " ";
    std::cout << "\n";
}


void _sort(int * arr, int size)
{
    bool (*comparator_ptr)(int, int) = forward_comparator;
    sort(arr, arr + size - 1, comparator_ptr);
    print_array(arr, arr + size - 1);
    return;
}


int main(int argc, char** argv)
{
    int array[] = {7, 2, 2, 2, 1, 5, 5, 5, 0};
    _sort(array, sizeof (array)/ sizeof (int));
    return 0;
}
