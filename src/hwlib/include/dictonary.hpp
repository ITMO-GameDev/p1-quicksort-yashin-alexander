#ifndef DICTONARY_HPP
#define DICTONARY_HPP

#include <iostream>


template<typename K, typename V>
class Dictonary final
{
private:
    struct Node
    {
    public:
        Node* left;
        Node* right;
        int _height;
        K key;
        V value;
        Node(K key, V value)
        {
            this->key = key;
            this->value = value;
            this->left = nullptr;
            this->right = nullptr;
            this->_height = 1;
        }

        int height()
        {
            return this->_height;
        }

        int balance_factor()
        {
            if (this->left == nullptr && this->right == nullptr)
                return 0;
            else if (this->left == nullptr)
                return this->right->height();
            else if (this->right == nullptr)
                return -this->left->height();
            else
                return this->right->height() - this->left->height();
        }

        int is_balanced()
        {
            return this->balance_factor <= 1;
        }

        int elements_count()
        {
            int left_cnt = 0;
            int right_cnt = 0;
            if (this->left == nullptr)
                left_cnt = 0;
            else
                left_cnt = this->left->elements_count();
            if (this->right == nullptr)
                right_cnt = 0;
            else
                right_cnt = this->right->elements_count();
            return right_cnt + left_cnt + 1;
        }

        Node* find_minimal(Node* p)
        {
            if (p->left != nullptr)
                return find_minimal(p->left);
            else
                return p;
        }

        Node* remove_minimal(Node* p)
        {
            if (p->left == nullptr)
                return p->right;
            p->left = this->remove_minimal(p->left);
            return balance(p);
        }

        Node* remove(Node* p, int k)
        {
            if (p == nullptr)
                return 0;
            if (k < p->key)
                p->left = this->remove(p->left, k);
            else if (k > p->key)
                p->right = this->remove(p->right, k);
            else
            {
                Node* l = p->left;
                Node* r = p->right;
                delete p;
                if (r == nullptr)
                    return l;
                Node * min = find_minimal(r);
                min->right = remove_minimal(r);
                min->left = l;
                return this->balance(min);
            }
            return this->balance(p);
        }

        Node* get(const K& key)
        {
            if (this->key == key)
                return this;
            else if (this->key > key)
            {
                if (this->left == nullptr)
                    return nullptr;
                return this->left->get(key);
            }
            else
            {
                if (this->right == nullptr)
                    return nullptr;
                return this->right->get(key);
            }
        }

        void recalculate_height()
        {
            if (this->left == nullptr && this->right == nullptr)
                this->_height = 1;
            else if (this->left == nullptr)
                this->_height = this->right->height() + 1;
            else if (this->right == nullptr)
                this->_height = this->left->height() + 1;
            else if (this->left->height() > this->right->height())
                this->_height = this->left->height() + 1;
            else
                this->_height = this->right->height() + 1;
        }

        Node* insert(const K& key, const V& value)
        {
            if (key == this->key)
                this->value = value;
            else if (key < this->key)
                this->insert_left(key, value);
            else
                this->insert_right(key, value);
            return balance(this);
        }

        void insert_left(const K& key, const V& value)
        {
            if (!this->left)
                this->left = new Node(key, value);
            else
                this->left = this->left->insert(key, value);
        }

        void insert_right(const K& key, const V& value)
        {
            if (!this->right)
                this->right = new Node(key, value);
            else
                this->right = this->right->insert(key, value);
        }


        Node* rotate_right(Node* p)
        {
            Node* q = p->left;
            p->left = q->right;
            q->right = p;
            q->recalculate_height();
            p->recalculate_height();
            return q;
        }

        Node* rotate_left(Node* q)
        {
            Node* p = q->right;
            q->right = p->left;
            p->left = q;
            p->recalculate_height();
            q->recalculate_height();
            return p;
        }

        Node* balance(Node* p)
        {
            p->recalculate_height();
            if (p->balance_factor() == 2)
            {
                if (p->right->balance_factor() < 0)
                    p->right = this->rotate_right(p->right);
                return this->rotate_left(p);
            }
            if (p->balance_factor() == -2)
            {
                if (p->left->balance_factor() > 0)
                    p->left = this->rotate_left(p->left);
                return this->rotate_right(p);
            }
            return p;
        }
    };
    Node *_root_node;
public:
    Dictonary():
    {
        this->_root_node = nullptr;
    }

    ~Dictonary()
    {}

    void put(const K& key, const V& value)
    {
        if (!this->_root_node)
            this->_root_node = new Node(key, value);
        else
            this->_root_node = this->_root_node->insert(key, value);
    }

    void remove(const K& key)
    {
        if (!this->_root_node)
            throw std::runtime_error("Key error");
        this->_root_node = this->_root_node->remove(this->_root_node, key);
    }

    bool contains(const K& key)
    {
        if (!this->_root_node)
            throw std::runtime_error("Key error");
        return this->_root_node->get(key) != nullptr;
    }

    V& operator[](const K& key)
    {
        if (!this->_root_node)
            throw std::runtime_error("Key error");
        Node* tmp = this->_root_node->get(key);
        if (tmp == nullptr)
            throw std::runtime_error("Key error");
        return this->_root_node->get(key)->value;
    }

    const V& operator[](const K& key) const
    {
        if (!this->_root_node)
            throw std::runtime_error("Key error");
        Node* tmp = this->_root_node->get(key);
        if (tmp == nullptr)
            throw std::runtime_error("Key error");
        return this->_root_node->get(key)->value;
    }

    int const size()
    {
        if (!this->_root_node)
            return 0;
        return this->_root_node->elements_count();
    }

    std::string pformat()
    {
        std::string dict_pformat;
        this->_pformat(this->_root_node, 0, dict_pformat);
        dict_pformat.append("\n");
        return dict_pformat;
    }

    void _pformat(Node* to_print, int tabulate, std::string& dict_pformat)
    {
        int i;
        if (to_print != nullptr)
        {
            this->_pformat(to_print->right, tabulate+1, dict_pformat);
            for (i = 0; i < tabulate; i++)
                dict_pformat.append("    ");
            dict_pformat.append(std::to_string(to_print->key));
            dict_pformat.append("    \n");
            this->_pformat(to_print->left, tabulate + 1, dict_pformat);
        }
    }
};


#endif // DICTONARY_HPP
