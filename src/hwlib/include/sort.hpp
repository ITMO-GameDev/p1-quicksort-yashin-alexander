#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <iostream>
#include <memory>

using namespace std;

const int MAX_THRESH = 4;


template<typename T>
T _mediana(T *first, T *second, T *third)
{
    if (*first <= *second)
        {
            if (*third >= *second)
                return *second;
            else if (*third >= *first)
                return *third;
            else
                return *first;
        }
    else
    {
        if (*second >= *third)
            return *second;
        else if (*third <= *first)
            return *third;
        else
            return *first;
    }
}


template <typename T>
T _get_pivot(T *first, T *last)
{
    T* middle_element_index;
    middle_element_index = first + ((last - first) / 2);
    return _mediana(first, middle_element_index, last);
}


template<typename T, typename ComparatorFunc>
int _hoare_partition(T *first, T *last, ComparatorFunc comparator)
{
    int i = -1;
    int j = last - first + 1;
    T pivot = _get_pivot(first, last);

    while (true){
        do { i++; }
        while (comparator(first[i], pivot));

        do { j--; }
        while (comparator(pivot, first[j]));

        if (i >= j)
            return j;
        std::swap(first[i], first[j]);
    }
}


template<typename T, typename ComparatorFunc>
void quicksort(T *first, T *last, ComparatorFunc comparator)
{
    while (last > first)
    {
        int pivot = _hoare_partition(first, last, comparator);

        if (&first[pivot] - first < last - &first[pivot])
        {
            quicksort(first, &first[pivot], comparator);
            first = &first[pivot + 1 ];
        }
        else
        {
            quicksort(&first[pivot + 1], last, comparator);
            last = &first[pivot];
        }
    }
}


template<typename T, typename ComparatorFunc>
void insertion_sort(T *first, T *last, ComparatorFunc comparator)
{
    T tmp;
    int previous_element_index;

    for (int cnt = 1; cnt < last - first + 1; cnt++)
    {
        tmp = first[cnt];
        previous_element_index = cnt - 1;
        while (previous_element_index >= 0 && comparator(tmp, first[previous_element_index]))
        {
            first[previous_element_index + 1] = first[previous_element_index];
            first[previous_element_index] = tmp;
            previous_element_index--;
        }
    }
}


template<typename T, typename ComparatorFunc>
void sort(T *first, T *last, ComparatorFunc comparator)
{
    if (last - first < MAX_THRESH)
        insertion_sort(first, last, comparator);
    else
        quicksort(first, last, comparator);
}


template <typename T>
bool forward_comparator(T a, T b)
{
    return a < b;
}


template <typename T>
bool backward_comparator(T a, T b)
{
    return a > b;
}


#endif // QUICKSORT_HPP
