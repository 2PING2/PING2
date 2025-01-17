/*
 * Copyright (C) 2024 robinAZERTY [https://github.com/robinAZERTY]
 *
 * This file is part of linearAlgebra library.
 *
 * linearAlgebra library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linearAlgebra library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linearAlgebra library. If not, see <https://www.gnu.org/licenses/>.
 */

#define COMMUN_CPP
#include "commun.hpp"

template <typename T>
using Vector_f3 = internal::tmp<Vector<T>> && (*)(const Vector<T> &, const Vector<T> &, const Vector<T> &);
template <typename T>
using Vector_f2 = internal::tmp<Vector<T>> && (*)(const Vector<T> &, const Vector<T> &);
template <typename T>
using Vector_f1 = internal::tmp<Vector<T>> && (*)(const Vector<T> &);
template <typename T>
using Matrix_f3 = internal::tmp<Matrix<T>> && (*)(const Vector<T> &, const Vector<T> &, const Vector<T> &);
template <typename T>
using Matrix_f2 = internal::tmp<Matrix<T>> && (*)(const Vector<T> &, const Vector<T> &);
template <typename T>
using Matrix_f1 = internal::tmp<Matrix<T>> && (*)(const Vector<T> &);

namespace internal
{
    template <typename T>
    T &&move(T &a) noexcept
    {
        return static_cast<T &&>(a);
    }

    template <typename T>
    void swap(T &a, T &b) noexcept
    {
        T tmp = move(a);
        a = move(b);
        b = move(tmp);
    }
    template <class Derived>
    Vector<tmp<Derived> *> tmp<Derived>::buffer;

    template <class Derived>
    template <typename... Args>
    tmp<Derived> *tmp<Derived>::get(Args... shape)
    {
        // look for the best tmp vector, the one with the smallest length but still enough
        size_t bestUnusedIndex = -1;
        size_t bestUsedIndex = -1;
        size_t best_unused_cap = 0;
        size_t best_used_cap = 0;
        size_t current_cap = 0;
        const size_t buffer_size = buffer.size();
        const size_t needed_N = Derived::staticHelper.minMemorySize(shape...);
        for (size_t i = 0; i < buffer_size; i++)
        {
            current_cap = buffer[i]->capacity();
            if (!buffer[i]->currentlyUsed)
                if (current_cap >= needed_N)
                {
                    if (bestUnusedIndex == -1 || current_cap < best_unused_cap)
                    {
                        best_unused_cap = current_cap;
                        bestUnusedIndex = i;
                    }
                }
                else
                {
                    if (bestUsedIndex == -1 || current_cap > best_used_cap)
                    {
                        best_used_cap = current_cap;
                        bestUsedIndex = i;
                    }
                }
        }

        size_t bestIndex = bestUnusedIndex != -1 ? bestUnusedIndex : bestUsedIndex;
        if (bestIndex != -1)
        {
            buffer[bestIndex]->template resize(shape..., false, false);
            buffer[bestIndex]->currentlyUsed = true;
            return buffer[bestIndex];
        }
        // means that no vector was found
        if (buffer.push_back(new tmp<Derived>(shape...)))
        {
            tmp_count++;
            return buffer[buffer_size];
        }
        return nullptr;
    }

    template <class Derived>
    tmp<Derived> *tmp<Derived>::release()
    {
        currentlyUsed = false;
        return this;
    }

    template <class Derived>
    const size_t tmp<Derived>::currentlyUsedCount()
    {
        size_t count = 0;
        const size_t buffer_size = buffer.size();
        for (size_t i = 0; i < buffer_size; i++)
            if (buffer[i]->currentlyUsed)
                count++;
        return count;
    }

    template <class Derived>
    void tmp<Derived>::freeAll()
    {
        const size_t buffer_size = buffer.capacity();
        for (size_t i = 0; i < buffer_size; i++)
            delete buffer[i];
        buffer.resize(0,true,false);
        tmp_count -= buffer_size;
    }
};