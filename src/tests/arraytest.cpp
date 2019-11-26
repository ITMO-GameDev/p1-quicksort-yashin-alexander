#include "gtest/gtest.h"
#include "containers.hpp"


template <typename T>
void _fill_test_arrays(Array<T>& tested, std::vector<T>& to_cmp, int size)
{
    for (T i = 0; i < size; i++)
    {
        tested.insert(i);
        to_cmp.push_back(i);
    }
}


template <typename T>
bool _array_content_eq(Array<T>& arr_to_check, std::vector<T> to_cmp)
{
    int i = 0;
    auto it = arr_to_check.begin();
    while(it.hasNext()){
        it.next();
        if (it.get() != to_cmp.at(i++))
            return false;
    }
    return true;
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



template <typename T>
Array<T> generate_array_via_move(std::vector<T>& values_container)
{
    Array<int> a(0);
    for (int i = 0; i < values_container.size(); i++)
        a.insert(values_container.at(i));
    Array<int> b(std::move(a));
    return b;
}


template <typename T>
Array<T> generate_array_via_copy(std::vector<T>& values_container)
{
    Array<int> a(10);
    for (int i = 0; i < values_container.size(); i++)
        a.insert(values_container.at(i));
    Array<int> b = a;
    return b;
}


TEST(ArrayTest, NullCapacityCtor)
{
    Array<int> a(0);
    EXPECT_EQ(a.capacity(), 0);
}


TEST(ArrayTest, CapacityGetter)
{
    int size = 10;
    Array<int> a(size);
    EXPECT_EQ(a.capacity(), size);
}


TEST(ArrayTest, SizeGetter)
{
    Array<int> a(0);
    std::vector<int> a1;
    int size = 10;

    EXPECT_EQ(a.size(), 0);
    _fill_test_arrays(a, a1, size);
    EXPECT_EQ(a.size(), size);
}


TEST(ArrayTest, Insertion)
{
    Array<int> a(8);
    std::vector<int> a1;
    int size = 6;

    EXPECT_EQ(a.capacity(), 8);
    _fill_test_arrays(a, a1, size);
    EXPECT_EQ(a.capacity(), 8);
    EXPECT_EQ(a.size(), size);
    EXPECT_TRUE(_array_content_eq(a, a1));
}


TEST(ArrayTest, InsertionWithExpansion)
{
    Array<int> a(0);
    std::vector<int> a1;
    int size = 6;

    _fill_test_arrays(a, a1, size);
    EXPECT_EQ(a.size(), size);
    EXPECT_TRUE(_array_content_eq(a, a1));
}


TEST(ArrayTest, InsertionWithMultipleExpansion)
{
    Array<int> a(0);
    std::vector<int> a1;
    int size = 1000;

    _fill_test_arrays(a, a1, size);
    EXPECT_EQ(a.size(), size);
    EXPECT_TRUE(_array_content_eq(a, a1));
}


TEST(ArrayTest, IndexedInsertion)
{
    Array<int> a(0);
    std::vector<int> a1{1, 2, 3, 4, 5, 6};
    a.insert(1);
    a.insert(3);
    a.insert(6);
    a.insert(1, 2);
    a.insert(3, 5);
    a.insert(3, 4);

    EXPECT_EQ(a.size(), 6);
    EXPECT_TRUE(_array_content_eq(a, a1));
}


TEST(ArrayTest, TestSimpleMoveCtor)
{
    Array<int> a(0);
    std::vector<int> a1{100, 200, 300, 400, 500, 600};

    a = generate_array_via_move(a1);
    EXPECT_TRUE(_array_content_eq(a, a1));
    EXPECT_EQ(a.size(), 6);
}


TEST(ArrayTest, TestCopyCtor)
{
    Array<int> a(0);
    std::vector<int> a1{700, 500, 200, 333, 123, 444};

    a = generate_array_via_copy(a1);
    EXPECT_TRUE(_array_content_eq(a, a1));
    EXPECT_EQ(a.size(), 6);
}


TEST(ArrayTest, Templates)
{
    Array<char> a(0);
    std::vector<char> a1;
    int size = 120;

    _fill_test_arrays(a, a1, size);
    EXPECT_EQ(a.size(), size);
}
