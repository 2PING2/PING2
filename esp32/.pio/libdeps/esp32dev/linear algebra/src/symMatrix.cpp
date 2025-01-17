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

#define SYM_MATRIX_CPP
#include "symMatrix.hpp"

template <typename T>
const symMatrix<T> symMatrix<T>::staticHelper;

template <typename T>
symMatrix<T>::symMatrix(const size_t rows, const size_t cols) : MatrixBase<T>(rows, cols)
{
    if (rows != cols)
        throw "symMatrix::symMatrix() not a square matrix";
    Vector<T>::resize(MatrixBase<T>::minMemorySize(), false, false);
};

template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdMul(const symMatrix<U> &a, const symMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);

    Vector<T>::holdMul(a, b, false);
    return this;
};

////////////////////////////// ldl_matrix //////////////////////////////
template <typename T>
template<typename U> symMatrix<T> *symMatrix<T>::holdInv(ldl_matrix<U> &other, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::resizeLike(other);
    // if (checkOverlap)
    //     if(this->overlap(other))
    //         throw "symMatrix::holdInv() overlap";

    other.decompose();
    other.L.holdInv(other.L, false);
    other.D.holdInv(1,other.D, false);
    using namespace operators;
    return this->holdMul(*(other.L*other.D).release(),other.LT);
}


////////////////////////////// rowMajorMatrix and rowMajorMatrix //////////////////////////////
template <typename T>
template <typename U>
symMatrix<T> *symMatrix<T>::hold(const rowMajorMatrix<U> &other, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::resizeLike(other);
    size_t index = 0;
    size_t other_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++)
            this->_begin[index++] = other._begin[other_index + j];
        other_index += this->_cols;
    }
    return this;
};
template <typename T>
template<typename U> symMatrix<T> *symMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    size_t index_a = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a.cols(); k++)
                sum += a._begin[index_a + k] * b._begin[k * b._cols + j];
            this->_begin[index++] = sum;
        }
        index_a+=a._cols;
    }

    return this;
};

template <typename T>
template<typename U> symMatrix<T> *symMatrix<T>::addMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    size_t index_a = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a.cols(); k++)
                sum += a._begin[index_a + k] * b._begin[k * b._cols + j];
            this->_begin[index++] += sum;
        }
        index_a+=a._cols;
    }
    return this;
};

template <typename T>
template<typename U> symMatrix<T> *symMatrix<T>::subMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    if (checkOverlap)
        MatrixBase<T>::checkOverlap(a, b);
    size_t index = 0;
    size_t index_a = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a.cols(); k++)
                sum += a._begin[index_a + k] * b._begin[k * b._cols + j];
            this->_begin[index++] -= sum;
        }
        index_a+=a._cols;
    }
    return this;
};



////////////////////////////// rowMajorMatrix and symMatrix //////////////////////////////
template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdAdd(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++, index++)
            this->_begin[index] = a._begin[a_index + j] + b._begin[index];
        a_index += this->_cols;
    }
    return this;
};

template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdSub(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++, index++)
            this->_begin[index] = a._begin[a_index + j] - b._begin[index];
        a_index += this->_cols;
    }
    return this;
};

template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            size_t b_index = ((j * (j + 1)) >> 1);
            for (size_t k = 0; k < j; k++)
                sum += a._begin[a_index + k] * b._begin[b_index + k];
            for (size_t k = j; k < a._cols; k++)
                sum += a._begin[a_index + k] * b._begin[((k * (k + 1)) >> 1) + j];

            this->_begin[index++] = sum;
        }
        a_index += this->_rows;
    }
    return this;
};

////////////////////////////// symMatrix and rowMajorMatrix //////////////////////////////
template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdSub(const symMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_add(a, b);
    size_t index = 0;
    size_t b_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        for (size_t j = 0; j <= i; j++, index++)
            this->_begin[index] = a._begin[index] - b._begin[b_index + j];
        b_index += this->_cols;
    }
    return this;
};

template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdMul(const symMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize, const bool checkOverlap)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    // optimized on ESP32
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        const size_t a_index = ((i * (i + 1)) >> 1);
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < i; k++)
            {
                sum += a._begin[a_index + k] * b._begin[k*this->_cols];
            }
            for (size_t k = i; k < this->_cols; k++)
            {
                sum += a._begin[((k * (k + 1)) >> 1) + i] * b._begin[k*this->_cols];
            }
            this->_begin[index++] = sum;
        }
    }
    return this;
};

////////////////////////////// triangMatrix and uu_triangMatrix //////////////////////////////
template <typename T>
template<typename U> symMatrix<T> *symMatrix<T>::holdMul(const triangMatrix<U> &a, const uu_triangMatrix<U> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    size_t index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        const size_t a_index = (i * (i + 1)) >> 1;
        for (size_t j = 0; j <= i; j++)
        {
            const size_t b_index = ((j-1) * j) >> 1;
            T sum = 0;
            const size_t k_max = (i<j)?i:j;
            for (size_t k = 0; k < k_max; k++)
                sum += a[a_index + k] * b[b_index + k];
            this->_begin[index++] = sum + a[a_index + j];
        }
    }
    return this;
};


////////////////////////////// rowMajorMatrix and colMajorMatrix //////////////////////////////
template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = 0;
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a.cols(); k++)
                sum += a[a_index + k] * b[b_index + k];
            this->_begin[index++] = sum;
            b_index += b.rows();
        }
        a_index += a.cols();
    }
    return this;
};

template <typename T>
template <typename U, typename V>
symMatrix<T> *symMatrix<T>::addMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize)
{
    if (checkSize)
        MatrixBase<T>::checkSize_mul(a, b);
    size_t index = 0;
    size_t a_index = 0;
    for (size_t i = 0; i < this->_rows; i++)
    {
        size_t b_index = 0;
        for (size_t j = 0; j <= i; j++)
        {
            T sum = 0;
            for (size_t k = 0; k < a.cols(); k++)
                sum += a[a_index + k] * b[b_index + k];
            this->_begin[index++] += sum;
            b_index += b.rows();
        }
        a_index += a.cols();
    }
    return this;
};