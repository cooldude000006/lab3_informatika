#pragma once
#include <complex>

namespace lab2
{
    namespace vector_detail
    {
        // Для int, double и других обычных числовых типов.
        template<class T>
        T DotProductTerm(
            const T& left,
            const T& right
        )
        {
            return left * right;
        }

        // Для комплексных чисел используется
        // сопряжение координаты первого вектора.
        template<class T>
        std::complex<T> DotProductTerm(
            const std::complex<T>& left,
            const std::complex<T>& right
        )
        {
            return std::conj(left) * right;
        }
    }
}