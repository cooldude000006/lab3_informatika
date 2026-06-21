#pragma once
#include "option.h"

namespace lab2
{
    template <typename T>
    class IEnumerator
    {
    public:
        virtual ~IEnumerator() = default; //Переместить курсор к следующему элементу

        virtual bool MoveNext() = 0; //Получить текущий элемент

        virtual Option<T> GetCurrent() const = 0; //Получить текущий элемент

        virtual void Reset() = 0; // Сбросить курсор в начало(до первого)
    };
}