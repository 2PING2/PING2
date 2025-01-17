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

#include "matrixBase.hpp"

// #include "rowMajorMatrix.hpp"
#ifndef VECTOR_HPP
#define VECTOR_HPP

template <typename T>
class Vector
{
    template <typename U> friend class Vector;
    friend class internal::tmp<Vector>;
protected:
    T *_begin = nullptr;
    T *_end = nullptr;
    T *_endOfStorage = nullptr;
    
public:
    Vector() noexcept {internal::vec_count++; }
    Vector(const size_t N);
    Vector(internal::tmp<Vector> &&v) noexcept : Vector()  {swap(*v.release()); }
    template <typename U> Vector(internal::tmp<Vector<U>> &&v): Vector() { hold(*v.release()); }
    Vector(const Vector &v, const bool share = false);
    Vector(T *begin, const size_t N, const bool share = false);
    ~Vector();
    const size_t size() const noexcept { return _end - _begin; }
    const size_t capacity() const noexcept { return shared() ? 0 : _endOfStorage - _begin; }
    bool shared() const noexcept { return _endOfStorage == nullptr && _begin != nullptr; }
    Vector *resize(const size_t N, const bool deallocateIfPossible = true, const bool saveData = true);

    const size_t findFirst(const T &value) const;
    const size_t findLast(const T &value) const;
    Vector<size_t> *findAll(const T &value, Vector<size_t> &indices_holder) const;
    Vector *removeFirst(const T &value);
    Vector *removeLast(const T &value);
    Vector *removeAt(const size_t index);
    Vector *insert(const T &value, const size_t index);
    Vector *push_back(const T &value);
    Vector *pop_back();
    Vector *sort(const bool ascending = true);
    template<typename U, typename V = decltype(T()*U())> V dot(const Vector<U> &other, const bool checkSize = true);
    Vector *fill(const T val);
    Vector *hold(T *begin, const size_t N, const bool checkSize = true);

    Vector *hold(const Vector &v, const bool checkSize = true);
    // matrixBase and vector
    template<typename U, typename V> Vector *holdMul(const MatrixBase<U> &a, const Vector<V> &b, const bool checkSize = true);
    // rowMajorMatrix and vector
    template<typename U, typename V> Vector *holdMul(const rowMajorMatrix<U> &a, const Vector<V> &b, const bool checkSize = true);
    template<typename U, typename V> Vector *addMul(const rowMajorMatrix<U> &a, const Vector<V> &b, const bool checkSize = true);
    template <typename U> Vector *hold(const Vector<U> &v, const bool checkSize = true);
    // symMatrix and vector
    // template<typename U, typename V> Vector *holdMul(const symMatrix<U> &a, const Vector<V> &b, const bool checkSize = true);
    template <typename U, typename V> Vector *holdAdd(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize = true);
    template <typename U, typename V> Vector *holdSub(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize = true);
    template <typename U, typename V> Vector *holdMul(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize = true);
    template <typename U, typename V> Vector *holdDiv(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize = true);

    template <typename U> Vector *holdAdd(const Vector<U> &v1, const T val, const bool checkSize = true);
    template <typename U> Vector *holdSub(const Vector<U> &v1, const T val, const bool checkSize = true);
    template <typename U, typename V> Vector *holdMul(const Vector<U> &v1, const V val, const bool checkSize = true);
    template <typename U> Vector *holdSub(const T val, const Vector<U> &v, const bool checkSize = true);
    template <typename U> Vector *holdDiv(const T val, const Vector<U> &v, const bool checkSize = true);



    T &operator[](const size_t i) { return _begin[i]; }
    const T operator[](size_t i) const { return _begin[i]; }
    T *begin() const { return _begin; }
    T *end() const { return _end; }

    template <typename U> const bool operator==(const Vector<U> &v) const;
    template <typename U> const bool operator!=(const Vector<U> &v) const { return !(*this == v); }
    Vector *operator=(const Vector &v) { return hold(v); }
    template <typename U> Vector *operator=(const Vector<U> &v) { return hold(v, operators::VectorCheckSize); }
    template <typename U> Vector *operator+=(const Vector<U> &v) { return holdAdd(*this, v, operators::VectorCheckSize); }
    template <typename U> Vector *operator-=(const Vector<U> &v) { return holdSub(*this, v, operators::VectorCheckSize); }
    template <typename U> Vector *operator*=(const Vector<U> &v) { return holdMul(*this, v, operators::VectorCheckSize); }
    template <typename U> Vector *operator/=(const Vector<U> &v) { return holdDiv(*this, v, operators::VectorCheckSize); }

    const bool operator==(const T val) const ;
    const bool operator!=(const T val) const { return !(*this == val); }
    // template <typename U> Vector *operator=(const U val) { return fill(val); }
    Vector *operator+=(const T &val) { return holdAdd(*this, val, false); }
    Vector *operator-=(const T &val) { return holdSub(*this, val, false); }
    Vector *operator*=(const T &val) { return holdMul(*this, val, false); }
    Vector *operator/=(const T &val) { return holdMul(*this, 1.0/val, false); }

