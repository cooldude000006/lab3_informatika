#pragma once
#include <stdexcept>
#include "exceptions.h"

namespace lab2
{
    template <typename T>
    class Option
    {
    private:
        T value_;
        bool has_value_;

        Option(T val, bool has) : value_(val), has_value_(has)
        {
        }
    public:
        static Option<T> Some(T value) //Создает Option с значением
        {
            return Option<T>(value, true);
        }

        static Option<T> None() //Создает пустой Option
        {
            return Option<T>(T(), false);
        }

        bool HasValue() const //Проверка наличия значения
        {
            return has_value_;
        }

        T GetValue() const //Получение значения
        {
            if (!has_value_)
            {
                throw InvalidOperationException("Попытка получить значение из пустого Option");
            }
            return value_;
        }

        T GetValueOr(T defaultValue) const //Получение значения или значения по умолчанию
        {
            return has_value_ ? value_ : defaultValue;
        }

        explicit operator bool() const //Оператор приведения к bool
        {
            return has_value_;
        }
    };

}

