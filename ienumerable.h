#pragma once
#include "ienumerator.h"

namespace lab3 {
    template <typename T>
    class IEnumerable {
    public:
        virtual ~IEnumerable() = default;

        virtual IEnumerator<T>* GetEnumerator() const = 0;
    };
}