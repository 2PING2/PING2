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


#define VECTOR_CPP
#include "vector.hpp"

template <typename T>
const Vector<T> Vector<T>::staticHelper = Vector<T>();


template <typename T>
Vector<T>::Vector(const size_t N) : Vector()
{
    internal::alloc_count+=N*sizeof(T);
    _begin = new T[N];
    _end = _begin + N;
    _endOfStorage = _end;
}

template <typename T>
Vector<T>::Vector(const Vector &v, const bool share) : Vector()
{
    _begin = share ? v._begin : new T[v.size()];
    const size_t N = v.size();
    _end = _begin + N;
    _endOfStorage = share ? nullptr : _end;
    if (!share)
    {
        internal::alloc_count+=N*sizeof(T);
        memcpy(_begin, v._begin, N * sizeof(T));
    }
}

template <typename T>
Vector<T>::Vector(T *begin, const size_t N, const bool share) : Vector()
{
    _begin = share ? begin : new T[N];
    _end = _begin + N;
    _endOfStorage = share ? nullptr : _end;
    if (!share)
    {
        internal::alloc_count+=N*sizeof(T);
        memcpy(_begin, begin, N * sizeof(T));
    }
}

template <typename T>
Vector<T>::~Vector()
{
    internal::vec_count--;
    if (!shared())
    {
        internal::alloc_count-=capacity()*sizeof(T);
        delete[] _begin;
    }
}

template <typename T>
Vector<T> *Vector<T>::fill(const T val)
{
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = val;
    return this;
}

template <typename T>
template <typename U>
const bool Vector<T>::overlap(const Vector<U> &other) const noexcept
{
    if ((void *)this == (void *)&other)
        return true;
    if ((void *)other._begin >= (void *)_begin && (void *)other._begin < (void *)_end)
        return true;
    if ((void *)other._end > (void *)_begin && (void *)other._end <= (void *)_end)
        return true;
    return false;
}

template <typename T>
Vector<T> *Vector<T>::refer(T *data, const size_t length) noexcept
{
    _begin = data;
    _end = _begin + length;
    _endOfStorage = nullptr;
    return this;
}

template <typename T>
Vector<T> *Vector<T>::swap(Vector<T> &other) noexcept
{
    T *tmp = _begin;
    _begin = other._begin;
    other._begin = tmp;
    tmp = _end;
    _end = other._end;
    other._end = tmp;
    tmp = _endOfStorage;
    _endOfStorage = other._endOfStorage;
    other._endOfStorage = tmp;
    return this;
}

template <typename T>
Vector<T> *Vector<T>::allocate(const size_t capacity, const bool deallocIfPossible, const bool saveData)
{
    if (_begin == nullptr)
    {
        _begin = new T[capacity];
        internal::alloc_count+=capacity*sizeof(T);
        _end = _begin + capacity;
        _endOfStorage = _end;
        return this;
    }
    if (shared())
    {
        return nullptr;
    }
    T *hypotheticalEndOfStorage = _begin + capacity;
    if (!deallocIfPossible)
        if (hypotheticalEndOfStorage < _endOfStorage)
            return this;
    if (hypotheticalEndOfStorage == _endOfStorage)
        return this;

    T *newBegin = new T[capacity];
    if (newBegin == nullptr)
        return nullptr;
    internal::alloc_count+=capacity*sizeof(T);

    const size_t length = size();
    if (saveData)
        memcpy(newBegin, _begin, length * sizeof(T));
    

    if (_begin)
    {
        internal::alloc_count-=this->capacity()*sizeof(T);
        delete[] _begin;
    }

    _begin = newBegin;
    _end = _begin + length;
    _endOfStorage = _begin + capacity;
    return this;
}

template <typename T>
Vector<T> *Vector<T>::resize(const size_t length, const bool deallocIfPossible, const bool saveData)
{
    if (_begin == nullptr && length > 0)
    {
        allocate(length, false, false);
        return this;
    }

    _end = _begin + length;
    if (!deallocIfPossible && _end < _endOfStorage || shared())
        return this;
    allocate(length, deallocIfPossible, saveData);
    return this;
}

template <typename T>
const size_t Vector<T>::findFirst(const T &value) const
{
    for (T *i = _begin; i < _end; i++)
        if (*i == value)
            return i - _begin;
    return -1;
}

template <typename T>
const size_t Vector<T>::findLast(const T &value) const
{
    for (T *i = _end - 1; i >= _begin; i--)
        if (*i == value)
            return i - _begin;
    return -1;
}

