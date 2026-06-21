#pragma once
#include "ienumerator.h"

namespace lab2 {
    template <typename T>
    class IEnumerable {
    public:
        virtual ~IEnumerable() = default;

        virtual IEnumerator<T>* GetEnumerator() const = 0;
    };
}