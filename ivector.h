#pragma once
#include <cstddef>
#include "icollection.h"
#include "ienumerable.h"
namespace lab2
{
    template<class T>
    class IVector :
        public ICollection<T>,
        public IEnumerable<T>
    {
    public:
        virtual ~IVector() = default;

        // Количество координат математического вектора.
        virtual std::size_t GetDimension() const = 0;

        // Реализация метода интерфейса ICollection<T>.
        std::size_t GetCount() const override
        {
            return GetDimension();
        }
        // Получение координаты по индексу.
        virtual T Get(std::size_t index) const override = 0;

        // Сложение двух векторов.
        virtual IVector<T>* Add(
            const IVector<T>& other
        ) const = 0;

        // Умножение вектора на скаляр.
        virtual IVector<T>* MultiplyByScalar(
            const T& scalar
        ) const = 0;

        // Скалярное произведение двух векторов.
        virtual T DotProduct(
            const IVector<T>& other
        ) const = 0;

        // Евклидова норма вектора.
        virtual double Norm() const = 0;

        // Получение энумератора.
        virtual IEnumerator<T>* GetEnumerator() const override = 0;
        // Применение функции к каждой координате.
        template<class Func>
        IVector<T>* Map(Func function) const
        {
            IVector<T>* result = CreateEmpty();
            try
            {
                for (std::size_t i = 0; i < GetDimension(); ++i)
                {
                    result->AppendCoordinate(
                        function(Get(i))
                    );
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }

            return result;
        }
        // Отбор координат по заданному условию.
        template<class Predicate>
        IVector<T>* Where(Predicate predicate) const
        {
            IVector<T>* result = CreateEmpty();
            try
            {
                for (std::size_t i = 0; i < GetDimension(); ++i)
                {
                    T value = Get(i);

                    if (predicate(value))
                    {
                        result->AppendCoordinate(value);
                    }
                }
            }
            catch (...)
            {
                delete result;
                throw;
            }

            return result;
        }
        // Свёртка координат в одно значение.
        template<class Func, class TAccumulator>
        TAccumulator Reduce(
            Func function,
            TAccumulator initial
        ) const
        {
            TAccumulator accumulator = initial;
            for (std::size_t i = 0; i < GetDimension(); ++i)
            {
                accumulator = function(
                    Get(i),
                    accumulator
                );
            }
            return accumulator;
        }
    protected:
        // Создаёт пустой вектор того же конкретного вида.
        virtual IVector<T>* CreateEmpty() const = 0;

        virtual void AppendCoordinate(const T& value) = 0;
    };
}