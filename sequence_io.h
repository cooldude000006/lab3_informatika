#pragma once
#include <ostream>
#include "sequence.h"
template<typename T>
std::ostream& operator<<(std::ostream& output,
                         const Sequence<T>& sequence)
{
    output << "[";

    for (int i = 0; i < sequence.GetLength(); ++i)
    {
        output << sequence.Get(i);

        if (i + 1 < sequence.GetLength())
        {
            output << ", ";
        }
    }

    output << "]";
    return output;
}