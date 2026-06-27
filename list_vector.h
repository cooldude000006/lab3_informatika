#pragma once
#include <cstddef>
#include <cmath>
#include <string>

#include "ivector.h"
#include "mutable_list_sequence.h"
#include "exceptions.h"
#include "vector_math.h"

namespace lab3
{
    template<class T>
    class ListVector : public IVector<T>
    {
    private:
        MutableListSequence<T>* coordinates_;

        static MutableListSequence<T>* CreateCoordinates(
            int dimension
        )
        {
            if (dimension < 0)
            {
                throw InvalidOperationException(
                    "Размерность вектора не может быть отрицательной"
                );
            }

            MutableListSequence<T>* result =
                new MutableListSequence<T>();

            try
            {
                for (int i = 0; i < dimension; ++i)
                {
                    result->Append(T{});
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }

            return result;
        }

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
        ListVector()
            : coordinates_(new MutableListSequence<T>())
        {
        }

        // Вектор указанной размерности.
        // Координаты получают значения по умолчанию.
        explicit ListVector(int dimension)
            : coordinates_(CreateCoordinates(dimension))
        {
        }

        // Создание вектора из обычного массива.
        ListVector(T* items, int count)
            : coordinates_(
                new MutableListSequence<T>(items, count)
            )
        {
        }

        // Глубокое копирование.
        ListVector(const ListVector<T>& other)
            : coordinates_(
                new MutableListSequence<T>(
                    *other.coordinates_
                )
            )
        {
        }

        ListVector<T>& operator=(
            const ListVector<T>& other
        )
        {
            if (this != &other)
            {
                MutableListSequence<T>* newCoordinates =
                    new MutableListSequence<T>(
                        *other.coordinates_
                    );

                delete coordinates_;
                coordinates_ = newCoordinates;
            }

            return *this;
        }

        ~ListVector() override
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
                    "получение координаты ListVector"
                );
            }

            return coordinates_->Get(
                static_cast<int>(index)
            );
        }

        IVector<T>* Add(const IVector<T>& other) const override
        {
            return new ListVector<T>(
                (*this) + other
            );
        }

        IVector<T>* MultiplyByScalar(const T& scalar) const override
        {
            return new ListVector<T>(
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

        ListVector<T> operator+(const IVector<T>& other) const
        {
            ValidateSameDimension(
                other,
                "сложение математических векторов"
            );

            ListVector<T> result;

            for (std::size_t i = 0; i < GetDimension(); ++i)
            {
                result.AppendCoordinate(
                    Get(i) + other.Get(i)
                );
            }

            return result;
        }

        ListVector<T> operator*(
            const T& scalar
        ) const
        {
            ListVector<T> result;

            for (std::size_t i = 0; i < GetDimension(); ++i)
            {
                result.AppendCoordinate(
                    Get(i) * scalar
                );
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
            return new ListVector<T>();
        }

        void AppendCoordinate(
            const T& value
        ) override
        {
            coordinates_->Append(value);
        }
    };
}