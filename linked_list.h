#pragma once
#include "exceptions.h"
#include <algorithm>

namespace lab2
{
    template<class T>
    class LinkedList
    {
    private:
        struct Node
        {
            T data;
            Node* next;
            Node(const T& value) : data(value), next(nullptr) {}
        };
        Node* head_;
        int count_;

         Node* get_node_at(int index) const
        {
             if (index < 0 || index >= count_)
             {
                throw IndexOutOfRangeException(index, count_, "LinkedList::get_node_at");
             }
             Node* current=head_;
             for (int i=0;i<index;++i)
             {
                current=current->next;
             }
             return current;
        }

    public: // конструкторы
        LinkedList() : head_(nullptr), count_(0) {}

        LinkedList(T* items, int count) : head_(nullptr), count_(0)
        {
            if (count < 0)
            {
                throw InvalidOperationException("Размер не может быть отрицательным");
            }
            if (items == nullptr && count > 0)
            {
                throw InvalidOperationException(
                    "Указатель items равен nullptr при положительном количестве элементов");
            }
            for (int i=0;i<count;++i)
            {
                Append(items[i]);
            }
        }
        LinkedList(const LinkedList<T>& other) : head_(nullptr), count_(0)
        {
            Node* current = other.head_;
            while (current != nullptr)
            {
                Append(current->data);
                current = current->next;
            }
        }
        ~LinkedList()
        {
            Clear();
        }

        //операторы
        LinkedList<T>& operator=(const LinkedList<T>& other)
        {
            if (this == &other) return *this;

            Clear();
            Node* current = other.head_;
            while (current != nullptr)
            {
                Append(current->data);
                current = current->next;
            }
            return *this;
        }

        //декомпозиция
        T GetFirst() const
        {
            if (head_ == nullptr)
            {
                throw InvalidOperationException("Невозможно получить первый элемент, список пуст");
            }
            return head_->data;
        }

        T GetLast() const
        {
            if (head_ == nullptr)
            {
                throw InvalidOperationException("Невозможно получить последний элемент: список пуст");
            }
            Node* current = head_;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            return current->data;
        }
        T Get(int index) const
        {
            return get_node_at(index)->data;
        }

        LinkedList<T>* GetSublist(int startIndex, int endIndex) const
        {
            if (startIndex < 0 || endIndex >= count_ || startIndex > endIndex)
            {
                throw IndexOutOfRangeException(startIndex, count_, "LinkedList::GetSubList (invalid range)");
            }
            LinkedList<T>* result = new LinkedList<T>();
            Node* current = get_node_at(startIndex);

            for (int i = startIndex; i <= endIndex; ++i)
            {
                result->Append(current->data);
                current = current->next;
            }
            return result;
        }

        int GetLength() const
        {
            return count_;
        }

        //операции
        void Append(const T& item)
        {
            Node* newNode = new Node(item);
            if (head_ == nullptr)
            {
                head_ = newNode;
            }
            else
            {
                Node* tail = head_;
                while (tail->next != nullptr)
                {
                    tail = tail->next;
                }
                tail->next = newNode;
            }
            count_++;
        }
        void Prepend(const T& item)
        {
            Node* newNode = new Node(item);
            newNode->next = head_;
            head_ = newNode;
            count_++;
        }

        void InsertAt(const T& item, int index)
        {
            if (index < 0 || index >= count_)
            {
                throw IndexOutOfRangeException(index, count_, "LinkedList::InsertAt");
            }
            if (index == 0)
            {
                Prepend(item);
                return;
            }

            Node* prev = get_node_at(index - 1);
            Node* newNode = new Node(item);
            newNode->next = prev->next;
            prev->next = newNode;
            count_++;
        }

        LinkedList<T>* Concat(LinkedList<T>* list) const
        {
            if (list == nullptr) return new LinkedList<T>(*this);

            LinkedList<T>* result = new LinkedList<T>(*this);
            Node* current = list->head_;
            while (current != nullptr)
            {
                result->Append(current->data);
                current = current->next;
            }
            return result;
         }

        void Clear()
        {
            while (head_ != nullptr)
            {
                Node* temp = head_;
                head_ = head_->next;
                delete temp;
            }
            count_=0;
        }
    };
}









