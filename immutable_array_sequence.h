#pragma once
#include "sequence.h"
#include "dynamic_array.h"
#include "array_enumerator.h"

namespace lab2
{
    template<class T>
    class ImmutableArraySequence : public Sequence<T>
    {
    private:
        DynamicArray<T>* items_;//инкапсулированный дин. массив
        //конструкор для внутр использования
        explicit ImmutableArraySequence(DynamicArray<T>* arr)
            : items_(arr)
        {
        }
    public:
        //конструкторы
        ImmutableArraySequence(T* items, int count)
            : items_(new DynamicArray<T>(items, count))
        {
        }
        ImmutableArraySequence()
            : items_(new DynamicArray<T>(0))
        {
        }
        ImmutableArraySequence(const ImmutableArraySequence<T>& other)
            : items_(new DynamicArray<T>(*other.items_))
        {
        }
        ImmutableArraySequence<T>& operator=(
            const ImmutableArraySequence<T>& other)
        {
            if (this != &other)
            {
                DynamicArray<T>* newItems =
                    new DynamicArray<T>(*other.items_);

                delete items_;
                items_ = newItems;
            }

            return *this;
        }
        ~ImmutableArraySequence() override
        {
            delete items_;
            items_ = nullptr;
        }
        //декомпозиция
        T GetFirst() const override
        {
            if (items_->GetSize()==0)
            {
                throw InvalidOperationException("Невозможно получить первый элемент: последовательность пуста");
            }
            return items_->Get(0);
        }
        T GetLast() const override
        {
            if (items_->GetSize()==0)
            {
                throw InvalidOperationException("Невозможно получить последний элемент: последовательность пуста");
            }
            return items_->Get(items_->GetSize()-1);
        }
        T Get(int index) const override
        {
            return items_->Get(index);
        }

        Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override
        {
            if (startIndex < 0 || endIndex >= items_->GetSize() || startIndex > endIndex)
            {
                throw IndexOutOfRangeException(startIndex, items_->GetSize(), "ImmutableArraySequence:GetSubsequence");
            }
            DynamicArray<T>* sub = new DynamicArray<T>(endIndex - startIndex + 1);
            for (int i=0; i<= endIndex - startIndex; ++i)
            {
                sub->Set(i, items_->Get(startIndex + i));
            }
            return new ImmutableArraySequence<T>(sub);
        }
        int GetLength() const override
        {
            return items_->GetSize();
        }
        //операции(immutable)
        Sequence<T>* Append(const T& item) override
        {
            DynamicArray<T>* newItems = new DynamicArray<T>(*items_);
            newItems->Resize(newItems->GetSize() + 1);
            newItems->Set(newItems->GetSize() - 1, item);
            return new ImmutableArraySequence<T>(newItems);
        }

        Sequence<T>* Prepend(const T& item) override
        {
            DynamicArray<T>* newItems =
                new DynamicArray<T>(items_->GetSize() + 1);
            newItems->Set(0, item);
            for (int i = 0; i < items_->GetSize(); ++i)
            {
                newItems->Set(i + 1, items_->Get(i));
            }
            return new ImmutableArraySequence<T>(newItems);
        }

        Sequence<T>* InsertAt(const T& item, int index) override
        {
            if (index<0 || index >= items_->GetSize())
            {
                throw IndexOutOfRangeException(index, items_->GetSize(),"ImmutableArraySequence::InsertAt");
            }
            DynamicArray<T>* newItems = new DynamicArray<T>(items_->GetSize()+1);

            for (int i=0; i<index; ++i)
            {
                newItems->Set(i, items_->Get(i));
            }
            newItems->Set(index, item);
            for (int i=index; i<items_->GetSize();++i)
            {
                newItems->Set(i+1, items_->Get(i));
            }
            return new ImmutableArraySequence<T>(newItems);
        }

        Sequence<T>* Concat(Sequence<T>* list) const override
        {
            if (list == nullptr)
            {
                return new ImmutableArraySequence<T>(*this);
            }

            DynamicArray<T>* newItems = new DynamicArray<T>(*items_);
            for (int i = 0; i < list->GetLength(); ++i)
            {
                newItems->Resize(newItems->GetSize() + 1);
                newItems->Set(newItems->GetSize() - 1, list->Get(i));
            }
            return new ImmutableArraySequence<T>(newItems);
        }

        //
        IEnumerator<T>* GetEnumerator() const override
        {
            return new ArrayEnumerator<T>(this);
        }
        //

    //map-reduce
    protected:
        Sequence<T>* CreateEmpty() const override
        {
            return new ImmutableArraySequence<T>();
        }
    };
}





















