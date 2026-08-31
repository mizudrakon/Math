#ifndef CRYPTID_MATRIX_H
#define CRYPTID_MATRIX_H

#include <array>
#include <memory>
#include <ostream>
#include <iostream>
#include <exception>
#include <ranges>

#include "my_concepts.hpp"

namespace cryptidmath
{
    constexpr const char    MATRIX_BRACKET_OPEN = '[',
                            MATRIX_BRACKET_CLOSE = ']';
    constexpr const char    *BAD_SIZE_MSG = "vector sizes do not match!";
    constexpr const char    *BAD_INDEX_MSG = "index is out of range";
    constexpr const char    *SEPARATOR = ", ";
    constexpr const char    *ERROR_SIZE_COL_MISMATCH = "Mismatch between matrix and a column vector!";
    constexpr const char    *ERROR_SIZE_ROW_MISMATCH = "Mismatch between matrix and a row vector!";

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Matrix
    {
    private:
        std::shared_ptr<std::array<Element,n_rows * m_cols>> data_;

        class Row;
    public:
        Matrix(const Element& value = 0);
        Matrix(const std::initializer_list<Element>& init_list);
        Element& get(size_t row, size_t column);
        const Element& get(size_t row, size_t column) const;
        void set(size_t row, size_t column, const Element& value);
        void print(std::ostream& stream = std::cout, bool stack = false) const;
        
    

        Matrix& operator++();
        Matrix operator++(int);
        Matrix& operator--();
        Matrix operator--(int);

        Row operator[](size_t row);
        const Row operator[](size_t row) const;

    private:
        void ensure_ownership();

    };


    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::
    Matrix(const Element& value)
        :data_(std::make_shared<std::array<Element,n_rows*m_cols>>())
    {
        data_->fill(value);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::
    Matrix(const std::initializer_list<Element>& init_list)
        :data_(std::make_shared<std::array<Element,n_rows*m_cols>>())
    {
        if (init_list.size() != n_rows*m_cols)
        {
            throw std::length_error(BAD_SIZE_MSG);
        }
        int i = 0;
        for (auto& el : init_list)
        {
            (*data_)[i++] = el;
        }
    }

// CLASS REQUEST
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Matrix<Element,n_rows,m_cols>::Row
    {
    private:
        const Matrix<Element,n_rows,m_cols>& M_;
        size_t row_;
        Row(const Matrix<Element,n_rows,m_cols>& M, size_t row);   
        friend class Matrix;
    public:
        Element& operator[](size_t column);
        const Element& operator[](size_t column) const;
    };

// REQUEST CONSTRUCTOR
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::
    Row::
    Row(const Matrix<Element,n_rows,m_cols>& M, size_t row):M_(M),row_(row){}   

// MATRIX []
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    typename Matrix<Element,n_rows,m_cols>::Row 
    Matrix<Element,n_rows,m_cols>::
    operator[](size_t row)
    {
        return Row(*this,row);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    const typename Matrix<Element,n_rows,m_cols>::Row 
    Matrix<Element,n_rows,m_cols>::
    operator[](size_t row) const
    {
        return Row(*this,row);
    }

// REQUEST []
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Element& Matrix<Element,n_rows,m_cols>::
    Row::operator[](size_t column)
    {
        return const_cast<Matrix&>(M_).get(row_,column);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    const Element& Matrix<Element,n_rows,m_cols>::
    Row::operator[](size_t column) const
    {
        return M_.get(row_,column);
    }

// MATRIX getters and setters
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    inline Element& Matrix<Element,n_rows,m_cols>::
    get(size_t row, size_t column)
    {
        ensure_ownership();
        return (*data_)[row*m_cols + column];
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    inline const Element& Matrix<Element,n_rows,m_cols>::
    get(size_t row, size_t column) const 
    {
        return (*data_)[row*m_cols + column];
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    inline void Matrix<Element,n_rows,m_cols>::
    set(size_t row, size_t column, const Element& value)
    {
        ensure_ownership();
        (*data_)[row*m_cols + column] = value;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    void Matrix<Element,n_rows,m_cols>::
    print(std::ostream& stream, bool stack) const
    {
        stream << MATRIX_BRACKET_OPEN;
        if (stack)
        {
            stream << std::endl << SEPARATOR;
        }
            for (size_t row = 0; row < n_rows; ++row)
        {
            if (row > 0)
            {
                stream << SEPARATOR;
            }
            stream << MATRIX_BRACKET_OPEN << get(row,0);
            for (size_t col = 1; col < m_cols; ++col)
            {
                stream << SEPARATOR << get(row,col);
            }
            stream << MATRIX_BRACKET_CLOSE;
            if (stack)
                stream << std::endl;
        }
        stream << MATRIX_BRACKET_CLOSE;
        if (stack)
            stream << std::endl;
        
    }

// MAKING COPY OF THE DATA
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    void Matrix<Element,n_rows,m_cols>::ensure_ownership()
    {
        if (data_.use_count() > 1){
            data_ = std::make_shared<std::array<Element,n_rows*m_cols>>(*data_);
        }
    }

// MEMBER OPERATORS
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>& 
    Matrix<Element,n_rows,m_cols>::
    operator++()
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            ++e;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols> 
    Matrix<Element,n_rows,m_cols>::
    operator++(int)
    {
        auto M = *this;
        ensure_ownership();
        for (auto&& e : *data_)
        {
            e++;
        }
        return M;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>& 
    Matrix<Element,n_rows,m_cols>::
    operator--()
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            --e;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols> 
    Matrix<Element,n_rows,m_cols>::
    operator--(int)
    {
        auto M = *this;
        ensure_ownership();
        for (auto&& e : *data_)
        {
            e--;
        }
        return M;
    }

// NON-MEMBER OPERATORS:
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element, n_rows, m_cols> operator+(
        const Matrix<Element, n_rows, m_cols>& matrix,
        const Element& increment
    )
    {
        Matrix result(matrix);
        for (size_t row = 0; row < n_rows; ++row)
        {
            for (size_t col = 0; col < m_cols; ++col)
            {
                result[row][col] += increment;
            }
        }
        return result;   
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element, n_rows, m_cols> operator*(
        const Matrix<Element, n_rows, m_cols>& matrix,
        const Element& factor
    )
    {
        Matrix result(matrix);
        for (size_t row = 0; row < n_rows; ++row)
        {
            for (size_t col = 0; col < m_cols; ++col)
            {
                result[row][col] *= factor;
            }
        }
        return result;   
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element, n_rows, m_cols> operator+(
        const Matrix<Element, n_rows, m_cols>& matrix_1,
        const Matrix<Element, n_rows, m_cols>& matrix_2
    )
    {
        Matrix result(matrix_1);
        for (size_t row = 0; row < n_rows; ++row)
        {
            for (size_t col = 0; col < m_cols; ++col)
            {
                result[row][col] += matrix_2[row][col];
            }
        }
        return result;   
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, size_t Depth>
    Matrix<Element, n_rows, m_cols> operator*(
        const Matrix<Element, n_rows, Depth>& matrix_1,
        const Matrix<Element, Depth, m_cols>& matrix_2
    )
    {
        Matrix<Element, n_rows, m_cols> result;
        for (size_t row = 0; row < n_rows; ++row)
        {
            for (size_t col = 0; col < m_cols; ++col)
            {
                for (size_t d = 0; d < Depth; ++d)
                {
                    result[row][col] += matrix_1[row][d]*matrix_2[d][col];
                }
            }
        }
        return result;   
    }
 


// OSTREAM OVERLOAD
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    std::ostream& operator<<(std::ostream& stream, const Matrix<Element,n_rows,m_cols>& Matrix)
    {
        Matrix.print(stream,true);
        return stream;
    }

}

#include <sstream>

// FORMAT 
template <Arithmetic Element, size_t n_rows, size_t m_cols>
struct std::formatter<cryptidmath::Matrix<Element, n_rows, m_cols>> : std::formatter<std::string> 
{
    template <typename Context>
    auto format(const cryptidmath::Matrix<Element, n_rows, m_cols>& matrix, Context& context) const
    {
        std::ostringstream stream;
        stream << matrix;
        return std::format_to(context.out(),"{}",stream.str());
    }
};

#endif