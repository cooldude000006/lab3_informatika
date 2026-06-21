#pragma once
#include "mutable_array_sequence.h"
#include "exceptions.h"

namespace lab2
{
    struct Bit
    {
        bool value;
        Bit() : value(false) {}
        explicit Bit(bool val) : value(val) {}
        operator bool() const { return value; }
    };

    class BitSequence : public MutableArraySequence<Bit>
    {
    public:
        explicit BitSequence(int size)
            : MutableArraySequence<Bit>(size)
        {
        }

        BitSequence(Bit* items, int count)
            : MutableArraySequence<Bit>(items, count)
        {
        }

        BitSequence(bool* items, int count)
            : MutableArraySequence<Bit>()
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
            for (int i = 0; i < count; ++i)
            {
                this->Append(Bit(items[i]));
            }
        }

        //побитовые операции
        BitSequence* And(const Sequence<Bit>* other) const
        {
            validate_bitwise_args(other);

            BitSequence* result = new BitSequence(0);
            for (int i = 0; i < this->GetLength(); ++i)
            {
                bool val = this->Get(i).value && other->Get(i).value;
                result->Append(Bit(val));
            }
            return result;
        }

        BitSequence* Or(const Sequence<Bit>* other) const
        {
            validate_bitwise_args(other);

            BitSequence* result = new BitSequence(0);
            for (int i = 0; i < this->GetLength(); ++i)
            {
                bool val = this->Get(i).value || other->Get(i).value;
                result->Append(Bit(val));
            }
            return result;
        }

        BitSequence* Xor(const Sequence<Bit>* other) const
        {
            validate_bitwise_args(other);

            BitSequence* result = new BitSequence(0);
            for (int i = 0; i < this->GetLength(); ++i)
            {
                bool val = this->Get(i).value != other->Get(i).value;
                result->Append(Bit(val));
            }
            return result;
        }

        BitSequence* Not() const
        {
            BitSequence* result = new BitSequence(0);
            for (int i = 0; i < this->GetLength(); ++i)
            {
                bool val = !this->Get(i).value;
                result->Append(Bit(val));
            }
            return result;
        }
    private:
        void validate_bitwise_args(const Sequence<Bit>* other) const
        {
            if (other == nullptr)
            {
                throw InvalidOperationException("Аргумент последовательности не может быть nullptr");
            }
            if (this->GetLength() != other->GetLength())
            {
                throw SizeMismatchException(this->GetLength(), other->GetLength(), "Bitwise operation");
            }
        }
    };
}