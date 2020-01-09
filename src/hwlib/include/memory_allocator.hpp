#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include <vector>
#include <cstddef>
#include <unistd.h>
#include <sys/mman.h>
#include <iostream>

#include "containers.hpp"
#include "dictonary.hpp"


const int FSA_MAX_BYTES_THRESHHOLD = 512;
const int CA_MAX_BYTES_THRESHHOLD = 10 * 1024 * 1024;

const int CA_SEGMENTS_NUMBER = 10;
const uint16_t CA_FAKE_MIN_ALLOC_SIZE = 2^16 - 1;
const int CA_FRAGMENT_IDENTIFER_SIZE = 16;
const int CA_MAX_FRAGMENT_SIZE = 10240;

const int FSA_FREE_LIST_IDENTIFER_SIZE = 16;

const int FSA_SEGMENTS_NUMBER = 6;
const int FSA_SEGMENT_SIZE_16 = 16;
const int FSA_SEGMENT_SIZE_32 = 32;
const int FSA_SEGMENT_SIZE_64 = 64;
const int FSA_SEGMENT_SIZE_128 = 128;
const int FSA_SEGMENT_SIZE_256 = 256;
const int FSA_SEGMENT_SIZE_512 = 512;

const int DEFAULT_MMAPPED_FRAGMENTS_NUM = 10;


class FixedSizeAllocator
{
private:
    Dictonary<uint16_t, LinkedList<uint8_t *>> free_lists_dict;
    size_t page_size;
    bool initialized = false;
    void * memory_ptr;
    size_t memory_amount;

    uint8_t * _build_free_list(void * start_mem_ptr, size_t fragment_size, uint8_t * page_end_ptr)
    {
        uint32_t raw_mem_amount = page_end_ptr - static_cast<uint8_t *>(start_mem_ptr);
        size_t fragment_real_size = fragment_size + FSA_FREE_LIST_IDENTIFER_SIZE;
        uint32_t fragments_in_page = raw_mem_amount / static_cast<uint32_t>(fragment_real_size);
        uint8_t * mem_ptr = static_cast<uint8_t *>(start_mem_ptr);
        uint16_t * meta_info_block = static_cast<uint16_t *>(start_mem_ptr);
        for(int i = 0; i < fragments_in_page; i++)
        {
            meta_info_block[0] = fragment_size;
            free_lists_dict[fragment_size].insert_head(mem_ptr);
            mem_ptr += fragment_real_size;
            meta_info_block = (uint16_t *)(static_cast<void *>(mem_ptr));
        }
        return mem_ptr;
    }

    void * get_fragment_with_size(LinkedList<uint8_t *> &ll, size_t requested_size)
    {
        void * fragment_ptr;
        if (ll.size() > 0)
        {
            fragment_ptr = ll.tail();

            ll.remove_tail();
            // Hide metadata
            return (void *)( static_cast<uint16_t *>(fragment_ptr) + 1);
        }
        return (void *)NULL;
    }

public:
    FixedSizeAllocator()
    {}

    ~FixedSizeAllocator()
    {
        destroy();
    }

    void destroy(){
        munmap(memory_ptr, memory_amount);
    }

    void init()
    {
        if (!initialized)
        {
            uint8_t * raw_memory_ptr;
            uint8_t * memory_start_ptr;
            this->page_size = static_cast<size_t>(sysconf(_SC_PAGE_SIZE));

            free_lists_dict.put(16, LinkedList<uint8_t *>());
            free_lists_dict.put(32, LinkedList<uint8_t *>());
            free_lists_dict.put(64, LinkedList<uint8_t *>());
            free_lists_dict.put(128, LinkedList<uint8_t *>());
            free_lists_dict.put(256, LinkedList<uint8_t *>());
            free_lists_dict.put(512, LinkedList<uint8_t *>());

            memory_amount =  static_cast<size_t>(page_size) * FSA_SEGMENTS_NUMBER;
            raw_memory_ptr = static_cast<uint8_t *>(mmap(nullptr, memory_amount, PROT_READ|PROT_WRITE,
                                                         MAP_PRIVATE|MAP_ANONYMOUS, -1, 0));
            memory_ptr = raw_memory_ptr;
            memory_start_ptr = raw_memory_ptr;

            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_512, memory_start_ptr + page_size);
            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_256, memory_start_ptr + 2 * page_size);
            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_128, memory_start_ptr + 3 * page_size);
            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_64, memory_start_ptr + 4 * page_size);
            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_32, memory_start_ptr + 5 * page_size);
            raw_memory_ptr = _build_free_list(raw_memory_ptr, FSA_SEGMENT_SIZE_16, memory_start_ptr + 6 * page_size);
            initialized = true;
        }
    }

    void * alloc(size_t size)
    {
        if (size <= FSA_SEGMENT_SIZE_16)
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_16], size);
        else if (size <= FSA_SEGMENT_SIZE_32)
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_32], size);
        else if (size <= FSA_SEGMENT_SIZE_64)
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_64], size);
        else if (size <= FSA_SEGMENT_SIZE_128)
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_128], size);
        else if (size <= FSA_SEGMENT_SIZE_256)
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_256], size);
        else
            return get_fragment_with_size(free_lists_dict[FSA_SEGMENT_SIZE_512], size);
    }

    void dump_blocks () const
    {
        std::cout << FSA_SEGMENT_SIZE_16 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_16].size() << '\n';
        std::cout << FSA_SEGMENT_SIZE_32 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_32].size() << '\n';
        std::cout << FSA_SEGMENT_SIZE_64 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_64].size() << '\n';
        std::cout << FSA_SEGMENT_SIZE_128 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_128].size() << '\n';
        std::cout << FSA_SEGMENT_SIZE_256 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_256].size() << '\n';
        std::cout << FSA_SEGMENT_SIZE_512 << "-byte fragments available: " << free_lists_dict[FSA_SEGMENT_SIZE_512].size() << '\n';
    }

    void free(void * p)
    {
        uint16_t fsa_segment;
        fsa_segment = *((uint16_t *)p - 1);
        free_lists_dict[fsa_segment].insert_head((uint8_t *)p - 1);
    }
};


