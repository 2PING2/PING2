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

#define DIAG_MATRIX_CPP
#include "diagMatrix.hpp"

template <typename T>
const diagMatrix<T> diagMatrix<T>::staticHelper;

template <typename T>
T &diagMatrix<T>::operator()(const size_t row, const size_t col)
{
    if (row == col)
        return this->_begin[row];
    else
        throw "diagMatrix::operator() out of range";
}

template <typename T>
T diagMatrix<T>::det() const
{
    if (this->rows() != this->cols())
    {
        throw "diagMatrix::det() not a square matrix";
        return 0;
    }
    T res = 1;
    for (size_t i = 0; i < this->_cols; i++)
        res *= this->_begin[i];
    return res;
}