    Vector *operator=(internal::tmp<Vector> &&v) noexcept { return swap(*v.release()); }
    template <typename U> Vector *operator=(internal::tmp<Vector<U>> &&v) { return hold(*v.release(), operators::VectorCheckSize); }
    template <typename U> Vector *operator+=(internal::tmp<Vector<U>> &&v) { return holdAdd(*this, *v.release(), operators::VectorCheckSize); }
    template <typename U> Vector *operator-=(internal::tmp<Vector<U>> &&v) { return holdSub(*this, *v.release(), operators::VectorCheckSize); }
    template <typename U> Vector *operator*=(internal::tmp<Vector<T>> &&v) { return holdMul(*this, *v.release(), operators::VectorCheckSize); }
    template <typename U> Vector *operator/=(internal::tmp<Vector<U>> &&v) { return holdDiv(*this, *v.release(), operators::VectorCheckSize); }



protected:
    static const Vector staticHelper;
    virtual inline const size_t minMemorySize(const size_t N) const noexcept { return N; }
    template <typename U> const bool overlap(const Vector<U> &other) const noexcept;
    Vector *refer(const Vector &other) noexcept { return refer(other.begin(), other.size()); }
    Vector *refer(T *data, size_t length) noexcept;
    Vector *swap(Vector &other) noexcept;
    Vector *allocate(const size_t capacity, const bool deallocIfPossible = true, const bool saveData = true);
};

namespace operators
{
    template <typename T> internal::tmp<Vector<T>> &&operator+(const Vector<T> &v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdAdd(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(internal::tmp<Vector<T>> &&v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdAdd(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(const Vector<T> &v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v2.holdAdd(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(internal::tmp<Vector<T>> &&v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdAdd(v1, *v2.release(), VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator-(const Vector<T> &v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdSub(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(internal::tmp<Vector<T>> &&v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdSub(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(const Vector<T> &v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v2.holdSub(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(internal::tmp<Vector<T>> &&v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdSub(v1, *v2.release(), VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator*(const Vector<T> &v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdMul(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(internal::tmp<Vector<T>> &&v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdMul(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(const Vector<T> &v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v2.holdMul(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(internal::tmp<Vector<T>> &&v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdMul(v1, *v2.release(), VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator/(const Vector<T> &v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdDiv(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(internal::tmp<Vector<T>> &&v1, const Vector<T> &v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdDiv(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(const Vector<T> &v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v2.holdDiv(v1, v2, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(internal::tmp<Vector<T>> &&v1, internal::tmp<Vector<T>> &&v2) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdDiv(v1, *v2.release(), VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator+(const Vector<T> &v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdAdd(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(internal::tmp<Vector<T>> &&v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdAdd(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(const T val, const Vector<T> &v1) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdAdd(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator+(const T val, internal::tmp<Vector<T>> &&v1) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdAdd(val, v1, VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator-(const Vector<T> &v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdSub(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(internal::tmp<Vector<T>> &&v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdSub(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(const T val, const Vector<T> &v1) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdSub(val, v1, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator-(const T val, internal::tmp<Vector<T>> &&v1) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdSub(val, v1, VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator*(const Vector<T> &v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdMul(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(internal::tmp<Vector<T>> &&v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdMul(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(const T val, const Vector<T> &v1) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdMul(v1, val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator*(const T val, internal::tmp<Vector<T>> &&v1) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdMul(val, v1, VectorCheckSize)); }

    template <typename T> internal::tmp<Vector<T>> &&operator/(const Vector<T> &v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdMul(v1, 1/val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(internal::tmp<Vector<T>> &&v1, const T val) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdMul(v1, 1.0/val, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(const T val, const Vector<T> &v1) { return internal::move(*(internal::tmp<Vector<T>> *)internal::tmp<Vector<T>>::get(v1.size())->holdDiv(val, v1, VectorCheckSize)); }
    template <typename T> internal::tmp<Vector<T>> &&operator/(const T val, internal::tmp<Vector<T>> &&v1) { return internal::move(*(internal::tmp<Vector<T>> *)v1.holdDiv(val, v1, VectorCheckSize)); }
    
    // rowMajorMatrix and Vector
    template<typename T, typename U> internal::tmp<Vector<T>> &&operator*(const rowMajorMatrix<T> &a, const Vector<U> &b) { return internal::move(*(internal::tmp<Vector<T>>*)internal::tmp<Vector<T>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize)); };
    // MatrixBase and Vector
    template<typename T, typename U> internal::tmp<Vector<T>> &&operator*(const MatrixBase<T> &a, const Vector<U> &b) { return internal::move(*(internal::tmp<Vector<T>>*)internal::tmp<Vector<T>>::get(a.rows())->holdMul(a, b, operators::MatrixCheckSize)); };
} // namespace operator

template <typename T> String to_string(const Vector<T> &v);

#ifndef VECTOR_CPP
#include "vector.cpp"
#endif
#endif // VECTOR_HPP