class CoalesceAllocator
{
private:
    size_t page_size;
    bool initialized = false;
    LinkedList<uint8_t *> free_list;
    void * memory_ptr;
    size_t memory_amount;

public:
    CoalesceAllocator()
    {}

    ~CoalesceAllocator()
    {
        destroy();
    }

    void destroy(){
        munmap(memory_ptr, memory_amount);
    }

    void build_free_list(void * start_mem_ptr, uint8_t number_of_segments)
    {
        uint8_t * mem_ptr = (uint8_t *)start_mem_ptr;
        uint16_t * meta_info_block = static_cast<uint16_t *>(start_mem_ptr);
        for(int i = 0; i < number_of_segments; i++)
        {
            meta_info_block = static_cast<uint16_t *>((void *)mem_ptr);
            meta_info_block[0] = page_size - CA_FRAGMENT_IDENTIFER_SIZE;
            free_list.insert_head((uint8_t *)meta_info_block);
            mem_ptr += page_size;
        }
    }

    void init()
    {
        if (!initialized){
            uint8_t * raw_memory_ptr;
            uint8_t * memory_start_ptr;
            this->page_size = CA_MAX_FRAGMENT_SIZE + CA_FRAGMENT_IDENTIFER_SIZE;

            memory_amount = static_cast<size_t>(page_size) * CA_SEGMENTS_NUMBER;
            raw_memory_ptr = static_cast<uint8_t *>(mmap(nullptr, memory_amount, PROT_READ|PROT_WRITE,
                                                         MAP_PRIVATE|MAP_ANONYMOUS, -1, 0));
            memory_ptr = raw_memory_ptr;
            build_free_list(raw_memory_ptr, CA_SEGMENTS_NUMBER);

            initialized = true;
        }
    }

    void * split_region(LinkedList<uint8_t *>::Iterator iter , size_t required_size, size_t min_size_delta)
    {
        uint16_t * region_to_split = (uint16_t *)iter.get();
        uint16_t actual_region_size = *(((uint16_t *)region_to_split));

        if (actual_region_size < required_size + min_size_delta + 2 * CA_FRAGMENT_IDENTIFER_SIZE){
            return region_to_split;
        } else {
            uint8_t * second_region_part;
            uint16_t * meta_info_block;
            region_to_split[0] = required_size;
            second_region_part = (uint8_t *)iter.get() + required_size + CA_FRAGMENT_IDENTIFER_SIZE;
            meta_info_block = static_cast<uint16_t *>((void *)second_region_part);
            meta_info_block[0] = actual_region_size - (CA_FRAGMENT_IDENTIFER_SIZE + required_size);

            iter.set((uint8_t *)second_region_part);
            return region_to_split;
        }
    }

    void * pop_any_region()
    {
        void * allocated_region;
        auto ite = free_list.iter();
        allocated_region = ite.pop();
        allocated_region = ((uint16_t *)allocated_region + 1);
        return allocated_region;
    }

