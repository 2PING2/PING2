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

#ifndef ROW_MAJOR_MATRIX_HPP
#define ROW_MAJOR_MATRIX_HPP

#include "matrixBase.hpp"
#include "colMajorMatrix.hpp"
#include "diagMatrix.hpp"
#include "symMatrix.hpp"

template <typename T>
class rowMajorMatrix : public MatrixBase<T>
{
    friend class internal::tmp<rowMajorMatrix>;
    friend class internal::tmp<Matrix<T>>;


    protected:
        rowMajorMatrix<T> *swap(rowMajorMatrix<T> &other) noexcept { return (rowMajorMatrix<T> *)this->MatrixBase<T>::swap(other); }
        rowMajorMatrix<T> *refer(rowMajorMatrix<T> &other) noexcept { return (rowMajorMatrix<T> *)this->MatrixBase<T>::refer(other._begin, other._rows, other._cols); };
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return rows * cols; }
        static const rowMajorMatrix<T> staticHelper;

    public:
        rowMajorMatrix() : MatrixBase<T>() {};
        rowMajorMatrix(const size_t rows, const size_t cols);
        rowMajorMatrix(T *data, const size_t rows, const size_t cols, const bool share= true);
        rowMajorMatrix(const rowMajorMatrix &other) {this->hold(other);};
        template<typename U> rowMajorMatrix(const rowMajorMatrix<U> &other) {this->hold(other);};
        rowMajorMatrix(internal::tmp<rowMajorMatrix<T>> &&other) noexcept {this->swap(*other.release());};
        template<typename U> rowMajorMatrix(internal::tmp<rowMajorMatrix<U>> &&other) noexcept {this->hold(*other.release());};
        template<typename U> rowMajorMatrix(const colMajorMatrix<U> &other) {this->hold(other);};
        template<typename U> rowMajorMatrix(const internal::tmp<colMajorMatrix<U>> &other) {this->hold(*other.release());};
        
        virtual T &operator()(const size_t row, const size_t col) override { return this->_begin[row * this->_cols + col]; };
        const T &operator()(const size_t row, const size_t col) const override { return this->_begin[row * this->_cols + col]; };
        // rowMajorMatrix and dataType
        // rowMajorMatrix<T> *hold(const T *data){ return (rowMajorMatrix<T> *)this->Vector<T>::hold(data, MatrixBase<T>::minMemorySize()); };
        template<typename U> rowMajorMatrix<T> *holdAdd(const rowMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U> rowMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> rowMajorMatrix<T> *holdSub(const T &a, const rowMajorMatrix<U> &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> rowMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
        
        // rowMajorMatrix and rowMajorMatrix
        rowMajorMatrix *hold(const rowMajorMatrix &other, const bool checkSize = true) { return (rowMajorMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> rowMajorMatrix<T> *hold(const rowMajorMatrix<U> &other, const bool checkSize = true){ return (rowMajorMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> rowMajorMatrix<T> *holdAdd(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::checkSize_add(a,b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true){ return (rowMajorMatrix<T> *)(checkSize? MatrixBase<T>::checkSize_sub(a,b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        
        // colMajorMatrix and colMajorMatrix
        template<typename U> rowMajorMatrix<T> *hold(const colMajorMatrix<U> &other, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdAdd(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);

        // rowMajorMatrix and colMajorMatrix
        template<typename U, typename V> rowMajorMatrix<T> *holdAdd(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);

        // rowMajorMatrix and diagMatrix
        template<typename U> rowMajorMatrix<T> *hold(const diagMatrix<U> &other, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdAdd(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdDiv(const rowMajorMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true);
        // diagMatrix and rowMajorMatrix
        template<typename U, typename V> rowMajorMatrix<T> *holdSub(const diagMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const diagMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        // rowMajorMatrix and symMatrix
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true);
        // colMajorMatrix and symMatrix
        template<typename U, typename V> rowMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const symMatrix<V> &b, const bool checkSize = true);
        ////////////////////////// operators //////////////////////////
        // dataType
        // template<typename U> rowMajorMatrix<T> *operator=(const U &data) { return (rowMajorMatrix<T> *)this->Vector<T>::operator=(data); };
        rowMajorMatrix *operator+=(const T &data) { return (rowMajorMatrix *)this->Vector<T>::operator+=(data); };
        rowMajorMatrix *operator-=(const T &data) { return (rowMajorMatrix *)this->Vector<T>::operator-=(data); };
        rowMajorMatrix *operator*=(const T &data) { return (rowMajorMatrix *)this->Vector<T>::operator*=(data); };
        rowMajorMatrix *operator/=(const T &data) { return (rowMajorMatrix *)this->Vector<T>::operator/=(data); };

        // rowMajorMatrix
        rowMajorMatrix<T> *operator=(const rowMajorMatrix<T> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator=(const rowMajorMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator+=(const rowMajorMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator-=(const rowMajorMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator*=(const rowMajorMatrix<U> &other) { return this->swap(*((internal::tmp<rowMajorMatrix<T>> *)(internal::tmp<rowMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, other, operators::MatrixCheckSize, false)))->release()); };

        // tmp rowMajorMatrix
        rowMajorMatrix *operator=(internal::tmp<rowMajorMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template<typename U> rowMajorMatrix<T> *operator=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator+=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator-=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator*=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->swap(*((internal::tmp<rowMajorMatrix<T>> *)(internal::tmp<rowMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)))->release()); };

        // colMajorMatrix
        template<typename U> rowMajorMatrix<T> *operator=(const colMajorMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator+=(const colMajorMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator-=(const colMajorMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator*=(const colMajorMatrix<U> &other) { return this->swap(*((internal::tmp<rowMajorMatrix<T>> *)(internal::tmp<rowMajorMatrix<T>>::get(this->_rows, other.cols())->holdMul(*this, other, operators::MatrixCheckSize, false)))->release()); };

        // tmp colMajorMatrix
        template<typename U> rowMajorMatrix<T> *operator=(internal::tmp<colMajorMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator+=(internal::tmp<colMajorMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator-=(internal::tmp<colMajorMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator*=(internal::tmp<colMajorMatrix<U>> &&other) { return this->swap(*((internal::tmp<rowMajorMatrix<T>> *)(internal::tmp<rowMajorMatrix<T>>::get(this->_rows, other.cols())->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)))->release()); };

        // diagMatrix
        template<typename U> rowMajorMatrix<T> *operator=(const diagMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator+=(const diagMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator-=(const diagMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator*=(const diagMatrix<U> &other) { return this->holdMul(*this, other, operators::MatrixCheckSize); };
        template<typename U> rowMajorMatrix<T> *operator/=(const diagMatrix<U> &other) { return this->holdDiv(*this, other, operators::MatrixCheckSize); };

};

namespace operators
{    
    // dataType and rowMajorMatrix
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator+(const T &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator+(const rowMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator-(const T &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator-(const rowMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator*(const T &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdMul(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator*(const rowMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    // dataType and tmp rowMajorMatrix
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator+(const T &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator-(const T &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator*(const T &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdMul(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<rowMajorMatrix<T>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, const T &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdMul(a, b, operators::MatrixCheckSize)); };
    // rowMajorMatrix and rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const rowMajorMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const rowMajorMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const rowMajorMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    
    // tmp rowMajorMatrix and rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<U>> &&operator+(const rowMajorMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<U>>*)b.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<U>> &&operator-(const rowMajorMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<U>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const rowMajorMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>> *)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); };
    // tmp rowMajorMatrix and tmp rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };

    // rowMajorMatrix and colMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const rowMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const colMajorMatrix<U> &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const rowMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const colMajorMatrix<U> &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const rowMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const colMajorMatrix<U> &a, const rowMajorMatrix<T> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(b.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    // tmp rowMajorMatrix and colMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const colMajorMatrix<U> &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const colMajorMatrix<U> &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const colMajorMatrix<U> &a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(b.rows(), a.cols())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); };
    // tmp colMajorMatrix and rowMajorMatrix
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<colMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const rowMajorMatrix<U> &a, internal::tmp<colMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); };
    // tmp rowMajorMatrix and tmp colMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<colMajorMatrix<U>> &&a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdAdd(b, *a.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<colMajorMatrix<U>> &&a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdSub(*a.release(), b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(internal::tmp<colMajorMatrix<U>> &&a, internal::tmp<rowMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };

    // rowMajorMatrix and diagMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const rowMajorMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const rowMajorMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(const rowMajorMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator/(const rowMajorMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdDiv(a, b, operators::MatrixCheckSize)); };
    // diagMatrix and rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const diagMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const diagMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdSub(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(const diagMatrix<T> &a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    // tmp rowMajorMatrix and diagMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdMul(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator/(internal::tmp<rowMajorMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdDiv(a, b, operators::MatrixCheckSize)); };
    // diagMatrix and tmp rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const diagMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const diagMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(const diagMatrix<T> &a, internal::tmp<rowMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)b.holdMul(a, b, operators::MatrixCheckSize)); };
    // tmp rowMajorMatrix and tmp diagMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator/(internal::tmp<rowMajorMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)a.holdDiv(a, *b.release(), operators::MatrixCheckSize)); };
    // tmp diagMatrix and rowMajorMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(internal::tmp<diagMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdAdd(b, *a.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(internal::tmp<diagMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdSub(b, *a.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(internal::tmp<diagMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(b.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize)); };
    // rowMajorMatrix and tmp diagMatrix
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator+(const rowMajorMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator-(const rowMajorMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<rowMajorMatrix<T>> &&operator*(const rowMajorMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    // rowMajorMatrix and symMatrix
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const rowMajorMatrix<T> &a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    //colMajorMatrix and symMatrix
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<rowMajorMatrix<V>> &&operator*(const colMajorMatrix<T> &a, const symMatrix<U> &b) { return internal::move(*(internal::tmp<rowMajorMatrix<V>>*)internal::tmp<rowMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
}

#ifndef ROW_MAJOR_MATRIX_CPP
#include "rowMajorMatrix.cpp"
#endif // ROW_MAJOR_MATRIX_CPP

#endif // ROW_MAJOR_MATRIX_HPP