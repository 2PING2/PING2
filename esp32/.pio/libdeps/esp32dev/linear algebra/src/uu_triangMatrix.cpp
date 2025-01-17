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

#define UU_TRIANG_MATRIX_CPP
#include "uu_triangMatrix.hpp"

template <typename T>
const uu_triangMatrix<T> uu_triangMatrix<T>::staticHelper;

template <typename T>
T &uu_triangMatrix<T>::operator()(const size_t row, const size_t col)
{
    if (col > row)
        return this->_begin[(((col - 1) * col) >> 1) + row];
    throw "index inaccessable";
    return this->_begin[-1];
};