    void * alloc(size_t required_size)
    {
        void * allocated_region;
        auto it = free_list.iter();
        for (; !it.iteration_stopped(); it.next())
        {
            uint16_t current_alloc_size = *(((uint16_t *)it.get()));
            if (current_alloc_size >= required_size)
            {
                allocated_region = split_region(it, required_size, FSA_SEGMENT_SIZE_512 + 1);
                allocated_region = ((uint16_t *)allocated_region + 1);
                return allocated_region;

            }
        }

//        if (current_min_alloc_size == CA_FAKE_MIN_ALLOC_SIZE)
//        {
            // mmap new regions;
//        }

        return pop_any_region();
    }

    void coalesce_blocks(uint16_t * existent_region, uint8_t * region_to_merge)
    {
        uint16_t new_region_identifer;
        new_region_identifer = (existent_region)[0] + ((uint16_t *)region_to_merge)[0] + CA_FRAGMENT_IDENTIFER_SIZE;
        existent_region[0] = new_region_identifer;
    }

    void free(void * p)
    {
        void * coalesce_candidate;
        uint16_t candidate_size;

        uint16_t * region_start;
        uint16_t region_size;
        uint8_t * region_end;

        region_start = (uint16_t *)p - 1;
        region_size = ((uint16_t *)p)[-1];
        region_end = (uint8_t*)region_start + region_size + CA_FRAGMENT_IDENTIFER_SIZE;

        auto it = free_list.iter();
        for (; !it.iteration_stopped(); it.next())
        {
            coalesce_candidate = it.get();
            candidate_size = ((uint16_t*)coalesce_candidate)[0];
            if (coalesce_candidate == region_end)
                coalesce_blocks((uint16_t *)coalesce_candidate, (uint8_t *)region_start);
        }
    }

    void dump_blocks() const
    {
        auto free_list_copy = free_list;
        std::cout << "Coalesce allocator stats:\n" << "Free list size = " << free_list.size() << '\n' << "Blocks:\n";
        for (auto it = free_list_copy.iter(); !it.iteration_stopped(); it.next())
        {
            std::cout << "Mem available " << ((uint16_t *)it.get())[0] << '\n';
        }
    }
};


class MemoryAllocator
{
private:
    struct MmappedMemInfo{
        void * ptr;
        size_t size;
    };

    FixedSizeAllocator _fsa;
    CoalesceAllocator _ca;
    Array<MmappedMemInfo> mmapped_memory;

    void * _alloc_via_mmap(size_t size)
    {
        MmappedMemInfo mmapped_mem_info;
        mmapped_mem_info.ptr = static_cast<uint8_t *>(mmap(nullptr, size, PROT_READ|PROT_WRITE,
                                                           MAP_PRIVATE|MAP_ANONYMOUS, -1, 0));
        mmapped_mem_info.size = size;
        mmapped_memory.insert(mmapped_mem_info);
        return mmapped_mem_info.ptr;
    }

    void _destroy_mmapped(){
        for (int i = 0; i < mmapped_memory.size(); i++)
            munmap(mmapped_memory[i].ptr, mmapped_memory[i].size);
    }

    void _dump_mmapped(){
        std::cout << "Memory mmapped:\n";
        for (int i = 0; i < mmapped_memory.size(); i++)
            std::cout << "Fragment " << i << " size: " << mmapped_memory[i].size << " ptr: " << mmapped_memory[i].ptr << '\n';
     }

public:
    MemoryAllocator():
    mmapped_memory(Array<MmappedMemInfo>(DEFAULT_MMAPPED_FRAGMENTS_NUM))
    {}

    virtual ~MemoryAllocator()
    {}

    virtual void init()
    {
        _fsa.init();
        _ca.init();
    }

    virtual void destroy()
    {
        _fsa.destroy();
        _ca.destroy();
        _destroy_mmapped();
    }

    virtual void *alloc(std::size_t size)
    {
        if (size <= FSA_MAX_BYTES_THRESHHOLD)
        {
            return _fsa.alloc(size);
        } else if (size <= CA_MAX_FRAGMENT_SIZE)
        {
            return _ca.alloc(size);
        } else
        {
            return _alloc_via_mmap(size);
        }
    }

    virtual void free(void *p)
    {
        if (((uint16_t*)p)[-1] <= FSA_MAX_BYTES_THRESHHOLD)
        {
            _fsa.free(p);
        } else if (((uint16_t*)p)[-1] <= CA_MAX_BYTES_THRESHHOLD)
        {
            _ca.free(p);
        } else {
            _destroy_mmapped();
        }
    }

    virtual void dumpBlocks()
    {
        std::cout << "======================\n";
        _fsa.dump_blocks();
        std::cout << "--------------------\n";
        _ca.dump_blocks();
        std::cout << "--------------------\n";
        _dump_mmapped();
        std::cout << "-------------\n";
    }
};


#endif // MEMORY_ALLOCATOR_HPP