template <typename T>
Vector<size_t> *Vector<T>::findAll(const T &value, Vector<size_t> &indices_holder) const
{
    size_t count = 0;
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        if (_begin[i] == value)
            count++;

    indices_holder.resize(count, false, false);

    count = 0;
    for (size_t i = 0; i < N; i++)
        if (_begin[i] == value)
            indices_holder[count++] = i;

    return &indices_holder;
}

template <typename T>
Vector<T> *Vector<T>::removeFirst(const T &value)
{
    size_t index = findFirst(value);
    if (index != -1)
        removeAt(index);
    return this;
}

template <typename T>
Vector<T> *Vector<T>::removeLast(const T &value)
{
    size_t index = findLast(value);
    if (index != -1)
        removeAt(index);
    return this;
}

template <typename T>
Vector<T> *Vector<T>::removeAt(const size_t index)
{
    if (index >= size())
        return this;

    memmove(_begin + index, _begin + index + 1, (size() - index - 1) * sizeof(T));
    _end--;
    return this;
}
template <typename T>
Vector<T> *Vector<T>::push_back(const T &value)
{
    if (_begin == nullptr)
    {
        allocate(1, false, true);
        *_begin = value;
    }
    else
    {
        if (_end >= _endOfStorage)
            allocate(size() + 1, false, true);
        *_end = value;
        _end++;
    }
    return this;
}

template <typename T>
Vector<T> *Vector<T>::pop_back()
{
    if (size() == 0)
        return this;
    _end--;
    return this;
}

template <typename T>
Vector<T> *Vector<T>::insert(const T &value, const size_t index)
{
    const size_t N = size();
    if (index >= N)
        return this;
    resize(N + 1);
    memmove(_begin + index + 1, _begin + index, (N - index) * sizeof(T));
    _begin[index] = value;
    return this;
}

template <typename T>
Vector<T> *Vector<T>::sort(const bool ascending)
{
    const size_t N = size();
    // selectionSort
    for (size_t i = 0; i < N - 1; i++)
    {
        size_t minIndex = i;
        for (size_t j = i + 1; j < N; j++)
            if (ascending ? _begin[j] < _begin[minIndex] : _begin[j] > _begin[minIndex])
                minIndex = j;
        internal::swap(_begin[i], _begin[minIndex]);
    }
    return this;
}

template <typename T>
template <typename U, typename V>
V Vector<T>::dot(const Vector<U> &other, const bool checkSize)
{
    if (checkSize && size() != other.size())
        throw "Vectors are not compatible for dot product";
    V sum = 0;
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        sum += _begin[i] * other._begin[i];
    return sum;
}

template <typename T>
Vector<T> *Vector<T>::hold(T *begin, const size_t N, const bool checkSize)
{
    if (checkSize)
        resize(N, false, false);
    memcpy(_begin, begin, N * sizeof(T));
    return this;
}


template <typename T>
Vector<T> *Vector<T>::hold(const Vector &v, const bool checkSize)
{
    if (checkSize)
        resize(v.size(), false, false);
    if (this == &v)
        return this;

    memcpy(_begin, v._begin, size() * sizeof(T));
    return this;
}

template <typename T>
template <typename U>
Vector<T> *Vector<T>::hold(const Vector<U> &v, const bool checkSize)
{
    if (checkSize)
        resize(v.size(), false, false);
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = v._begin[i];
    return this;
}

////////////////////////// matrixBase and Vector //////////////////////////
template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdMul(const MatrixBase<U> &a, const Vector<V> &b, const bool checkSize)
{
    if (checkSize)
        {
            if (a.cols() != b.size())
                throw "Matrix and vector are not compatible for multiplication";
            this->resize(a.rows(), false, false);
        }

    // optimized on ESP32
    size_t index = 0;
    size_t a_index = 0;
    size_t size = this->size();
    for (size_t i = 0; i < size; i++)
    {
        T sum = 0;
        for (size_t k = 0; k < a.cols(); k++)
            sum += a(i,k) * b._begin[k];
        this->_begin[index++] = sum;
    
        a_index += a.cols();
    }
    return this;
}

////////////////////////// rowMajorMatrix and Vector //////////////////////////
template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdMul(const rowMajorMatrix<U> &a, const Vector<V> &b, const bool checkSize)
{
    if (checkSize)
        {
            if (a.cols() != b.size())
                throw "Matrix and vector are not compatible for multiplication";
            this->resize(a.rows(), false, false);
        }

    // optimized on ESP32
    size_t index = 0;
    size_t a_index = 0;
    size_t size = this->size();
    for (size_t i = 0; i < size; i++)
    {
        T sum = 0;
        for (size_t k = 0; k < a._cols; k++)
            sum += a._begin[a_index + k] * b._begin[k];
        this->_begin[index++] = sum;
    
        a_index += a._cols;
    }
    return this;
}
template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::addMul(const rowMajorMatrix<U> &a, const Vector<V> &b, const bool checkSize)
{
    if (checkSize)
        {
            if (a.cols() != b.size())
                throw "Matrix and vector are not compatible for multiplication";
            this->resize(a.rows(), false, false);
        }

    // optimized on ESP32
    size_t index = 0;
    size_t a_index = 0;
    size_t size = this->size();
    for (size_t i = 0; i < size; i++)
    {
        T sum = 0;
        for (size_t k = 0; k < a._cols; k++)
            sum += a._begin[a_index + k] * b._begin[k];
        this->_begin[index++] += sum;
    
        a_index += a._cols;
    }
    return this;
}

