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

#define ROW_MAJOR_MATRIX_CPP
#include "rowMajorMatrix.hpp"

template <typename T>
const rowMajorMatrix<T> rowMajorMatrix<T>::staticHelper;


template <typename T>
rowMajorMatrix<T>::rowMajorMatrix(const size_t rows, const size_t cols) : MatrixBase<T>(rows, cols) 
{
    Vector<T>::resize(MatrixBase<T>::minMemorySize(), false, false);
}

template <typename T>
rowMajorMatrix<T>::rowMajorMatrix(T *data, const size_t rows, const size_t cols, const bool share) : MatrixBase<T>(rows, cols)
{
    if (share)
        Vector<T>::refer(data, MatrixBase<T>::minMemorySize());
    else
        Vector<T>::hold(data, MatrixBase<T>::minMemorySize());
}

////////////////////////// rowMajorMatrix and rowMajorMatrix //////////////////////////
template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    // optimized on ESP32
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a._cols; k++)
                sum += a._begin[a_index + k] * b._begin[k*b._cols+j];
            this->_begin[index++] = sum;
        }
        a_index += a._cols;
    }
    return this;
}

////////////////////////// colMajorMatrix and colMajorMatrix //////////////////////////
template <typename T>
template <typename U>
rowMajorMatrix<T> *rowMajorMatrix<T>::hold(const colMajorMatrix<U> &other, const bool checkSize)
{
    if (checkSize)
        this->resizeLike(other, false, false);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t other_index = i;
        for (size_t j = 0; j < this->_cols; j++)
        {
            this->_begin[index++] = other._begin[other_index];
            other_index += other._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdAdd(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t other_index = i;
        for (size_t j = 0; j < this->_cols; j++)
        {
            this->_begin[index++] = a._begin[other_index] + b._begin[other_index];
            other_index += a._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdSub(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t other_index = i;
        for (size_t j = 0; j < this->_cols; j++)
        {
            this->_begin[index++] = a._begin[other_index] - b._begin[other_index];
            other_index += a._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = 0;
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            size_t a_index = i;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index] * b._begin[b_index + k];
                a_index += a._rows;
            }
            this->_begin[index++] = sum;
            b_index += b._rows;
        }
    }
    return this;
}

////////////////////////// rowMajorMatrix and colMajorMatrix //////////////////////////
template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdAdd(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
    {
        if (!a.isAdditionCompatible(b))
            throw "Matrices are not compatible for addition";
        this->resizeLike(a, false, false);
    }
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = i;
        for (size_t j = 0; j < this->_cols; j++, index++)
        {
            this->_begin[index] = a._begin[index] + b._begin[b_index];
            b_index += b._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdSub(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = i;
        for (size_t j = 0; j < this->_cols; j++, index++)
        {
            this->_begin[index] = a._begin[index] - b._begin[b_index];
            b_index += b._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdSub(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t a_index = i;
        for (size_t j = 0; j < this->_cols; j++, index++)
        {
            this->_begin[index] = a._begin[a_index] - b._begin[index];
            a_index += b._rows;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = 0;
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index+k] * b._begin[b_index + k];
            }
            this->_begin[index++] = sum;
            b_index += b._rows;
        }
        a_index += a._rows;
    }

    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            size_t a_index = i;
            size_t b_index = j;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index] * b._begin[b_index];
                a_index += a._rows;
                b_index += b._cols;
            }
            this->_begin[index++] = sum;
        }
    }
    return this;
}

////////////////////////// rowMajorMatrix and diagMatrix //////////////////////////
template <typename T>
template <typename U>
rowMajorMatrix<T> *rowMajorMatrix<T>::hold(const diagMatrix<U> &other, const bool checkSize)
{
    if (checkSize)
        this->resizeLike(other, false, false);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
        for (size_t j = 0; j < this->_cols; j++)
            this->_begin[index++] = (i == j) ? other._begin[i] : internal::_zero<T>;
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdAdd(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);

    if (this != &a)
        this->hold(a, false);

    const size_t b_size = b.size();
    size_t index = 0;
    for (size_t i = 0; i < b_size; i++)
    {
        this->_begin[index] += b._begin[i];
        index += this->_cols + 1;
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdSub(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    if (this != &a)
        this->hold(a, false);

    const size_t b_size = b.size();
    size_t index = 0;
    for (size_t i = 0; i < b_size; i++)
    {
        this->_begin[index] -= b._begin[i];
        index += this->_cols + 1;
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
                if (j >= a._cols)
                    this->_begin[index++] = internal::_zero<T>;
                else
                    this->_begin[index++] = a._begin[a_index+j] * b._begin[j];
        a_index += a._cols;
    }
    return this;
}

template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdDiv(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
    {
        b.checkIsSquare();
        MatrixBase<T>::checkSize_mul(a, b);
    }
    
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
                this->_begin[index++] = a._begin[a_index+j] / b._begin[j];
        a_index += a._cols;
    }
    return this;
}

////////////////////////// diagMatrix and rowMajorMatrix //////////////////////////
template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdSub(const diagMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    
    const size_t size = this->size();
    for (size_t i = 0; i < size; i++)
        this->_begin[i] = -a._begin[i];

    const size_t b_size = b.size();
    size_t index = 0;
    for (size_t i = 0; i < b_size; i++)
    {
        this->_begin[index] += b._begin[i];
        index += this->_cols + 1;
    }
    return this;
}
template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const diagMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);

    size_t index = 0;
    size_t b_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
            if (i>=this->_cols)
                this->_begin[index++] = internal::_zero<T>;
            else
                this->_begin[index++] = a._begin[i]*b._begin[b_index+j];

        b_index += this->_cols;
    }

    return this;
}

////////////////////////// rowMajorMatrix and symMatrix //////////////////////////
template <typename T>
template <typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);

    // optimized on ESP32
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            const size_t b_index = (j * (j + 1)) >> 1;
            for (size_t k = 0; k < j; k++)
                sum += a._begin[a_index + k] * b[b_index + k];
            for (size_t k = j; k < a._cols; k++)
                sum += a._begin[a_index + k] * b[((k * (k + 1)) >> 1) + j];

            this->_begin[index++] = sum;
        }
        a_index += a._cols;
    }
    return this;
}

////////////////////////// colMajorMatrix and symMatrix //////////////////////////
template <typename T>
template<typename U, typename V>
rowMajorMatrix<T> *rowMajorMatrix<T>::holdMul(const colMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
    MatrixBase<T>::checkSize_mul(a, b);

    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j < this->_cols; j++)
        {
            T sum = 0;
            size_t a_index = i;
            const size_t b_index = (j * (j + 1)) >> 1; 
            
            for (size_t k = 0; k < j; k++)
            {
                sum += a._begin[a_index] * b[b_index + k];
                a_index += a._rows;
            }
            for (size_t k = j; k < a._cols; k++)
            {
                sum += a._begin[a_index] * b[((k * (k + 1)) >> 1) + j];
                a_index += a._rows;
            }
            this->_begin[index++] = sum;
        }
    }
    return this;
}
