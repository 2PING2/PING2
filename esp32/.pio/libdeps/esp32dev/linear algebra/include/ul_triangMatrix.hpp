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

#ifndef UL_TRIANG_MATRIX_HPP
#define UL_TRIANG_MATRIX_HPP

#include "matrixBase.hpp"

template <typename T>
class ul_triangMatrix : public MatrixBase<T>
{
    friend class internal::tmp<ul_triangMatrix>;
    protected:
        ul_triangMatrix *swap(ul_triangMatrix &other) noexcept { return (ul_triangMatrix *)MatrixBase<T>::swap(other);}
        const size_t minMemorySize(const size_t order) const noexcept { return ((order-1) * order) >> 1; }
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return minMemorySize(rows); }
        static const ul_triangMatrix<T> staticHelper;

    public:
        virtual T &operator()(const size_t row, const size_t col) override;
        const T &operator()(const size_t row, const size_t col) const override {return (row > col)? this->_begin[(((row-1) * row) >> 1) + col]: (row == col)? internal::_one<T>: internal::_zero<T>;};
        ul_triangMatrix() : MatrixBase<T>(){};
        ul_triangMatrix(const size_t order) : MatrixBase<T>(order, order){Vector<T>::resize(MatrixBase<T>::minMemorySize());};

        // ul_triangMatrix and dataTpe
        template <typename U> ul_triangMatrix *holdAdd(const ul_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template <typename U> ul_triangMatrix *holdSub(const ul_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template <typename U> ul_triangMatrix *holdMul(const ul_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
        template <typename U> ul_triangMatrix *holdDiv(const ul_triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdDiv(a, b, false); };
        // dataTpe and ul_triangMatrix
        template <typename U> ul_triangMatrix *holdSub(const T &a, const ul_triangMatrix<U> &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
        // ul_triangMatrix and ul_triangMatrix
        ul_triangMatrix *hold(const ul_triangMatrix &other, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> ul_triangMatrix *hold(const ul_triangMatrix<U> &other, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> ul_triangMatrix *holdAdd(const ul_triangMatrix<U> &a, const ul_triangMatrix<V> &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> ul_triangMatrix *holdSub(const ul_triangMatrix<U> &a, const ul_triangMatrix<V> &b, const bool checkSize = true){ return (ul_triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U, typename V> ul_triangMatrix *holdMul(const ul_triangMatrix<U> &a, const ul_triangMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        // ul_triangMatrix and u_ul_triangMatrix
        // ...

        template <typename U> ul_triangMatrix *holdInv(const ul_triangMatrix<U> &other, const bool checkSize = true);

        ////////////////////////// operators //////////////////////////
        // dataTpe
        ul_triangMatrix *operator+=(const T &data) { return (ul_triangMatrix *)this->Vector<T>::operator+=(data); };
        ul_triangMatrix *operator-=(const T &data) { return (ul_triangMatrix *)this->Vector<T>::operator-=(data); };
        ul_triangMatrix *operator*=(const T &data) { return (ul_triangMatrix *)this->Vector<T>::operator*=(data); };
        ul_triangMatrix *operator/=(const T &data) { return (ul_triangMatrix *)this->Vector<T>::operator/=(data); };
        // ul_triangMatrix
        ul_triangMatrix *operator=(const ul_triangMatrix &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator=(const ul_triangMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator+=(const ul_triangMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator-=(const ul_triangMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator*=(const ul_triangMatrix<U> &other)  { return this->swap(*internal::tmp<ul_triangMatrix>::get(this->_rows)->release()->holdMul(*this, other, operators::MatrixCheckSize, false)); };
        // tmp ul_triangMatrix
        ul_triangMatrix *operator=(internal::tmp<ul_triangMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template <typename U> ul_triangMatrix *operator=(internal::tmp<ul_triangMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator+=(internal::tmp<ul_triangMatrix<U>> &&other) { return this->holdAdd(*this,*other.release(), operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator-=(internal::tmp<ul_triangMatrix<U>> &&other) { return this->holdSub(*this,*other.release(), operators::MatrixCheckSize); };
        template <typename U> ul_triangMatrix *operator*=(internal::tmp<ul_triangMatrix<U>> &&other) { return this->swap(*internal::tmp<ul_triangMatrix>::get(this->_rows)->release()->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)); };
       
};

namespace operators
{
    // dataTpe and ul_triangMatrix
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator+(const T &a, const ul_triangMatrix<T> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(b.rows())->holdAdd(b, a, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator-(const T &a, const ul_triangMatrix<T> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(b.rows())->holdSub(a, b, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator*(const T &a, const ul_triangMatrix<T> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(b.rows())->holdMul(b, a, false)); };
    // ul_triangMatrix and dataTpe
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator+(const ul_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdAdd(a, b, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator-(const ul_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdSub(a, b, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator*(const ul_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdMul(a, b, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator/(const ul_triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdDiv(a, b, false)); };
    template <typename T> internal::tmp<ul_triangMatrix<T>> &&operator^(const ul_triangMatrix<T> &a, const T &b) { if (b != -1)throw "power not supported";return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdInv(a,false)); };
    // ul_triangMatrix and ul_triangMatrix
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator+(const ul_triangMatrix<T> &a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator-(const ul_triangMatrix<T> &a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator*(const ul_triangMatrix<T> &a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)internal::tmp<ul_triangMatrix<T>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    // ul_triangMatrix and tmp ul_triangMatrix
    template <typename T, typename U> internal::tmp<ul_triangMatrix<U>> &&operator+(const ul_triangMatrix<T> &a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<U>>*)b->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<ul_triangMatrix<U>> &&operator-(const ul_triangMatrix<T> &a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<U>>*)b->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<ul_triangMatrix<V>> &&operator*(const ul_triangMatrix<T> &a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<V>>*)internal::tmp<ul_triangMatrix<V>>::get(a.rows())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); }; 
    // tmp ul_triangMatrix and ul_triangMatrix
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator+(internal::tmp<ul_triangMatrix<T>> &&a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)a->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator-(internal::tmp<ul_triangMatrix<T>> &&a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)a->holdSub(b, a, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<ul_triangMatrix<V>> &&operator*(internal::tmp<ul_triangMatrix<T>> &&a, const ul_triangMatrix<U> &b) { return internal::move(*(internal::tmp<ul_triangMatrix<V>>*)internal::tmp<ul_triangMatrix<V>>::get(b.rows())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    // tmp ul_triangMatrix and tmp ul_triangMatrix
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator+(internal::tmp<ul_triangMatrix<T>> &&a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)a->holdAdd(*b.release(), a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<ul_triangMatrix<T>> &&operator-(internal::tmp<ul_triangMatrix<T>> &&a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<T>>*)a->holdSub(*b.release(), a, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<ul_triangMatrix<V>> &&operator*(internal::tmp<ul_triangMatrix<T>> &&a, internal::tmp<ul_triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<ul_triangMatrix<V>>*)internal::tmp<ul_triangMatrix<V>>::get(a->rows())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };
}

#ifndef UL_TRIANG_MATRIX_CPP
#include "ul_triangMatrix.cpp"
#endif

#endif