// symatrix and vector
// template <typename T>
// template <typename U, typename V>
// Vector<T> *Vector<T>::holdMul(const symMatrix<U> &a, const Vector<V> &b, const bool checkSize)
// {
//     if (checkSize)
//         {
//             if (a.cols() != b.size())
//                 throw "Matrix and vector are not compatible for multiplication";
//             this->resize(a.rows(), false, false);
//         }

//     // optimized on ESP32
//     size_t index = 0;
//     size_t a_index = 0;
//     size_t size = this->size();
//     for (size_t i = 0; i < size; i++)
//     {
//         T sum = 0;
//         for (size_t k = 0; k < a._cols; k++)
//             sum += a(i,k) * b._begin[k];
//         this->_begin[index++] = sum;
    
//         a_index += a._cols;
//     }
//     return this;
// }

template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdAdd(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize)
{
    if (checkSize)
        resize(v1.size(), false, false);
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] + v2._begin[i];
    return this;
}

template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdSub(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize)
{
    if (checkSize)
        resize(v1.size(), false, false);
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] - v2._begin[i];
    return this;
}

template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdMul(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize)
{
    if (checkSize)
        resize(v1.size(), false, false);
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] * v2._begin[i];
    return this;
}
template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdDiv(const Vector<U> &v1, const Vector<V> &v2, const bool checkSize)
{
    if (checkSize)
        resize(v1.size(), false, false);
    const size_t N = size();
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] / v2._begin[i];
    return this;
}

template <typename T>
template <typename U>
Vector<T> *Vector<T>::holdAdd(const Vector<U> &v1, const T val, const bool checkSize)
{
    const size_t N = v1.size();
    if (checkSize)
        resize(N, false, false);
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] + val;
    return this;
}

template <typename T>
template <typename U>
Vector<T> *Vector<T>::holdSub(const Vector<U> &v1, const T val, const bool checkSize)
{
    const size_t N = v1.size();
    if (checkSize)
        resize(N, false, false);
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] - val;
    return this;
}

template <typename T>
template <typename U, typename V>
Vector<T> *Vector<T>::holdMul(const Vector<U> &v1, const V val, const bool checkSize)
{
    const size_t N = v1.size();
    if (checkSize)
        resize(N, false, false);
    for (size_t i = 0; i < N; i++)
        _begin[i] = v1._begin[i] * val;
    return this;
}

template <typename T>
template <typename U>
Vector<T> *Vector<T>::holdSub(const T val, const Vector<U> &v, const bool checkSize)
{
    const size_t N = v.size();
    if (checkSize)
        resize(N, false, false);
    for (size_t i = 0; i < N; i++)
        _begin[i] = val - v._begin[i];
    return this;
}

template <typename T>
template <typename U>
Vector<T> *Vector<T>::holdDiv(const T val, const Vector<U> &v, const bool checkSize)
{
    const size_t N = v.size();
    if (checkSize)
        resize(N, false, false);
    for (size_t i = 0; i < N; i++)
        _begin[i] = val / v._begin[i];
    return this;
}

template <typename T>
template <typename U>
const bool Vector<T>::operator==(const Vector<U> &v) const
{
    if (this == &v)
        return true;
    if (_begin == v._begin && _end == v._end)
        return true;

    const size_t N = size();
    if (N != v.size())
        return false;
    for (size_t i = 0; i < N; i++)
        if (_begin[i] != v._begin[i])
            return false;
    return true;
}

template <typename T>
const bool Vector<T>::operator==(const T val) const
{
    const size_t N = size();
    if (N == 0)
        return false;
    for (size_t i = 0; i < N; i++)
        if (_begin[i] != val)
            return false;
    return true;
}

template <typename T>
String to_string(const Vector<T> &v)
{
    String s = "[";
    const size_t N = v.size();
    for (size_t i = 0; i < N; i++)
    {
        #ifdef ARDUINO
        s += String(v[i]);
        #else
        s += std::to_string(v[i]);
        #endif
        if (i < N - 1)
            s += ", ";
    }
    s += "]";
    return s;
}