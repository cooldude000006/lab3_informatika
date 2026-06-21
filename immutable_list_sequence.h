#pragma once
#include "sequence.h"
#include "linked_list.h"
#include "list_enumerator.h"

namespace lab2
{
    template <class T>
    class ImmutableListSequence : public Sequence<T>
    {
    private:
        LinkedList<T>* list_;
        explicit ImmutableListSequence(LinkedList<T>* lst)
            : list_(lst)
        {
        }
    //конструкторы
    public:
        ImmutableListSequence(T* items, int count)
            : list_(new LinkedList<T>(items, count))
        {
        }

        ImmutableListSequence()
            : list_(new LinkedList<T>())
        {
        }

        ImmutableListSequence(const ImmutableListSequence<T>& other)
            : list_(new LinkedList<T>(*other.list_))
        {
        }

        ImmutableListSequence<T>& operator=(
            const ImmutableListSequence<T>& other)
        {
            if (this != &other)
            {
                LinkedList<T>* newList =
                    new LinkedList<T>(*other.list_);
                delete list_;
                list_ = newList;
            }
            return *this;
        }

        ~ImmutableListSequence() override
        {
            delete list_;
            list_ = nullptr;
        }

        // декомпозиция
        T GetFirst() const override
        {
            if (list_->GetLength() == 0)
            {
                throw InvalidOperationException("Невозможно получить первый элемент: список пуст");
            }
            return list_->GetFirst();
        }

        T GetLast() const override
        {
            if (list_->GetLength() == 0)
            {
                throw InvalidOperationException("Невозможно получить последний элемент: список пуст");
            }
            return list_->GetLast();
        }

        T Get(int index) const override
        {
            return list_->Get(index);
        }

        Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override
        {
            if (startIndex < 0 || endIndex >= list_->GetLength() || startIndex > endIndex)
            {
                throw IndexOutOfRangeException(startIndex, list_->GetLength(), "ImmutableListSequence::GetSubsequence");
            }

            LinkedList<T>* subList = new LinkedList<T>();
            for (int i = startIndex; i <= endIndex; ++i)
            {
                subList->Append(list_->Get(i));
            }
            return new ImmutableListSequence<T>(subList);
        }

        int GetLength() const override
        {
            return list_->GetLength();
        }

        //операции
        Sequence<T>* Append(const T& item) override
        {
            LinkedList<T>* newList = new LinkedList<T>(*list_);
            newList->Append(item);
            return new ImmutableListSequence<T>(newList);
        }

        Sequence<T>* Prepend(const T& item) override
        {
            LinkedList<T>* newList = new LinkedList<T>(*list_);
            newList->Prepend(item);
            return new ImmutableListSequence<T>(newList);
        }

        Sequence<T>* InsertAt(const T& item, int index) override
        {
            if (index < 0 || index >= list_->GetLength())
            {
                throw IndexOutOfRangeException(
                    index,
                    list_->GetLength(),
                    "вставка в неизменяемую последовательность на основе списка"
                );
            }
            LinkedList<T>* newList = new LinkedList<T>(*list_);
            newList->InsertAt(item, index);
            return new ImmutableListSequence<T>(newList);
        }

        Sequence<T>* Concat(Sequence<T>* list) const override
        {
            if (list == nullptr)
            {
                return new ImmutableListSequence<T>(*this);
            }

            LinkedList<T>* newList = new LinkedList<T>(*list_);
            for (int i = 0; i < list->GetLength(); ++i)
            {
                newList->Append(list->Get(i));
            }
            return new ImmutableListSequence<T>(newList);
        }

        //
        IEnumerator<T>* GetEnumerator() const override
        {
            return new ListEnumerator<T>(this);
        }
        //

    //map-reduce
    protected:
        Sequence<T>* CreateEmpty() const override
        {
            return new ImmutableListSequence<T>();
        }
    };
}




















