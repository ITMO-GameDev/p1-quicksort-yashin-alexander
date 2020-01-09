#ifndef CONTAINERS_HPP
#define CONTAINERS_HPP

#include <iostream>
#include <iterator>


const int MIN_CAPACITY = 0;
const int INCREMENTION_MULTIPLIE = 2;
const int LIST_CHUNK_SIZE = 8;


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



template<typename T>
class LinkedList
{
private:
    struct Chunk
    {
        Array<T> *_container;
        int _size;
        int _capacity;
        Chunk * next;
        Chunk * prev;

        Chunk(const int capacity, Chunk *prev_chunk, Chunk *next_chunk):
        _size(0),
        _capacity(capacity),
        next(next_chunk),
        prev(prev_chunk)
        {
            _container = new Array<T>(capacity);
        }

        ~Chunk()
        {}

        void insert_to_start(const T& value)
        {
            this->_container->insert(0, value);
            this->_size++;
        }

        void insert_to_end(const T& value)
        {
            this->_container->insert(value);
            this->_size++;
        }

        int const size()
        {
            return this->_size;
        }

        bool const filled()
        {
            return this->_capacity == this->_size;

        }

        void remove_first()
        {
            this->_container->remove(0);
            this->_size--;
        }

        void remove_last()
        {
            int cnt_size;
            cnt_size = this->_container->size() - 1;
            this->_container->remove(cnt_size);
            this->_size--;
        }

        const T& first()
        {
            auto it = this->_container->begin();
            it.next();
            return it.get();
        }

        const T& last()
        {
            auto it = this->_container->end();
            it.prev();
            return it.get();
        }
    };

    Chunk *_storage_ptr;
    Chunk * _head_chunk_ptr;
    Chunk * _tail_chunk_ptr;
    int _size;
public:
    LinkedList():
    _storage_ptr(nullptr),
    _size(0)
    {
        this->_tail_chunk_ptr = new Chunk(LIST_CHUNK_SIZE, nullptr, nullptr);
        this->_head_chunk_ptr = this->_tail_chunk_ptr;
    }

    class Iterator: public std::iterator<std::forward_iterator_tag, T>
    {
    private:
        LinkedList<T>& _linked_list;
        Chunk * _current_chunk_ptr;
        int16_t _in_chunk_position;
        int16_t _in_list_position;
        bool _stop_iteration = false;
    public:
        Iterator(LinkedList<T> &linked_list):
            _linked_list(linked_list),
            _current_chunk_ptr(linked_list._tail_chunk_ptr),
            _in_chunk_position(0),
            _in_list_position(0)
        {}

        const T& get()
        {
            auto it = _current_chunk_ptr->_container->begin();
            it.to_index(_in_chunk_position);
            return it.get();
        }

        bool iteration_stopped()
        {
            return this->_stop_iteration;
        }

        void next()
        {
            if (this->_in_list_position == this->_linked_list.size() - 1)
            {
                _stop_iteration = true;
                return;
            }
            else if (this->_in_chunk_position == this->_current_chunk_ptr->size() - 1)
            {
                //jump to next chunk
                this->_current_chunk_ptr = this->_current_chunk_ptr->prev;
                _in_chunk_position = 0;
                _in_list_position++;
            } else {
                _in_chunk_position++;
                _in_list_position++;
            }
        }

        void set(const T& value)
        {
            auto it = _current_chunk_ptr->_container->begin();
            it.to_index(_in_chunk_position);
            it.set(value);
        }

        T pop()
        {
            T element;
            auto it = _current_chunk_ptr->_container->begin();
            it.to_index(_in_chunk_position);
            element = it.get();
            it.remove();
            _linked_list._size--;
            return element;
        }
    };


    void insert_head(const T& value)
    {
        if (this->_head_chunk_ptr->filled())
        {
            this->_head_chunk_ptr->prev = new Chunk(LIST_CHUNK_SIZE, nullptr, this->_storage_ptr);
            this->_head_chunk_ptr = this->_head_chunk_ptr->prev;
        }
        this->_head_chunk_ptr->insert_to_start(value);
        this->_size++;
    }

    void insert_tail(const T& value)
    {
        if (this->_tail_chunk_ptr->filled())
        {
            this->_tail_chunk_ptr->next = new Chunk(LIST_CHUNK_SIZE, this->_tail_chunk_ptr, nullptr);
            this->_tail_ptr = this->_tail_chunk_ptr->next;
        }
        this->_tail_chunk_ptr->insert_to_end(value);
        this->_size++;
    }

    void remove_head()
    {
        if (this->size() == 0)
            return;

        this->_head_chunk_ptr->remove_first();
        this->_size--;

        // get rid of empty chunks
        if (this->_head_chunk_ptr->next == nullptr)  // keep chunk if it's last
            return;

        if (this->_head_chunk_ptr->size() == 0)  // and remove if it's not
        {
            this->_head_chunk_ptr = this->_head_chunk_ptr->next;
            this->_head_chunk_ptr->prev->~Chunk();
            this->_head_chunk_ptr->prev = nullptr;
        }
    }

    void remove_tail()
    {
        if (this->size() == 0)
            return;

        this->_tail_chunk_ptr->remove_last();
        this->_size--;

        // get rid of empty chunks
        if (this->_tail_chunk_ptr->prev == nullptr)  // keep chunk if it's last
            return;

        if (this->_tail_chunk_ptr->size() == 0)  // and remove if it's not
        {
            this->_tail_chunk_ptr = this->_tail_chunk_ptr->prev;
            this->_tail_chunk_ptr->next->~Chunk();
            this->_tail_chunk_ptr->next = nullptr;
        }
    }

    const T& head()
    {
        if (this->size() == 0)
            throw std::runtime_error("Unable to read head, List is empty");
        return this->_head_chunk_ptr->first();
    }

    const T& tail()
    {
        if (this->size() == 0)
            throw std::runtime_error("Unable to read head, List is empty");
        return this->_tail_chunk_ptr->last();
    }

    int size() const
    {
        return this->_size;
    }

    Iterator iter()
    {
        Iterator *it = new Iterator(*this);
        return *it;
    }
};

#endif // CONTAINERS_HPP
