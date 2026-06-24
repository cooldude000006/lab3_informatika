#pragma once
#include <cstddef>
#include <cmath>
#include <complex>
#include <string>

#include "ivector.h"
#include "mutable_array_sequence.h"
#include "exceptions.h"
#include "vector_math.h"

namespace lab2
{
    template<class T>
    class ArrayVector : public IVector<T>
    {
    private:
        MutableArraySequence<T>* coordinates_;

        void ValidateSameDimension(
            const IVector<T>& other,
            const std::string& operation
        ) const
        {
            if (GetDimension() != other.GetDimension())
            {
                throw SizeMismatchException(
                    GetDimension(),
                    other.GetDimension(),
                    operation
                );
            }
        }

    public:
        // Пустой математический вектор.
        ArrayVector()
            : coordinates_(new MutableArraySequence<T>())
        {
        }

        // Вектор указанной размерности.
        // Все координаты получают значения по умолчанию.
        explicit ArrayVector(int dimension)
            : coordinates_(
                new MutableArraySequence<T>(dimension)
            )
        {
        }

        // Создание вектора из обычного массива.
        ArrayVector(T* items, int count)
            : coordinates_(
                new MutableArraySequence<T>(items, count)
            )
        {
        }

        // Глубокое копирование.
        ArrayVector(const ArrayVector<T>& other)
            : coordinates_(
                new MutableArraySequence<T>(
                    *other.coordinates_
                )
            )
        {
        }

        ArrayVector<T>& operator=(
            const ArrayVector<T>& other
        )
        {
            if (this != &other)
            {
                MutableArraySequence<T>* newCoordinates =
                    new MutableArraySequence<T>(
                        *other.coordinates_
                    );

                delete coordinates_;
                coordinates_ = newCoordinates;
            }

            return *this;
        }

        ~ArrayVector() override
        {
            delete coordinates_;
            coordinates_ = nullptr;
        }

        std::size_t GetDimension() const override
        {
            return static_cast<std::size_t>(
                coordinates_->GetLength()
            );
        }

        T Get(std::size_t index) const override
        {
            if (index >= GetDimension())
            {
                throw IndexOutOfRangeException(
                    index,
                    GetDimension(),
                    "получение координаты ArrayVector"
                );
            }

            return coordinates_->Get(
                static_cast<int>(index)
            );
        }

        IVector<T>* Add(const IVector<T>& other) const override
        {
            return new ArrayVector<T>(
                (*this) + other
            );
        }

        IVector<T>* MultiplyByScalar(const T& scalar) const override
        {
            return new ArrayVector<T>(
                (*this) * scalar
            );
        }

        T DotProduct(
            const IVector<T>& other
        ) const override
        {
            ValidateSameDimension(
                other,
                "скалярное произведение математических векторов"
            );

            T result{};

            for (
                std::size_t i = 0;
                i < GetDimension();
                ++i
            )
            {
                result += vector_detail::DotProductTerm(
                    Get(i),
                    other.Get(i)
                );
            }

            return result;
        }

        double Norm() const override
        {
            double sum = 0.0;

            for (
                std::size_t i = 0;
                i < GetDimension();
                ++i
            )
            {
                double magnitude =
                    static_cast<double>(
                        std::abs(Get(i))
                    );

                sum += magnitude * magnitude;
            }

            return std::sqrt(sum);
        }

        IEnumerator<T>* GetEnumerator() const override
        {
            return coordinates_->GetEnumerator();
        }

        ArrayVector<T> operator+(const IVector<T>& other) const
        {
            ValidateSameDimension(
                other,
                "сложение математических векторов"
            );

            ArrayVector<T> result(
                static_cast<int>(GetDimension())
            );

            for (std::size_t i = 0; i < GetDimension(); ++i)
            {
                (*result.coordinates_)[static_cast<int>(i)] =
                    Get(i) + other.Get(i);
            }

            return result;
        }

        ArrayVector<T> operator*(
            const T& scalar
        ) const
        {
            ArrayVector<T> result(
                static_cast<int>(GetDimension())
            );

            for (std::size_t i = 0; i < GetDimension(); ++i)
            {
                (*result.coordinates_)[static_cast<int>(i)] =
                    Get(i) * scalar;
            }

            return result;
        }

        T operator[](std::size_t index) const
        {
            return Get(index);
        }

    protected:
        IVector<T>* CreateEmpty() const override
        {
            return new ArrayVector<T>();
        }

        void AppendCoordinate(
            const T& value
        ) override
        {
            coordinates_->Append(value);
        }
    };
}