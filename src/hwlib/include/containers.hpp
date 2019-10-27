#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <iostream>
#include <iterator>


const int MIN_CAPACITY = 0;
const int INCREMENTION_MULTIPLIE = 2;


template<typename T>
class Array
{
private:
    T *_storage_ptr;
    int _capacity;
    int _size;

public:
    class Iterator: public std::iterator<std::forward_iterator_tag, T>
    {
    private:
        Array<T>& _array;
        int _it_pos;
    public:
        Iterator(Array<T> &array): _array(array), _it_pos(0) {}

        const T& get() const
        {
            return this->_array[this->_it_pos];
        }

        void set(const T& value)
        {
            this->_array[this->_it_pos] = value;
        }

        void insert(const T &value)
        {
            this->_array.insert(this->_it_pos, value);
        }

        void remove()
        {
            this->_array.remove(this->_it_pos);
        }

        void next()
        {
            if (this->_it_pos >= this->_array.size() - 1 )
                throw std::runtime_error("Unable to set iterator index to that position");
            this->_it_pos++;
        }

        void prev()
        {
            if (this->_it_pos <= 0)
                throw std::runtime_error("Unable to set iterator index to that position");
            this->_it_pos--;
        }

        void to_index(int index)
        {
            if ((index > this->_array._size) || (index < -1))
                throw std::runtime_error("Unable to set iterator index to that position");
            this->_it_pos = index;
        }

        bool hasNext() const
        {
            return (this->_it_pos < this->_array._size - 1);
        }

        bool hasPrev() const
        {
            return (this->_it_pos > 0);
        }
    };

    Array(int capacity):
    _capacity(capacity),
    _storage_ptr(nullptr),
    _size(0)
    {
        if (capacity < MIN_CAPACITY)
            throw std::runtime_error("Invalid array capacity");
        if (this->_capacity != 0)
            this->_storage_ptr = new T [capacity];
        else
            this->_storage_ptr = nullptr;
    }

    ~Array(){
        delete[] this->_storage_ptr;
    }

    // Copy constructor
    Array(const Array& other)
    {
        this->_storage_ptr = new T[other._capacity];
        std::copy(other._storage_ptr,
                  other._storage_ptr + other._capacity,
                  this->_storage_ptr);

        _capacity = other._capacity;
        _size = other._size;
    }

    // Copy assignment operator
    Array& operator =(const Array& other)
    {
        if (&other == this)
            return *this;

        delete[] this->_storage_ptr;
        this->_storage_ptr = new T[other._capacity];

        std::copy(other._storage_ptr,
                  other._storage_ptr + other._capacity,
                  this->_storage_ptr);

        _capacity = other._capacity;
        _size = other._size;

        return *this;
    }

    // Move constructor
    Array(Array&& other)
    {
        this->_storage_ptr = new T [other._capacity];
        std::move(other._storage_ptr,
                  other._storage_ptr + other._capacity,
                  this->_storage_ptr);

        _capacity = other._capacity;
        _size = other._size;

        other._storage_ptr = nullptr;
        other._size = 0;
    }

    // Move assignment operator
    Array& operator=(Array&& other)
    {
        if (this != &other)
        {
            delete[] this->_storage_ptr;
            this->_storage_ptr = new T [other._capacity];

            std::move(other._storage_ptr,
                      other._storage_ptr + other._capacity,
                      this->_storage_ptr);

            _capacity = other._capacity;
            _size = other._size;

            other._storage_ptr = nullptr;
            other._size = 0;
        }
        return *this;
    }

    void _expand_storage()
    {
        T* new_chunk_ptr;
        if (this->capacity() == 0)
            this->_capacity++;
        this->_capacity = this->_capacity * INCREMENTION_MULTIPLIE;

        new_chunk_ptr = new T [this->_capacity];

        for (int i = 0; i != this->_size; i++)
            new_chunk_ptr[i] = std::move(this->_storage_ptr[i]);

        this->_storage_ptr = new_chunk_ptr;
    }

    int const capacity()
    {
        return _capacity;
    }

    T& operator[](int index)
    {
        if ((index > this->size()) || (index < -1))
            throw std::runtime_error("Reference to invalid item, index is out of bounds");
        return this->_storage_ptr[index];
    }

    const T& operator[](int index) const
    {
        if ((index > this->size()) || (index < -1))
            throw std::runtime_error("Reference to invalid item, index is out of bounds");
        return std::move(this->_storage_ptr[index]);
    }

    int const size()
    {
        return this->_size;
    }

    void insert(const T& value)
    {
        this->insert(this->size(), value);
    }

    void insert(int index, const T& value)
    {
        if ((index > this->size()) || (index < 0))
            throw std::runtime_error("Unable to insert, index is out of bounds");

        if (this->_capacity == this->size())
            this->_expand_storage();

        for (int i = this->size() - 1; i >= index; i--)
            this->_storage_ptr[i + 1] = std::move(this->_storage_ptr[i]);

        this->_storage_ptr[index] = std::move(value);

        this->_size++;
    }

    void remove(int index)
    {
        if ((index >= this->size()) || (index < -1))
            throw std::runtime_error("Unable to remove, index is out of bounds");

        for (int i = index; i < this->size(); i++)
            this->_storage_ptr[i] = std::move(this->_storage_ptr[i + 1]);

        this->_size--;
    }

    Iterator iterator()
    {
         return Iterator(*this);
    }

    Iterator begin()
    {
        Iterator *it = new Iterator(*this);
        it->to_index(-1);
        return *it;
    }

    Iterator end()
    {
        Iterator *it = new Iterator(*this);
        it->to_index(this->size());
        return *it;
    }
};


#endif // CONTAINERS_HPP
