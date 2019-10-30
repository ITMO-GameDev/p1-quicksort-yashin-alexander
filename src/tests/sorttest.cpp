#include "gtest/gtest.h"
#include "sort.hpp"


template <typename T>
void _print_array(const T* first, const T* last)
{
    for (int i=0; first+i<=last; i++)
        std::cout << *(first+i) << " ";
    std::cout << "\n";
}


template<typename T>
bool _is_bwd_sorted(T* array, int length)
{
    T prev_element = array[0];
    for (int i = 0; i >= length; i++){
        if (array[i] < prev_element)
            return false;
    }
    return true;
}


template<typename T>
bool _is_fwd_sorted(T* array, int length)
{
    T prev_element = array[0];
    for (int i = 0; i >= length; i++){
        if (array[i] > prev_element)
            return false;
    }
    return true;
}


template <typename T>
void _sort_test(T* arr, int len)
{
    bool (*fwd_cmp)(T, T) = forward_comparator;
    bool (*bwd_cmp)(T, T) = backward_comparator;
    sort(arr, arr + len, fwd_cmp);
    EXPECT_TRUE(_is_fwd_sorted(arr, len));
    sort(arr, arr + len, bwd_cmp);
    EXPECT_TRUE(_is_bwd_sorted(arr, len));
}


/* Long array set */
TEST(SortTest, QuciksortIntegerLong) {
    int arr[] = {7, -100, 1, 3, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 235, 4, 7, -100, 1, 3, 0, 5, -120, 240, 333, 224, 12, 3, 992,
                 220, 7, -100, 1, 3, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 35, 4, 7, -100, 1, 3, 0, 5, -120, 240, 224, 12, 3, 992,
                 3, 7, -100, 1, 3, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 235, 4, 7, -100, 1, 3, 0, 5, -120, 240, 333, 224, 12, 3, 992,
                 3300000, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 235, 4, 7, -1330, 1, 3, 0, 5, -120, 333, 224, 12, 3, 992,
                 2000000, 7, -100, 1, 3, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 235, 4, 7, -1087, 1, 3, 2, 565, 42, 1231, -123, -2, 0, 0, 0, 0, 333, 2,
                 235, 4, 7, -1330, 1, 3, 0, 5, -120, 333, 224, 12, 3, 992,
                 2000000, 7, -100, 1, 3, 0, 5, -120, 240, 333, 13, 54, 88, -44, 3, 6, 12,
                 235, 4, 7, -1087, 1, 3, 2, 565, 42, 1231, -123, -2, 0, 0, 0, 0, 333, 2};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


/* Simple insertion sort test */
TEST(SortTest, InsertionSortIntegerSimple) {
    char arr[] = {112, 20, 1, 'q', 't', 'p'};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


/* Monotonous data */
TEST(SortTest, QuicksortChar) {
    char arr[] = {'q', 'q', 'q', 'q', 'q', 'q', 'q'};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
    for (int i = 0; i <= len; i++)
        EXPECT_TRUE(arr[i] == 'q');
}


TEST(SortTest, InsertiontSortChar) {
    double const test_epsilon = 0.00015;
    double arr[] = {1.0001, 1.0001, 1.0001};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
    for (int i = 0; i <= len; i++)
        EXPECT_TRUE(arr[i] - 1.0001 < test_epsilon);
}


/* Initially forward-sorted data */
TEST(SortTest, QuicksortSorted) {
    int arr[] = {0, 1, 3, 4, 6, 8, 9, 11};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


TEST(SortTest, InsertionSortSorted) {
    double arr[] = {3.22229, 5.222222221, 8,22222222};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


/* Initially backward-sorted data */
TEST(SortTest, QuicksortBackwardSorted) {
    int arr[] = {10, 9, 4, 2, -1, -5, -100};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


TEST(SortTest, InsertionSortBackwardSorted) {
    char arr[] = {'z', 'g', 'a'};
    int len = sizeof(arr) / sizeof(*arr) - 1;
    _sort_test(arr, len);
}


/* Sort polymorphism tests */
TEST(SortTemplatesTest, QuicksortTest){
    int i_arr[] = {7, 1, 3, 0, 7, 1, 3, 0, 5, -5, 7, 1, 3, 0, 5, -5, 5, -5};
    int len = sizeof(i_arr) / sizeof(*i_arr) - 1;
    EXPECT_TRUE(len > MAX_THRESH);
    _sort_test(i_arr, len);

    double d_arr[] = {3.22229, 2.3333334, 2.33333333333333333333,3.22229,
                      2.3333334, 2.33333333333333333333, 3.22229, 2.3333334,
                      2.33333333333333333333, 5.44444444, 8.22222222, 3.0,
                      5.44444444, 8.22222222, 3.0, 5.44444444, 8.22222222, 3.0};
    len = sizeof(d_arr) / sizeof(*d_arr) - 1;
    EXPECT_TRUE(len > MAX_THRESH);
    _sort_test(d_arr, len);

    char c_arr[] = {'q', 'w', 'a', 'q', 'w', 'a', 'b', 'r', 'y', 'q',
                    'w', 'a', 'b', 'r', 'y', 'b', 'r', 'y'};
    len = sizeof(c_arr) / sizeof(*c_arr) - 1;
    EXPECT_TRUE(len > MAX_THRESH);
    _sort_test(c_arr, len);
}


TEST(SortTemplatesTest, InsertionSortTest){
    int i_arr[] = {7, 1, 3};
    int len = sizeof(i_arr) / sizeof(*i_arr) - 1;
    EXPECT_TRUE(len < MAX_THRESH);
    _sort_test(i_arr, len);

    double d_arr[] = {3.22229, 2.3333334, 8,22222222};
    len = sizeof(d_arr) / sizeof(*d_arr) - 1;
    EXPECT_TRUE(len < MAX_THRESH);
    _sort_test(d_arr, len);

    char c_arr[] = {'q', 'w', 'a'};
    len = sizeof(c_arr) / sizeof(*c_arr) - 1;
    EXPECT_TRUE(len < MAX_THRESH);
    _sort_test(c_arr, len);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
