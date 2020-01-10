#include <unistd.h>
#include "gtest/gtest.h"
#include "memory_allocator.hpp"


void assert_memory_alloc(MemoryAllocator &ma, size_t size, uint16_t expected_size)
{
    void * p = ma.alloc(size);
    EXPECT_EQ(((uint16_t *)p)[-1], expected_size);
    ma.free(p);
}

TEST(MemoryAllocatorTest, TestSimple16BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    assert_memory_alloc(ma, 5, FSA_SEGMENT_SIZE_16);
}

TEST(MemoryAllocatorTest, Test16BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    for (int i = 1; i <= 16; i ++)
        assert_memory_alloc(ma, i, FSA_SEGMENT_SIZE_16);
}


TEST(MemoryAllocatorTest, Test32BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    for (int i = 17; i <= 32; i++)
        assert_memory_alloc(ma, i, FSA_SEGMENT_SIZE_32);
}


TEST(MemoryAllocatorTest, Test64BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    for (int i = 33; i <= 64; i++)
        assert_memory_alloc(ma, i, FSA_SEGMENT_SIZE_64);
}


TEST(MemoryAllocatorTest, Test128BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    assert_memory_alloc(ma, 65, FSA_SEGMENT_SIZE_128);
    assert_memory_alloc(ma, 100, FSA_SEGMENT_SIZE_128);
    assert_memory_alloc(ma, 128, FSA_SEGMENT_SIZE_128);
}


TEST(MemoryAllocatorTest, Test256BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    assert_memory_alloc(ma, 129, FSA_SEGMENT_SIZE_256);
    assert_memory_alloc(ma, 200, FSA_SEGMENT_SIZE_256);
    assert_memory_alloc(ma, 256, FSA_SEGMENT_SIZE_256);
}


TEST(MemoryAllocatorTest, Test512BytesAlloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    assert_memory_alloc(ma, 257, FSA_SEGMENT_SIZE_512);
    assert_memory_alloc(ma, 400, FSA_SEGMENT_SIZE_512);
    assert_memory_alloc(ma, 512, FSA_SEGMENT_SIZE_512);
}


TEST(MemoryAllocatorTest, TestBlockRealloc){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    void * first_alloc_ptr = ma.alloc(10);
    ma.free(first_alloc_ptr);

    uint8_t * realloc_ptr = ((uint8_t *)((void *)ma.alloc(2)) + 2 * FSA_SEGMENT_SIZE_16);
    EXPECT_EQ(first_alloc_ptr, realloc_ptr);
}


TEST(MemoryAllocatorTest, TestGetManySmallAllocs){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();

    void * first_alloc_ptr;
    void * realloc_ptr;

    for (int i = 0; i < 60; i++)
    {
        first_alloc_ptr = ma.alloc(i % 16 + 1);
        ma.free(first_alloc_ptr);
        realloc_ptr = ((uint8_t *)((void *)ma.alloc(2)) + 2 * FSA_SEGMENT_SIZE_16);
        EXPECT_EQ(realloc_ptr, first_alloc_ptr);
    }
}


TEST(MemoryAllocatorTest, TestCoalesceAllocation){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();

    void * p = ma.alloc(4022);
    EXPECT_EQ(((uint16_t *)p)[-1], 4022);
}


TEST(MemoryAllocatorTest, TestCoalesceMerges){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();

    void * first_alloc_ptr = ma.alloc(4022);
    ma.free(first_alloc_ptr);
    void * second_alloc_ptr = ma.alloc(1000);
    ma.free(second_alloc_ptr);
    EXPECT_EQ(second_alloc_ptr, first_alloc_ptr);
}

TEST(MemoryAllocatorTest, TestCoalesceFirstFitSearch){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();

    uint8_t * first_alloc_ptr = (uint8_t *)ma.alloc(2000);
    uint8_t * second_alloc_ptr = (uint8_t *)ma.alloc(6000);
    uint8_t * expected_ptr = first_alloc_ptr + 2000 + 16;
    EXPECT_EQ(second_alloc_ptr, expected_ptr);

    second_alloc_ptr = (uint8_t *)ma.alloc(1000);
    expected_ptr += 6000 + 16;
    EXPECT_EQ(second_alloc_ptr, expected_ptr);
}

TEST(MemoryAllocatorTest, TestCoalesceFirstFitSearchNext){
    MemoryAllocator ma = MemoryAllocator();
    ma.init();
    uint8_t * first_alloc_ptr = (uint8_t *)ma.alloc(2000);
    uint8_t * second_alloc_ptr = (uint8_t *)ma.alloc(9000);
    uint8_t * expected_ptr = second_alloc_ptr + 10240 + 16;
    EXPECT_EQ(first_alloc_ptr, expected_ptr);
}
