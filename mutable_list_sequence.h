#pragma once
#include "sequence.h"
#include "linked_list.h"
#include "list_enumerator.h"

namespace lab2
{
    template<class T>
    class MutableListSequence : public Sequence<T>
    {
    private:
        LinkedList<T>* list_;
    public:
        //конструкторы
        MutableListSequence(T* items, int count)
            : list_(new LinkedList<T>(items, count))
        {
        }

        MutableListSequence()
            :list_(new LinkedList<T>())
        {
        }

        MutableListSequence(const MutableListSequence<T>& other)
            : list_(new LinkedList<T>(*other.list_))
        {
        }

        MutableListSequence<T>& operator=(
            const MutableListSequence<T>& other)
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

        ~MutableListSequence() override
        {
            delete list_;
            list_=nullptr;
        }
        //декомпозиция
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
                throw IndexOutOfRangeException(startIndex, list_->GetLength(), "MutableListSequence::GetSubsequence");
            }

            MutableListSequence<T>* result = new MutableListSequence<T>();
            for (int i = startIndex; i <= endIndex; ++i)
            {
                result->Append(list_->Get(i));
            }
            return result;
        }

        int GetLength() const override
        {
            return list_->GetLength();
        }
        //операции
        Sequence<T>* Append(const T& item) override
        {
            list_->Append(item);
            return this;
        }

        Sequence<T>* Prepend(const T& item) override
        {
            list_->Prepend(item);
            return this;
        }

        Sequence<T>* InsertAt(const T& item, int index) override
        {
            list_->InsertAt(item, index);
            return this;
        }

        Sequence<T>* Concat(Sequence<T>* list) const override
        {
            if (list == nullptr)
            {
                return new MutableListSequence<T>(*this);
            }
            MutableListSequence<T>* result = new MutableListSequence<T>(*this);
            for (int i=0; i<list->GetLength(); ++i)
            {
                result->Append(list->Get(i));
            }
            return result;
        }

        //
        IEnumerator<T>* GetEnumerator() const override
        {
            return new ListEnumerator<T>(this);
        }
        //

    protected:
        Sequence<T>* CreateEmpty() const override
        {
            return new MutableListSequence<T>();
        }
    };
}