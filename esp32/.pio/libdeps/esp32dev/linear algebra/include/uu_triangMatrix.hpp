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

#ifndef UU_TRIANG_MATRIX_HPP
#define UU_TRIANG_MATRIX_HPP

#include "matrixBase.hpp"

template <typename T>
class uu_triangMatrix : public MatrixBase<T>
{
    friend class internal::tmp<uu_triangMatrix>;
    protected:
        uu_triangMatrix *swap(uu_triangMatrix &other) noexcept { return (uu_triangMatrix *)MatrixBase<T>::swap(other);}
        const size_t minMemorySize(const size_t order) const noexcept { return ((order-1) * order) >> 1; }
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return minMemorySize(rows); }
        static const uu_triangMatrix<T> staticHelper;

    public:
        virtual T &operator()(const size_t row, const size_t col) override;
        const T &operator()(const size_t row, const size_t col) const override {return (col > row)? this->_begin[(((col-1) * col) >> 1) + row]: (row == col)? internal::_one<T>: internal::_zero<T>;};
        uu_triangMatrix() : MatrixBase<T>(){};
        uu_triangMatrix(const size_t order) : MatrixBase<T>(order, order){Vector<T>::resize(MatrixBase<T>::minMemorySize());};

        // uu_triangMatrix and dataTpe
        template <typename U> uu_triangMatrix *holdAdd(const uu_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template <typename U> uu_triangMatrix *holdSub(const uu_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template <typename U> uu_triangMatrix *holdMul(const uu_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
        template <typename U> uu_triangMatrix *holdDiv(const uu_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdDiv(a, b, false); };
        // dataTpe and uu_triangMatrix
        template <typename U> uu_triangMatrix *holdSub(const T &a, const uu_triangMatrix<U> &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
        // uu_triangMatrix and uu_triangMatrix
        uu_triangMatrix *hold(const uu_triangMatrix &other, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> uu_triangMatrix *hold(const uu_triangMatrix<U> &other, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> uu_triangMatrix *holdAdd(const uu_triangMatrix<U> &a, const uu_triangMatrix<V> &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> uu_triangMatrix *holdSub(const uu_triangMatrix<U> &a, const uu_triangMatrix<V> &b, const bool checkSize = true){ return (uu_triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdSub(a, b, false); };
        // uu_triangMatrix and u_uu_triangMatrix
        // ...

        ////////////////////////// operators //////////////////////////
        // dataTpe
        uu_triangMatrix *operator+=(const T &data) { return (uu_triangMatrix *)this->Vector<T>::operator+=(data); };
        uu_triangMatrix *operator-=(const T &data) { return (uu_triangMatrix *)this->Vector<T>::operator-=(data); };
        uu_triangMatrix *operator*=(const T &data) { return (uu_triangMatrix *)this->Vector<T>::operator*=(data); };
        uu_triangMatrix *operator/=(const T &data) { return (uu_triangMatrix *)this->Vector<T>::operator/=(data); };
        // uu_triangMatrix
        uu_triangMatrix *operator=(const uu_triangMatrix &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> uu_triangMatrix *operator=(const uu_triangMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> uu_triangMatrix *operator+=(const uu_triangMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template <typename U> uu_triangMatrix *operator-=(const uu_triangMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        // tmp uu_triangMatrix
        uu_triangMatrix *operator=(internal::tmp<uu_triangMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template <typename U> uu_triangMatrix *operator=(internal::tmp<uu_triangMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template <typename U> uu_triangMatrix *operator+=(internal::tmp<uu_triangMatrix<U>> &&other) { return this->holdAdd(*this,*other.release(), operators::MatrixCheckSize); };
        template <typename U> uu_triangMatrix *operator-=(internal::tmp<uu_triangMatrix<U>> &&other) { return this->holdSub(*this,*other.release(), operators::MatrixCheckSize); };
       
};

namespace operators
{
    // dataTpe and uu_triangMatrix
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator+(const T &a, const uu_triangMatrix<T> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(b.rows())->holdAdd(b, a, false)); };
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator-(const T &a, const uu_triangMatrix<T> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(b.rows())->holdSub(a, b, false)); };
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator*(const T &a, const uu_triangMatrix<T> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(b.rows())->holdMul(b, a, false)); };
    // uu_triangMatrix and dataTpe
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator+(const uu_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdAdd(a, b, false)); };
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator-(const uu_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdSub(a, b, false)); };
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator*(const uu_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdMul(a, b, false)); };
    template <typename T> internal::tmp<uu_triangMatrix<T>> &&operator/(const uu_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdDiv(a, b, false)); };
    // uu_triangMatrix and uu_triangMatrix
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator+(const uu_triangMatrix<T> &a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator-(const uu_triangMatrix<T> &a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)internal::tmp<uu_triangMatrix<T>>::get(a.rows())->holdSub(a, b, operators::MatrixCheckSize)); };
    // uu_triangMatrix and tmp uu_triangMatrix
    template <typename T, typename U> internal::tmp<uu_triangMatrix<U>> &&operator+(const uu_triangMatrix<T> &a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<uu_triangMatrix<U>>*)b->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<uu_triangMatrix<U>> &&operator-(const uu_triangMatrix<T> &a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<uu_triangMatrix<U>>*)b->holdSub(a, b, operators::MatrixCheckSize)); };
    // tmp uu_triangMatrix and uu_triangMatrix
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator+(internal::tmp<uu_triangMatrix<T>> &&a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)a->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator-(internal::tmp<uu_triangMatrix<T>> &&a, const uu_triangMatrix<U> &b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)a->holdSub(b, a, operators::MatrixCheckSize)); };
    // tmp uu_triangMatrix and tmp uu_triangMatrix
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator+(internal::tmp<uu_triangMatrix<T>> &&a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)a->holdAdd(*b.release(), a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<uu_triangMatrix<T>> &&operator-(internal::tmp<uu_triangMatrix<T>> &&a, internal::tmp<uu_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<uu_triangMatrix<T>>*)a->holdSub(*b.release(), a, operators::MatrixCheckSize)); };
}

#ifndef UU_TRIANG_MATRIX_CPP
#include "uu_triangMatrix.cpp"
#endif

#endif
