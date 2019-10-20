#ifndef QUICKSORT_HPP
#define QUICKSORT_HPP

#include <iostream>
#include <memory>

using namespace std;


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
        while (first[i] < pivot);

        do { j--; }
        while (first[j] > pivot);

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
            quicksort(first, &first[pivot - 1], NULL);
            first = &first[pivot + 1];
        }
        else
        {
            quicksort(&first[pivot + 1], last, NULL);
            last = &first[pivot - 1];
        }
    }
}

#endif // QUICKSORT_HPP
