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

#ifndef LDL_MATRIX_HPP
#define LDL_MATRIX_HPP

#include "ul_triangMatrix.hpp"
#include "diagMatrix.hpp"
#include "uu_triangMatrix.hpp"
// #include "triangMatrix.hpp"

template <typename T>
class ldl_matrix : public symMatrix<T>
{
    // protected:
    public:
        ldl_matrix<T> *referT() noexcept {LT.refer(L, this->_cols, this->_rows); return this;}
    public:
        ul_triangMatrix<T> L;
        diagMatrix<T> D;
        uu_triangMatrix<T> LT;

        ldl_matrix() : symMatrix<T>(){}
        ldl_matrix(const size_t order) : symMatrix<T>(){this->resize(order);}
        ldl_matrix<T> *resize(const size_t order, const bool deallocIfPossible = false, const bool saveData = true) {symMatrix<T>::resize(order,order, deallocIfPossible, saveData); L.resize(order,order); D.resize(order,order); referT(); return this;}
        ldl_matrix<T> *decompose();
};


#ifndef LDL_MATRIX_CPP
#include "ldl_Matrix.cpp"
#endif
#endif