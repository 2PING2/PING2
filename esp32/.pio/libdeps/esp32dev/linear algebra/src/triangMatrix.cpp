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

#define TRIANG_MATRIX_CPP
#include "triangMatrix.hpp"

template <typename T>
const triangMatrix<T> triangMatrix<T>::staticHelper;

template <typename T>
T &triangMatrix<T>::operator()(const size_t row, const size_t col)
{ 
    if (row >= col)
        return this->_begin[((row * (row + 1)) >> 1) + col];
    throw "index inaccessable";
    return this->_begin[-1]; 
};

// triangMatrix and triangMatrix
template <typename T>
template <typename U, typename V>
triangMatrix<T> *triangMatrix<T>::holdMul(const triangMatrix<U> &a, const triangMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        const size_t a_index = (i * (i + 1)) >> 1;
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = j; k <= i; k++)
                sum += a._begin[a_index + k] * b._begin[((k * (k + 1)) >> 1) + j];
                
            this->_begin[index++] = sum;
        }
    }
    return this;
};

// ul_triangMatrix and diagMatrix
template <typename T>
template<typename U, typename V> triangMatrix<T> *triangMatrix<T>::holdMul(const ul_triangMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t a_index = ((i-1) * i) >> 1;
        for (size_t j = 0; j < i; j++)
            this->_begin[index++] = a[a_index + j] * b[j];
        this->_begin[index++] = b[i];   
    }
    return this;
};
