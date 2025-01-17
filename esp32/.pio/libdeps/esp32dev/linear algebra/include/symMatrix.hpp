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

#ifndef SYM_MATRIX_HPP
#define SYM_MATRIX_HPP

#include "rowMajorMatrix.hpp"
#include "ldl_Matrix.hpp"
#include "triangMatrix.hpp"

template <typename T>
class symMatrix : public MatrixBase<T>
{
        friend class internal::tmp<symMatrix>;

    protected:
        symMatrix *swap(symMatrix &other) noexcept { return (symMatrix *)(MatrixBase<T>::swap(other)); }
        const size_t minMemorySize(const size_t order) const noexcept { return (order * (order + 1)) >> 1; }
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return minMemorySize(rows); }
        static const symMatrix<T> staticHelper;

    public:
        virtual T &operator()(const size_t row, const size_t col) override { return (row >= col) ? this->_begin[((row * (row + 1)) >> 1) + col] : this->_begin[((col * (col + 1)) >> 1) + row]; }

        symMatrix() : MatrixBase<T>(){};
        symMatrix(const size_t order) : MatrixBase<T>(order, order){Vector<T>::resize(MatrixBase<T>::minMemorySize(), false, false);};
        symMatrix(const size_t rows, const size_t cols);

        // symMatrix and data_type
        template<typename U> symMatrix<T> *holdAdd(const symMatrix<U> &a, const T &b, const bool checkSize = true){ return (symMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U> symMatrix<T> *holdSub(const symMatrix<U> &a, const T &b, const bool checkSize = true){ return (symMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> symMatrix<T> *holdSub(const T &a, const symMatrix<U> &b, const bool checkSize = true){ return (symMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> symMatrix<T> *holdMul(const symMatrix<U> &a, const T &b, const bool checkSize = true){ return (symMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };

        // symMatrix and symMatrix
        symMatrix *hold(const symMatrix &other, const bool checkSize = true){ return (symMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> symMatrix *hold(const symMatrix<U> &other, const bool checkSize = true){ return (symMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> symMatrix *holdAdd(const symMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true){ return (symMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a,b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> symMatrix *holdSub(const symMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true){ return (symMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a,b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U, typename V> symMatrix *holdMul(const symMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        // symMatrix and ldl_matrix
        template<typename U> symMatrix *holdInv(ldl_matrix<U> &other, const bool checkSize = true);
        // rowMajorMatrix and rowMajorMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *hold(const rowMajorMatrix<U> &other, const bool checkSize = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize = true, const bool checkOverlap = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *addMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize = true, const bool checkOverlap = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *subMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<U> &b, const bool checkSize = true, const bool checkOverlap = true);
        // rowMajorMatrix and symMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdAdd(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdSub(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdMul(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        // symMatrix and rowMajorMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdSub(const symMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdMul(const symMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        // rowMajorMatrix and colMajorMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U, typename V> symMatrix *addMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        // triangMatrix and uu_triangMatrix
        template<typename U> symMatrix *holdMul(const triangMatrix<U> &a, const uu_triangMatrix<U> &b, const bool checkSize = true);
        ////////////////////////// operators //////////////////////////
        // dataType
        symMatrix *operator+=(const T &data) { return (symMatrix *)this->Vector<T>::operator+=(data); };
        symMatrix *operator-=(const T &data) { return (symMatrix *)this->Vector<T>::operator-=(data); };
        symMatrix *operator*=(const T &data) { return (symMatrix *)this->Vector<T>::operator*=(data); };
        symMatrix *operator/=(const T &data) { return (symMatrix *)this->Vector<T>::operator/=(data); };

        // symMatrix
        symMatrix *operator=(const symMatrix &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator=(const symMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator+=(const symMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator-=(const symMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator*=(const symMatrix<U> &other) { return this->swap(*internal::tmp<symMatrix>::get(this->_rows, other.cols())->release()->holdMul(*this, other, operators::MatrixCheckSize, false)); };

        // tmp symMatrix
        symMatrix *operator=(internal::tmp<symMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template<typename U> symMatrix *operator=(internal::tmp<symMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator+=(internal::tmp<symMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator-=(internal::tmp<symMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> symMatrix *operator*=(internal::tmp<symMatrix<U>> &&other) { return this->swap(*internal::tmp<symMatrix>::get(this->_rows, other.cols())->release()->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)); };
      
        // rowMajorMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator=(const rowMajorMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator+=(const rowMajorMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator-=(const rowMajorMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator*=(const rowMajorMatrix<U> &other) {  return this->swap(*internal::tmp<symMatrix>::get(this->_rows, other.cols())->release()->holdMul(*this, other, operators::MatrixCheckSize, false)); };

        // tmp rowMajorMatrix
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator+=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator-=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        // !!! warning: loss of information !!! Normally, this operations give not obviously symmetrical matrices, but it speed up the calculation if you are sure that the result is symmetrical.
        template<typename U> symMatrix *operator*=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->swap(*internal::tmp<symMatrix>::get(this->_rows, other.cols())->release()->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)); };

};

namespace operators
{
    // dataType and symMatrix
    template <typename T> internal::tmp<symMatrix<T>> &&operator+(const T &a, const symMatrix<T> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator-(const T &a, const symMatrix<T> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(b.rows(), b.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator*(const T &a, const symMatrix<T> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(b.rows(), b.cols())->holdMul(b, a, operators::MatrixCheckSize)); };
    // symMatrix and dataType
    template <typename T> internal::tmp<symMatrix<T>> &&operator+(const symMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator-(const symMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator*(const symMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator/(const symMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, 1 / b, operators::MatrixCheckSize)); };
    // tmp symMatrix and dataType
    template <typename T> internal::tmp<symMatrix<T>> &&operator+(internal::tmp<symMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdAdd(*a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator-(internal::tmp<symMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdSub(*a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator*(internal::tmp<symMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdMul(*a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator/(internal::tmp<symMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdMul(*a, 1 / b, operators::MatrixCheckSize)); };
    // dataType and tmp symMatrix
    template <typename T> internal::tmp<symMatrix<T>> &&operator+(const T &a, internal::tmp<symMatrix<T>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdAdd(*b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator-(const T &a, internal::tmp<symMatrix<T>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdSub(a, *b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<symMatrix<T>> &&operator*(const T &a, internal::tmp<symMatrix<T>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdMul(*b, a, operators::MatrixCheckSize)); };
    // symMatrix and symMatrix
    template <typename T, typename U, typename V=decltype(T()+U())> internal::tmp<symMatrix<V>> &&operator+(const symMatrix<T> &a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()-U())> internal::tmp<symMatrix<V>> &&operator-(const symMatrix<T> &a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()*U())> internal::tmp<symMatrix<V>> &&operator*(const symMatrix<T> &a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    // tmp symMatrix and symMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator+(internal::tmp<symMatrix<T>> &&a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator-(internal::tmp<symMatrix<T>> &&a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()*U())> internal::tmp<symMatrix<V>> &&operator*(internal::tmp<symMatrix<T>> &&a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    // symMatrix and tmp symMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator+(const symMatrix<T> &a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator-(const symMatrix<T> &a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()*U())> internal::tmp<symMatrix<V>> &&operator*(const symMatrix<T> &a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a.rows(), b->cols())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); };
    // tmp symMatrix and tmp symMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator+(internal::tmp<symMatrix<T>> &&a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator-(internal::tmp<symMatrix<T>> &&a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()*U())> internal::tmp<symMatrix<V>> &&operator*(internal::tmp<symMatrix<T>> &&a, internal::tmp<symMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<V>>*)internal::tmp<symMatrix<V>>::get(a->rows(), b->cols())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };
    // triangMatrix and uu_triangMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator*(const triangMatrix<T> &a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)internal::tmp<symMatrix<T>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    // tmp symMatrix and uu_triangMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator*(internal::tmp<symMatrix<T>> &&a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdMul(*a, b, operators::MatrixCheckSize)); };
    // symMatrix and tmp symMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator*(const symMatrix<T> &a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)b->holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    // tmp symMatrix and tmp uu_triangMatrix
    template <typename T, typename U> internal::tmp<symMatrix<T>> &&operator*(internal::tmp<symMatrix<T>> &&a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<symMatrix<T>>*)a->holdMul(*a, *b.release(), operators::MatrixCheckSize)); };
}


#ifndef SYM_MATRIX_CPP
#include "symMatrix.cpp"
#endif // SYM_MATRIX_CPP

#endif // SYM_MATRIX_HPP