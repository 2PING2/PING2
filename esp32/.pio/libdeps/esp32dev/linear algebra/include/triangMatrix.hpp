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

#ifndef TRIANG_MATRIX_HPP
#define TRIANG_MATRIX_HPP

#include "matrixBase.hpp"

template <typename T>
class triangMatrix : public MatrixBase<T>
{
    friend class internal::tmp<triangMatrix>;
    protected:
        triangMatrix *swap(triangMatrix &other) noexcept { return (triangMatrix *)MatrixBase<T>::swap(other);}
        const size_t minMemorySize(const size_t order) const noexcept { return (order * (order + 1)) >> 1; }
        virtual const size_t minMemorySize(const size_t rows, const size_t cols) const noexcept override { return minMemorySize(rows); }
        static const triangMatrix<T> staticHelper;

    public:
        virtual T &operator()(const size_t row, const size_t col) override;
        const T &operator()(const size_t row, const size_t col) const override{ return (row >= col) ? this->_begin[((row * (row + 1)) >> 1) + col] : internal::_zero<T>; };
        triangMatrix() : MatrixBase<T>(){};
        triangMatrix(const size_t order) : MatrixBase<T>(order, order){Vector<T>::resize(MatrixBase<T>::minMemorySize());};

        // triangMatrix and dataTpe
        template <typename U> triangMatrix *holdAdd(const triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdAdd(a, b, false); };
        template <typename U> triangMatrix *holdSub(const triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdSub(a, b, false); };
        template <typename U> triangMatrix *holdMul(const triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdMul(a, b, false); };
        template <typename U> triangMatrix *holdDiv(const triangMatrix<U> &a, const T &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(a):this)->Vector<T>::holdDiv(a, b, false); };
        // dataTpe and triangMatrix
        template <typename U> triangMatrix *holdSub(const T &a, const triangMatrix<U> &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(b):this)->Vector<T>::holdSub(a, b, false); };
        // triangMatrix and triangMatrix
        triangMatrix *hold(const triangMatrix &other, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U> triangMatrix *hold(const triangMatrix<U> &other, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::resizeLike(other):this)->Vector<T>::hold(other, false); };
        template<typename U, typename V> triangMatrix *holdAdd(const triangMatrix<U> &a, const triangMatrix<V> &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdAdd(a, b, false); };
        template<typename U, typename V> triangMatrix *holdSub(const triangMatrix<U> &a, const triangMatrix<V> &b, const bool checkSize = true){ return (triangMatrix *)(checkSize? MatrixBase<T>::checkSize_add(a, b):this)->Vector<T>::holdSub(a, b, false); };
        template<typename U, typename V> triangMatrix *holdMul(const triangMatrix<U> &a, const triangMatrix<V> &b, const bool checkSize = true, const bool checkOverlap = true);
        // ul_triangMatrix and diagMatrix
        template<typename U, typename V> triangMatrix *holdMul(const ul_triangMatrix<U> &a, const diagMatrix<V> &b, const bool checkSize = true);
        // ...

        ////////////////////////// operators //////////////////////////
        // dataTpe
        triangMatrix *operator+=(const T &data) { return (triangMatrix *)this->Vector<T>::operator+=(data); };
        triangMatrix *operator-=(const T &data) { return (triangMatrix *)this->Vector<T>::operator-=(data); };
        triangMatrix *operator*=(const T &data) { return (triangMatrix *)this->Vector<T>::operator*=(data); };
        triangMatrix *operator/=(const T &data) { return (triangMatrix *)this->Vector<T>::operator/=(data); };
        // triangMatrix
        triangMatrix *operator=(const triangMatrix &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator=(const triangMatrix<U> &other) { return this->hold(other, operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator+=(const triangMatrix<U> &other) { return this->holdAdd(*this, other, operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator-=(const triangMatrix<U> &other) { return this->holdSub(*this, other, operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator*=(const triangMatrix<U> &other)  { return this->swap(*internal::tmp<triangMatrix>::get(this->_rows)->release()->holdMul(*this, other, operators::MatrixCheckSize, false)); };
        // tmp triangMatrix
        triangMatrix *operator=(internal::tmp<triangMatrix> &&other) noexcept { return this->swap(*other.release()); };
        template <typename U> triangMatrix *operator=(internal::tmp<triangMatrix<U>> &&other) { return this->hold(*other.release(), operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator+=(internal::tmp<triangMatrix<U>> &&other) { return this->holdAdd(*this,*other.release(), operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator-=(internal::tmp<triangMatrix<U>> &&other) { return this->holdSub(*this,*other.release(), operators::MatrixCheckSize); };
        template <typename U> triangMatrix *operator*=(internal::tmp<triangMatrix<U>> &&other) { return this->swap(*internal::tmp<triangMatrix>::get(this->_rows)->release()->holdMul(*this, *other.release(), operators::MatrixCheckSize, false)); };
       
};

namespace operators
{
    // dataTpe and triangMatrix
    template <typename T> internal::tmp<triangMatrix<T>> &&operator+(const T &a, const triangMatrix<T> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(b.rows())->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<triangMatrix<T>> &&operator-(const T &a, const triangMatrix<T> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(b.rows())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<triangMatrix<T>> &&operator*(const T &a, const triangMatrix<T> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(b.rows())->holdMul(b, a, operators::MatrixCheckSize)); };
    // triangMatrix and dataTpe
    template <typename T> internal::tmp<triangMatrix<T>> &&operator+(const triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<triangMatrix<T>> &&operator-(const triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<triangMatrix<T>> &&operator*(const triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize)); };
    template <typename T> internal::tmp<triangMatrix<T>> &&operator/(const triangMatrix<T> &a, const T &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdDiv(a, b, operators::MatrixCheckSize)); };
    // triangMatrix and triangMatrix
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator+(const triangMatrix<T> &a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator-(const triangMatrix<T> &a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator*(const triangMatrix<T> &a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)internal::tmp<triangMatrix<T>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize, false)); };
    // triangMatrix and tmp triangMatrix
    template <typename T, typename U> internal::tmp<triangMatrix<U>> &&operator+(const triangMatrix<T> &a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<U>>*)b->holdAdd(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<triangMatrix<U>> &&operator-(const triangMatrix<T> &a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<U>>*)b->holdSub(a, b, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(const triangMatrix<T> &a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a.rows())->holdMul(a, *b.release(), operators::MatrixCheckSize, false)); }; 
    // tmp triangMatrix and triangMatrix
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator+(internal::tmp<triangMatrix<T>> &&a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)a->holdAdd(b, a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator-(internal::tmp<triangMatrix<T>> &&a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)a->holdSub(b, a, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(internal::tmp<triangMatrix<T>> &&a, const triangMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(b.rows())->holdMul(*a.release(), b, operators::MatrixCheckSize, false)); };
    // tmp triangMatrix and tmp triangMatrix
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator+(internal::tmp<triangMatrix<T>> &&a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)a->holdAdd(*b.release(), a, operators::MatrixCheckSize)); };
    template <typename T, typename U> internal::tmp<triangMatrix<T>> &&operator-(internal::tmp<triangMatrix<T>> &&a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<T>>*)a->holdSub(*b.release(), a, operators::MatrixCheckSize)); };
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(internal::tmp<triangMatrix<T>> &&a, internal::tmp<triangMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a->rows())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize, false)); };
    // ul_triangMatrix and diagMatrix
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(const ul_triangMatrix<T> &a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize)); };
    // tmp ul_triangMatrix and diagMatrix
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(internal::tmp<ul_triangMatrix<T>> &&a, const diagMatrix<U> &b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a->rows())->holdMul(*a.release(), b, operators::MatrixCheckSize)); };
    // ul_triangMatrix and tmp diagMatrix
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(const ul_triangMatrix<T> &a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a.rows())->holdMul(a, *b.release(), operators::MatrixCheckSize)); };
    // tmp ul_triangMatrix and tmp diagMatrix
    template <typename T, typename U, typename V = decltype(T()*U())> internal::tmp<triangMatrix<V>> &&operator*(internal::tmp<ul_triangMatrix<T>> &&a, internal::tmp<diagMatrix<U>> &&b) { return internal::move(*(internal::tmp<triangMatrix<V>>*)internal::tmp<triangMatrix<V>>::get(a->rows())->holdMul(*a.release(), *b.release(), operators::MatrixCheckSize)); };
}   

#ifndef TRIANG_MATRIX_CPP
#include "triangMatrix.cpp"
#endif

#endif
