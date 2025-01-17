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

#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "rowMajorMatrix.hpp"

template <typename TT>
class Matrix: public rowMajorMatrix<TT>
{
    friend class internal::tmp<Matrix>;

    protected:
        Matrix<TT> *swap(Matrix<TT> &other) noexcept{rowMajorMatrix<TT>::swap(other); other.referT(); referT();}
      
    public:
        colMajorMatrix<TT> T;
        Matrix<TT> *referT() noexcept {T.MatrixBase<TT>::refer(*this, this->_cols, this->_rows); return this;}
        Matrix() : rowMajorMatrix<TT>(){}
        Matrix(const size_t rows, const size_t cols) : rowMajorMatrix<TT>(rows, cols){referT();}
        Matrix(TT *data, const size_t rows, const size_t cols, const bool share = true) : rowMajorMatrix<TT>(data, rows, cols, share){referT();}
        Matrix(const Matrix<TT> &other) : rowMajorMatrix<TT>(other){referT();}
        template<typename U> Matrix(const Matrix<U> &other) : rowMajorMatrix<TT>(other){referT();}
        Matrix(const rowMajorMatrix<TT> &other) : rowMajorMatrix<TT>(other){referT();}
        template<typename U> Matrix(const rowMajorMatrix<U> &other) : rowMajorMatrix<TT>(other){referT();}
        Matrix(const colMajorMatrix<TT> &other) : rowMajorMatrix<TT>(other){referT();}
        template<typename U> Matrix(const colMajorMatrix<U> &other) : rowMajorMatrix<TT>(other){referT();}
        Matrix(internal::tmp<rowMajorMatrix<TT>> &&other) : rowMajorMatrix<TT>(internal::move(other)){referT();}
        Matrix(internal::tmp<colMajorMatrix<TT>> &&other) : rowMajorMatrix<TT>(internal::move(other)){referT();}

        virtual Matrix<TT> *resize(const size_t rows, const size_t cols, const bool deallocIfPossible = false, const bool saveData = true) override {rowMajorMatrix<TT>::resize(rows, cols, deallocIfPossible, saveData); return referT();}

        ///////////////////////////// operators /////////////////////////////
        // dataType
        template<typename U> Matrix<TT> *operator+=(const U &data){return (Matrix<TT> *)rowMajorMatrix<TT>::operator+=(data);}
        template<typename U> Matrix<TT> *operator-=(const U &data){return (Matrix<TT> *)rowMajorMatrix<TT>::operator-=(data);}
        template<typename U> Matrix<TT> *operator*=(const U &data){return (Matrix<TT> *)rowMajorMatrix<TT>::operator*=(data);}
        template<typename U> Matrix<TT> *operator/=(const U &data){return (Matrix<TT> *)rowMajorMatrix<TT>::operator/=(data);}

        // rowMajorMatrix
        template<typename U> Matrix<TT> *operator=(const rowMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator=(other);}
        template<typename U> Matrix<TT> *operator+=(const rowMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator+=(other);}
        template<typename U> Matrix<TT> *operator-=(const rowMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator-=(other);}
        template<typename U> Matrix<TT> *operator*=(const rowMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator*=(other);}

        // tmp rowMajorMatrix
        template<typename U> Matrix<TT> *operator=(internal::tmp<rowMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator=(internal::move(other));}
        template<typename U> Matrix<TT> *operator+=(internal::tmp<rowMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator+=(internal::move(other));}
        template<typename U> Matrix<TT> *operator-=(internal::tmp<rowMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator-=(internal::move(other));}
        template<typename U> Matrix<TT> *operator*=(internal::tmp<rowMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator*=(internal::move(other));}

        // colMajorMatrix
        template<typename U> Matrix<TT> *operator=(const colMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator=(other);}
        template<typename U> Matrix<TT> *operator+=(const colMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator+=(other);}
        template<typename U> Matrix<TT> *operator-=(const colMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator-=(other);}
        template<typename U> Matrix<TT> *operator*=(const colMajorMatrix<U> &other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator*=(other);}

        // tmp colMajorMatrix
        template<typename U> Matrix<TT> *operator=(internal::tmp<colMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator=(internal::move(other));}
        template<typename U> Matrix<TT> *operator+=(internal::tmp<colMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator+=(internal::move(other));}
        template<typename U> Matrix<TT> *operator-=(internal::tmp<colMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator-=(internal::move(other));}
        template<typename U> Matrix<TT> *operator*=(internal::tmp<colMajorMatrix<U>> &&other){return (Matrix<TT> *)rowMajorMatrix<TT>::operator*=(internal::move(other));}        
};

namespace operators
{
    template<typename T, typename U, typename V=decltype(T() + U())> internal::tmp<rowMajorMatrix<T>> &&operator+(const Matrix<T> &a, const Matrix<T> &b){return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdAdd(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V=decltype(T() - U())> internal::tmp<rowMajorMatrix<T>> &&operator-(const Matrix<T> &a, const Matrix<T> &b){return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), a.cols())->holdSub(a, b, operators::MatrixCheckSize)); };
    template<typename T, typename U, typename V=decltype(T() * U())> internal::tmp<rowMajorMatrix<T>> &&operator*(const Matrix<T> &a, const Matrix<T> &b){return internal::move(*(internal::tmp<rowMajorMatrix<T>>*)internal::tmp<rowMajorMatrix<T>>::get(a.rows(), b.cols())->holdMul(a, b, operators::MatrixCheckSize)); };

};

#endif // MATRIX_HPP