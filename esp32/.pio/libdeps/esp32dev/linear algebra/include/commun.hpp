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


#ifndef COMMUN_HPP
#define COMMUN_HPP

#ifdef NATIVE
#include <iostream>
// typedef unsigned long long size_t;
#include <cstring>
#include <string>
typedef std::string String;
#else
#include <Arduino.h>
#endif


template <typename T = float>
class Vector;

template <typename T>
class rowMajorMatrix;

template <typename T>
class symMatrix;


namespace internal
{
    // float InvSqrt43 (float x);
    static size_t vec_count = 0;
    static size_t alloc_count = 0;
    template <typename T> T &&move(T &a) noexcept;
    template <typename T> void swap(T &a, T &b) noexcept;
    template <class Derived>
    class tmp;
    template <typename T> static const T _zero = T();
    template <typename T> static const T _one = T(1);
} // namespace internal;

namespace operators
{
    static bool VectorCheckSize = true;
    static bool MatrixCheckSize = true;
} // namespace operators


#include "vector.hpp"
namespace internal
{
    static size_t tmp_count = 0;
    template <class Derived>
    class tmp : public Derived
    {
    private:
        static Vector<tmp *> buffer;

    public:
        bool currentlyUsed = true;
        using Derived::Derived;
        template <typename... Args>
        static tmp *get(Args... shape);
        tmp *release();
        static const size_t currentlyUsedCount();
        static const size_t bufferSize() { return buffer.size(); }
        static void freeAll();
    };

} // namespace internal

#ifndef COMMUN_CPP
#include "commun.cpp"
#endif // COMMUN_CPP

#endif // COMMUN_HPP