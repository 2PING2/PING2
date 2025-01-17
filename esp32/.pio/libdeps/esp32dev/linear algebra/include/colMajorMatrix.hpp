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

#ifndef COL_MAJOR_MATRIX_HPP
#define COL_MAJOR_MATRIX_HPP

#include "rowMajorMatrix.hpp"

template <typename T>
class colMajorMatrix : public MatrixBase<T>
{
    friend class internal::tmp<colMajorMatrix>;

    protected:
        colMajorMatrix<T> *swap(colMajorMatrix<T> &other) noexcept { return (colMajorMatrix<T> *)this->MatrixBase<T>::swap(other); }
        colMajorMatrix<T> *refer(colMajorMatrix<T> &other) noexcept { return (colMajorMatrix<T> *)this->MatrixBase<T>::refer(other._begin, other._rows, other._cols); };
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return rows * cols; }
        const static colMajorMatrix<T> staticHelper;

    public:
        colMajorMatrix() : MatrixBase<T>(){};
        colMajorMatrix(const size_t rows, const size_t cols);
        colMajorMatrix(T *data, const size_t rows, const size_t cols, const bool share = true);
        colMajorMatrix(const colMajorMatrix &other) {this->hold(other);};
        template<typename U> colMajorMatrix(const colMajorMatrix<U> &other) {this->hold(other);};
        colMajorMatrix(internal::tmp<colMajorMatrix<T>> &&other) {this->swap(*other.release());};
        template<typename U> colMajorMatrix(internal::tmp<colMajorMatrix<U>> &&other) {this->hold(*other.release());};
        template<typename U> colMajorMatrix(const rowMajorMatrix<U> &other) {this->hold(other);};
        template<typename U> colMajorMatrix(internal::tmp<rowMajorMatrix<U>> &&other) {this->hold(*other.release());};

        virtual T &operator()(const size_t row, const size_t col) override { return this->_begin[col * this->_rows + row];};  
        
        // colMajorMatrix and dataType
        // colMajorMatrix<T> *hold(const T *data){ return (colMajorMatrix<T> *)this->Vector<T>::hold(data, MatrixBase<T>::minMemorySize()); };
        template<typename U> colMajorMatrix<T> *holdAdd(const colMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U> colMajorMatrix<T> *holdSub(const colMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> colMajorMatrix<T> *holdSub(const T &a, const colMajorMatrix<U> &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U> colMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const T &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
        
        // colMajorMatrix and colMajorMatrix
        colMajorMatrix *hold(const colMajorMatrix &other, const bool checkSize = true){ return (colMajorMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> colMajorMatrix<T> *hold(const colMajorMatrix<U> &other, const bool checkSize = true){ return (colMajorMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> colMajorMatrix<T> *holdAdd(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::checkSize_add(a,b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> colMajorMatrix<T> *holdSub(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true){ return (colMajorMatrix<T> *)(checkSize? MatrixBase<T>::checkSize_add(a,b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U, typename V> colMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        

        // rowMajorMatrix and rowMajorMatrix
        template<typename U> colMajorMatrix<T> *hold(const rowMajorMatrix<U> &other, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdAdd(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);

        // colMajorMatrix and rowMajorMatrix
        template<typename U, typename V> colMajorMatrix<T> *holdAdd(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdSub(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdSub(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true);
        template<typename U, typename V> colMajorMatrix<T> *holdMul(const colMajorMatrix<U> &a, const rowMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        template<typename U, typename V> colMajorMatrix<T> *holdMul(const rowMajorMatrix<U> &a, const colMajorMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);


        ////////////////////////// operators //////////////////////////
        // dataType
        // template<typename U> colMajorMatrix<T> *operator=(const U &data) { return (colMajorMatrix<T> *)this->Vector<T>::operator=(data); };
        colMajorMatrix<T> *operator+=(const T &data) { return (colMajorMatrix *)this->Vector<T>::operator+=(data); };
        colMajorMatrix<T> *operator-=(const T &data) { return (colMajorMatrix *)this->Vector<T>::operator-=(data); };
        colMajorMatrix<T> *operator*=(const T &data) { return (colMajorMatrix *)this->Vector<T>::operator*=(data); };
        colMajorMatrix<T> *operator/=(const T &data) { return (colMajorMatrix *)this->Vector<T>::operator/=(data); };

        // colMajorMatrix
        colMajorMatrix<T> *operator=(const colMajorMatrix<T> &other) { return this->hold(other); };
        template<typename U> colMajorMatrix<T> *operator=(const colMajorMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator+=(const colMajorMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator-=(const colMajorMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator*=(const colMajorMatrix<U> &other) { return this->swap(*((internal::tmp<colMajorMatrix<T>> *)(internal::tmp<colMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, other, operators::MatrixCheckSize)))->release()); };

        // tmp colMajorMatrix
        colMajorMatrix *operator=(internal::tmp<colMajorMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template<typename U> colMajorMatrix<T> *operator=(internal::tmp<colMajorMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator+=(internal::tmp<colMajorMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator-=(internal::tmp<colMajorMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator*=(internal::tmp<colMajorMatrix<U>> &&other) { return this->swap(*((internal::tmp<colMajorMatrix<T>> *)(internal::tmp<colMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, *other.release(), operators::MatrixCheckSize)))->release()); };

        // rowMajorMatrix
        template<typename U> colMajorMatrix<T> *operator=(const rowMajorMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator+=(const rowMajorMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator-=(const rowMajorMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator*=(const rowMajorMatrix<U> &other) { return this->hold(*(internal::tmp<colMajorMatrix<T>> *)internal::tmp<colMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, other, operators::MatrixCheckSize)); };

        // tmp rowMajorMatrix
        template<typename U> colMajorMatrix<T> *operator=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator+=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdAdd(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator-=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->holdSub(*this, *other.release(), operators::MatrixCheckSize); };
        template<typename U> colMajorMatrix<T> *operator*=(internal::tmp<rowMajorMatrix<U>> &&other) { return this->swap(*((internal::tmp<colMajorMatrix<T>> *)(internal::tmp<colMajorMatrix<T>>::get(this->_rows, other._cols)->holdMul(*this, *other.release(), operators::MatrixCheckSize)))->release()); };

};

namespace operators
{    
    // dataType and colMajorMatrix
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator+(const T &a, const colMajorMatrix<T> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(b.rows(), b.cols())->Vector<T>::holdAdd(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator-(const T &a, const colMajorMatrix<T> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(b.rows(), b.cols())->Vector<T>::holdSub(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator*(const T &a, const colMajorMatrix<T> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(b.rows(), b.cols())->Vector<T>::holdMul(b, a, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator+(const colMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(b.rows(), b.cols())->Vector<T>::holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator-(const colMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(b.rows(), b.cols())->Vector<T>::holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T> internal::tmp<colMajorMatrix<T>> &&operator*(const colMajorMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)internal::tmp<colMajorMatrix<T>>::get(a.rows(), a.cols())->Vector<T>::holdMul(a, b, operators::MatrixCheckSize)); };
    // colMajorMatrix and colMajorMatrix
    template<typename T, typename U, typename V = decltype(T() + U())> internal::tmp<colMajorMatrix<V>> &&operator+(const colMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<V>>*)internal::tmp<colMajorMatrix<V>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() - U())> internal::tmp<colMajorMatrix<V>> &&operator-(const colMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<V>>*)internal::tmp<colMajorMatrix<V>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<colMajorMatrix<V>> &&operator*(const colMajorMatrix<T> &a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<V>>*)internal::tmp<colMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };
    // dataType and tmp colMajorMatrix
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator+(const T &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)b.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator+(internal::tmp<colMajorMatrix<U>> &&a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator-(const T &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator-(internal::tmp<colMajorMatrix<U>> &&a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator*(const T &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)b.holdMul(b, a, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator*(internal::tmp<colMajorMatrix<U>> &&a, const T &b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)a.holdMul(a, b, operators::MatrixCheckSize)); };
    // tmp colMajorMatrix and colMajorMatrix
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator+(internal::tmp<colMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator+(const colMajorMatrix<T> &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)b.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator-(internal::tmp<colMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<U>> &&operator-(const colMajorMatrix<T> &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<U>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<colMajorMatrix<V>> &&operator*(internal::tmp<colMajorMatrix<T>> &&a, const colMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<V>>*)internal::tmp<colMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<colMajorMatrix<V>> &&operator*(const colMajorMatrix<T> &a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<V>> *)internal::tmp<colMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    // tmp colMajorMatrix and tmp colMajorMatrix
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator+(internal::tmp<colMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdAdd(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator-(internal::tmp<colMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdSub(a, *b.release(), operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V = decltype(T() * U())> internal::tmp<colMajorMatrix<V>> &&operator*(internal::tmp<colMajorMatrix<T>> &&a, internal::tmp<colMajorMatrix<U>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<V>>*)internal::tmp<colMajorMatrix<V>>::get(a.rows(), b.cols())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize)); };

    // tmp colMajorMatrix and rowMajorMatrix
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator+(internal::tmp<colMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator-(internal::tmp<colMajorMatrix<T>> &&a, const rowMajorMatrix<U> &b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)a.holdSub(a, b, operators::MatrixCheckSize)); };
    // rowMajorMatrix and tmp colMajorMatrix
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator+(const rowMajorMatrix<U> &a, internal::tmp<colMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)b.holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U> internal::tmp<colMajorMatrix<T>> &&operator-(const rowMajorMatrix<U> &a, internal::tmp<colMajorMatrix<T>> &&b) { return internal::move(*(internal::tmp<colMajorMatrix<T>>*)b.holdSub(a, b, operators::MatrixCheckSize)); };
}

#ifndef COL_MAJOR_MATRIX_CPP
#include "colMajorMatrix.cpp"
#endif // COL_MAJOR_MATRIX_CPP

#endif // COL_MAJOR_MATRIX_HPP