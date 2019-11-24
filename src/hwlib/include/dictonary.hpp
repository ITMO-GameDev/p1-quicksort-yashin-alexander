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
    Dictonary()
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
