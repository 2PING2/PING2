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

#include "commun.hpp"

#ifndef MATRIX_BASE_HPP
#define MATRIX_BASE_HPP

template <typename T = float>
class rowMajorMatrix;

template <typename T = float>
class colMajorMatrix;

template <typename T = float>
class Matrix;

template <typename T = float>
class diagMatrix;

template <typename T = float>
class symMatrix;

template <typename T = float>
class ul_triangMatrix;

template <typename T = float>
class uu_triangMatrix;

template <typename T = float>
class ldl_matrix;

template <typename T = float>
class MatrixBase : public Vector<T>
{
    template <typename U>
    friend class Vector;
    template <typename U>
    friend class MatrixBase;
    template <typename U>
    friend class rowMajorMatrix;
    template <typename U>
    friend class colMajorMatrix;
    template <typename U>
    friend class Matrix;
    template <typename U>
    friend class diagMatrix;
    template <typename U>
    friend class symMatrix;
    template <typename U>
    friend class ul_triangMatrix;
    template <typename U>
    friend class uu_triangMatrix;
    template <typename U>
    friend class ldl_matrix;

protected:
    size_t _rows = 0;
    size_t _cols = 0;
    MatrixBase<T> *swap(MatrixBase<T> &other) noexcept;
    virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept = 0;
    const size_t minMemorySize() const noexcept { return minMemorySize(_rows, _cols); }

public:
    MatrixBase() : Vector<T>(){};
    MatrixBase(const size_t rows, const size_t cols) : _rows(rows), _cols(cols) {}
    const size_t rows() const noexcept { return _rows; }
    const size_t cols() const noexcept { return _cols; }

    virtual T &operator()(const size_t row, const size_t col) = 0;
    virtual const T &operator()(const size_t row, const size_t col) const { return const_cast<MatrixBase<T> *>(this)->operator()(row, col); }

    template <typename U>
    bool isAdditionCompatible(const MatrixBase<U> &other) const noexcept { return _rows == other._rows && _cols == other._cols; }
    template <typename U>
    bool isMultiplicationCompatible(const MatrixBase<U> &other) const noexcept { return _cols == other._rows; }

    virtual MatrixBase<T> *resize(const size_t rows, const size_t cols, const bool deallocIfPossible = false, const bool saveData = true)
    {
        if (rows == _rows && cols == _cols)
            return this;
        _rows = rows;
        _cols = cols;
        Vector<T>::resize(minMemorySize(), deallocIfPossible, saveData);
        return this;
    }

    template <typename U>
    MatrixBase<T> *resizeLike(const MatrixBase<U> &other, const bool deallocIfPossible = false, const bool saveData = true) { return resize(other._rows, other._cols, deallocIfPossible, saveData); }
    MatrixBase<T> *fill(const T value) { return (MatrixBase<T> *)Vector<T>::fill(value); }

protected:

    template <typename U, typename V>
    MatrixBase<T> *checkSize_add(const MatrixBase<U> &a, const MatrixBase<V> &b)
    {
        if (!a.isAdditionCompatible(b))
            throw "Matrices are not compatible for addition";
        return this->resizeLike(a, false, false);

    }
    template <typename U, typename V>
    MatrixBase<T> *checkSize_mul(const MatrixBase<U> &a, const MatrixBase<V> &b)
    {
        if (!a.isMultiplicationCompatible(b))
        {
            throw "Matrices are not compatible for multiplication";
            return nullptr;
        }
        this->resize(a._rows, b._cols, false, false);
        return this;
    }
    template <typename U, typename V>
    MatrixBase<T> *checkOverlap(const MatrixBase<U> &a, const MatrixBase<V> &b)
    {
        if (this->overlap(a) || this->overlap(b))
        {
            throw "Matrices overlap";
            return nullptr;
        }
        return this;
    }


    MatrixBase<T> *refer(const Vector<T> &data, const size_t rows, const size_t cols) noexcept
    {
        _rows = rows;
        _cols = cols;
        Vector<T>::refer(data.begin(), minMemorySize());
        return this;
    }
    MatrixBase<T> *refer(const T *data, const size_t rows, const size_t cols) noexcept
    {
        _rows = rows;
        _cols = cols;
        Vector<T>::refer(data, minMemorySize());
        return this;
    }
};

/* 
template <typename T>
class Derived : public MatrixBase<T>
{
    friend class internal::tmp<Derived>;
    protected:
        Derived * swap(Derived &other) noexcept;
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override;

    public:
        virtual T &operator()(const size_t row, const size_t col) override;

};
*/

template <typename T>
MatrixBase<T> *MatrixBase<T>::swap(MatrixBase<T> &other) noexcept
{
    size_t tmp = this->_rows;
    this->_rows = other._rows;
    other._rows = tmp;
    tmp = this->_cols;
    this->_cols = other._cols;
    other._cols = tmp;
    Vector<T>::swap(other);
    return this;
}

template <typename T>
String to_string(const MatrixBase<T> &matrix)
{
    String s = "";
    for (size_t i = 0; i < matrix.rows(); i++)
    {
        for (size_t j = 0; j < matrix.cols(); j++)
        {
            #ifdef ARDUINO
            s += String(matrix(i, j))+ " ";
            #else
            s += std::to_string(matrix(i, j))+ " ";
            #endif
        }

        s += "\n";
    }
    return s;
}

#endif // MATRIX_HPP