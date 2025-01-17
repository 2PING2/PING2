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

#define COL_MAJOR_MATRIX_CPP
#include "colMajorMatrix.hpp"

template <typename T>
const colMajorMatrix<T> colMajorMatrix<T>::staticHelper;

template <typename T>
colMajorMatrix<T>::colMajorMatrix(const size_t rows, const size_t cols) : MatrixBase<T>(rows, cols)
{
    Vector<T>::resize(MatrixBase<T>::minMemorySize(), false, false);
}

template <typename T>
colMajorMatrix<T>::colMajorMatrix(T *data, const size_t rows, const size_t cols, const bool share) : MatrixBase<T>(rows, cols)
{
    if (share)
        Vector<T>::refer(data, MatrixBase<T>::minMemorySize());
    else
        Vector<T>::hold(data, MatrixBase<T>::minMemorySize());

}

////////////////////////// colMajorMatrix and colMajorMatrix //////////////////////////
template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdMul(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    this->resize(a._rows, b._cols, false, false);
    size_t index = 0;
    size_t b_index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        for (size_t i = 0; i < this->_rows; i++)
        {
            T sum = 0;
            size_t a_index = i;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index] * b._begin[b_index + k];
                a_index += this->_rows;
            }
            this->_begin[index++] = sum;
        }
        b_index += b._rows;
    }
    return this;
}

////////////////////////// rowMajorMatrix and rowMajorMatrix //////////////////////////
template <typename T>
template <typename U>
colMajorMatrix<T> *colMajorMatrix<T>::hold(const rowMajorMatrix<U> &other, const bool checkSize)
{
    if (checkSize)
        this->resizeLike(other, false, false);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t other_index = j;
        for (size_t i = 0; i < this->_rows; i++)
        {
            this->_begin[index++] = other._begin[other_index];
            other_index += other._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdAdd(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t other_index = j;
        for (size_t i = 0; i < this->_rows; i++)
        {
            this->_begin[index++] = a._begin[other_index] + b._begin[other_index];
            other_index += a._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdSub(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t other_index = j;
        for (size_t i = 0; i < this->_rows; i++)
        {
            this->_begin[index++] = a._begin[other_index] - b._begin[other_index];
            other_index += a._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t a_index = 0;
        for (size_t i = 0; i < this->_rows; i++)
        {
            T sum = 0;
            size_t b_index = j;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index + k] * b._begin[b_index];
                b_index += b._cols;
            }
            this->_begin[index++] = sum;
            a_index += a._cols;
        }
    }
    return this;
}

////////////////////////// colMajorMatrix and rowMajorMatrix //////////////////////////

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdAdd(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t b_index = j;
        for (size_t i = 0; i < this->_rows; i++, index++)
        {
            this->_begin[index] = a._begin[index] + b._begin[b_index];
            b_index += b._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdSub(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t b_index = j;
        for (size_t i = 0; i < this->_rows; i++, index++)
        {
            this->_begin[index] = a._begin[index] - b._begin[b_index];
            b_index += b._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdSub(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t a_index = j;
        for (size_t i = 0; i < this->_rows; i++, index++)
        {
            this->_begin[index] = a._begin[a_index] - b._begin[index];
            a_index += a._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdMul(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        for (size_t i = 0; i < this->_rows; i++)
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
            a_index += a._cols;
        }
    }
    return this;
}

template <typename T>
template <typename U, typename V>
colMajorMatrix<T> *colMajorMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    size_t b_index = 0;
    for (size_t j = 0; j < this->_cols; j++)
    {
        size_t a_index = 0;
        for (size_t i = 0; i < this->_rows; i++)
        {
            T sum = 0;
            for (size_t k = 0; k < a._cols; k++)
            {
                sum += a._begin[a_index + k] * b._begin[b_index + k];
            }
            this->_begin[index++] = sum;
            a_index += a._cols;
        }
        b_index += b._rows;
    }
    return this;
}