#ifndef LA_DEPENDANCIES
#define LA_DEPENDANCIES

#include <array>
#include <memory>
#include <ostream>
#include <iostream>
#include <exception>
#include <ranges>
#include "my_concepts.hpp"

namespace cryptidmath
{
    constexpr const char    *TRANSPOSE = "\u1D40";//should be ^T
    constexpr const char    *ALT_TRANSPOSE = "index is out of range";
    constexpr const char    VECTOR_BRACKET_OPEN = '(',
                            VECTOR_BRACKET_CLOSE = ')';
    constexpr const char    MATRIX_BRACKET_OPEN = '[',
                            MATRIX_BRACKET_CLOSE = ']';
    constexpr const char    *BAD_SIZE_MSG = "vector sizes do not match!";
    constexpr const char    *BAD_INDEX_MSG = "index is out of range";
    constexpr const char    *SEPARATOR = ", ";
    constexpr const char    *ERROR_SIZE_COL_MISMATCH = "Mismatch between matrix and a column vector!";
    constexpr const char    *ERROR_SIZE_ROW_MISMATCH = "Mismatch between matrix and a row vector!";

    enum class VectorOrientation 
    {
        COLUMN,
        ROW
    };


    template <Arithmetic Element, size_t size>
    class Vector;
    

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Matrix;
}


#endif