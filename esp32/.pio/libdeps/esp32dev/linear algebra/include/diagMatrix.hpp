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

#ifndef DIAG_MATRIX_HPP
#define DIAG_MATRIX_HPP

#include "matrixBase.hpp"

template <typename T>
class diagMatrix : public MatrixBase<T>
{
    friend class internal::tmp<diagMatrix>;

protected:
    diagMatrix<T> *swap(diagMatrix<T> &other) noexcept { return (diagMatrix<T> *)this->MatrixBase<T>::swap(other); }
    virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return (rows > cols) ? rows : cols; }
    static const diagMatrix<T> staticHelper;

public:
    diagMatrix<T> *checkIsSquare() const { if (this->rows() != this->cols()) throw "diagMatrix::det() not a square matrix"; return (diagMatrix<T> *)this; }

    virtual T &operator()(const size_t row, const size_t col) override;
    const T &operator()(const size_t row, const size_t col) const override { return (row == col) ? this->_begin[row] : internal::_zero<T>; }

    diagMatrix(const size_t rows, const size_t cols) : MatrixBase<T>(rows, cols) { Vector<T>::resize(MatrixBase<T>::minMemorySize()); }
    diagMatrix(const size_t size) : diagMatrix(size, size) {}
    diagMatrix() : MatrixBase<T>() {}

    // diagMatrix and dataType
    // diagMatrix<T> *hold(const T *data){ return (diagMatrix<T> *)this->Vector<T>::hold(data, checkSize); };
    template <typename U> diagMatrix<T> *holdAdd(const diagMatrix<U> &a, const T &b, const bool checkSize = true){ return (diagMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
    template <typename U> diagMatrix<T> *holdSub(const diagMatrix<U> &a, const T &b, const bool checkSize = true){ return (diagMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
    template <typename U> diagMatrix<T> *holdSub(const T &a, const diagMatrix<U> &b, const bool checkSize = true){ return (diagMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
    template <typename U> diagMatrix<T> *holdMul(const diagMatrix<U> &a, const T &b, const bool checkSize = true){ return (diagMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
    template <typename U> diagMatrix<T> *holdDiv(const diagMatrix<U> &a, const T &b, const bool checkSize = true){ return (diagMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdDiv(a, b, false); };
    template <typename U> diagMatrix<T> *holdInv(const T val, const diagMatrix<U> &other, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::resizeLike(*other.checkIsSquare()):this)->Vector<T>::holdDiv(val, other, false); };

    // diagMatrix and diagMatrix
    diagMatrix<T> *hold(const diagMatrix<T> &other, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
    template<typename U> diagMatrix<T> *hold(const diagMatrix<U> &other, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
    template<typename U, typename V> diagMatrix<T> *holdAdd(const diagMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdAdd(a, b, false); };
    template<typename U, typename V> diagMatrix<T> *holdSub(const diagMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdSub(a, b, false); };
    template<typename U, typename V> diagMatrix<T> *holdMul(const diagMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true){ return (diagMatrix *)(checkSize? MatrixBase<T>::checkSize_mul(a, b):this)->Vector<T>::holdMul(a, b, false); };
    ////////////////////////// operators //////////////////////////
    // dataType
    diagMatrix *operator+=(const T &data) { return (diagMatrix *)this->Vector<T>::operator+=(data); };
    diagMatrix *operator-=(const T &data) { return (diagMatrix *)this->Vector<T>::operator-=(data); };
    diagMatrix *operator*=(const T &data) { return (diagMatrix *)this->Vector<T>::operator*=(data); };
    diagMatrix *operator/=(const T &data) { return (diagMatrix *)this->Vector<T>::operator/=(data); };

    // diagMatrix
    diagMatrix *operator=(const diagMatrix &other) { return this->hold(other, operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator=(const diagMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator+=(const diagMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator-=(const diagMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator*=(const diagMatrix<U> &other) { return this->holdMul(*this, other, operators::MatrixCheckSize); };

    // tmp diagMatrix
    diagMatrix *operator=(internal::tmp<diagMatrix> &&other) noexcept { return this->swap(*other.release()); };
    template <typename U> diagMatrix *operator=(internal::tmp<diagMatrix<U>> &&other) noexcept { return this->hold(*other.release(), operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator+=(internal::tmp<diagMatrix<U>> &&other) noexcept { return this->holdAdd(*this,*other.release(), operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator-=(internal::tmp<diagMatrix<U>> &&other) noexcept { return this->holdSub(*this,*other.release(), operators::MatrixCheckSize); };
    template <typename U> diagMatrix *operator*=(internal::tmp<diagMatrix<U>> &&other) noexcept { return this->holdMul(*this,*other.release(), operators::MatrixCheckSize); };

    // other functionalities
    T det() const;
    template <typename U> diagMatrix *holdInv(const diagMatrix<U> &other, const bool checkSize = true) { return this->holdInv(1, other, checkSize); };

};

namespace operators
{
    // dataType and diagMatrix
    template <typename T> internal::tmp<diagMatrix<T>> &&operator+(const T &a, const diagMatrix<T> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator-(const T &a, const diagMatrix<T> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(b.rows(), b.cols())->holdSub(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator*(const T &a, const diagMatrix<T> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(b.rows(), b.cols())->holdMul(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator/(const T &a, const diagMatrix<T> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(b.rows(), b.cols())->holdInv(a, b, operators::MatrixCheckSize)); };
    // diagMatrix and dataType
    template <typename T> internal::tmp<diagMatrix<T>> &&operator+(const diagMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator-(const diagMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator*(const diagMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator/(const diagMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)internal::tmp<diagMatrix<T>>::get(a.rows(), a.cols())->holdDiv(a, b, operators::MatrixCheckSize)); };
    // dataType and tmp diagMatrix
    template <typename T> internal::tmp<diagMatrix<T>> &&operator+(const T &a, internal::tmp<diagMatrix<T>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)b.holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator-(const T &a, internal::tmp<diagMatrix<T>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)b.holdSub(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator*(const T &a, internal::tmp<diagMatrix<T>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)b.holdMul(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator/(const T &a, internal::tmp<diagMatrix<T>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)b.holdInv(a, b, operators::MatrixCheckSize)); };
    // tmp diagMatrix and dataType
    template <typename T> internal::tmp<diagMatrix<T>> &&operator+(internal::tmp<diagMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator-(internal::tmp<diagMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator*(internal::tmp<diagMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<diagMatrix<T>> &&operator/(internal::tmp<diagMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdDiv(a, b, operators::MatrixCheckSize)); };
    // diagMatrix and diagMatrix
    template <typename T, typename U, typename V=decltype(T()+U())> internal::tmp<diagMatrix<V>> &&operator+(const diagMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<V>>*)internal::tmp<diagMatrix<V>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()-U())> internal::tmp<diagMatrix<V>> &&operator-(const diagMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<V>>*)internal::tmp<diagMatrix<V>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()*U())> internal::tmp<diagMatrix<V>> &&operator*(const diagMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<V>>*)internal::tmp<diagMatrix<V>>::get(a.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V=decltype(T()/U())> internal::tmp<diagMatrix<V>> &&operator/(const diagMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<V>>*)internal::tmp<diagMatrix<V>>::get(a.rows(), a.cols())->holdDiv(a, b, operators::MatrixCheckSize)); };
    // diagMatrix and tmp diagMatrix
    template <typename T, typename U> internal::tmp<diagMatrix<U>> &&operator+(const diagMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<U>>*)b.holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<U>> &&operator-(const diagMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<U>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<U>> &&operator*(const diagMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<U>>*)b.holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<U>> &&operator/(const diagMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<U>>*)b.holdInv(a, b, operators::MatrixCheckSize)); };
    // tmp diagMatrix and diagMatrix
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator+(internal::tmp<diagMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator-(internal::tmp<diagMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator*(internal::tmp<diagMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator/(internal::tmp<diagMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdDiv(a, b, operators::MatrixCheckSize)); };
    // tmp diagMatrix and tmp diagMatrix
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator+(internal::tmp<diagMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator-(internal::tmp<diagMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator*(internal::tmp<diagMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<diagMatrix<T>> &&operator/(internal::tmp<diagMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<diagMatrix<T>>*)a.holdDiv(a, *b.release(), operators::MatrixCheckSize)); };
}

#ifndef DIAG_MATRIX_CPP
#include "diagMatrix.cpp"
#endif

#endif // DIAG_MATRIX_HPP