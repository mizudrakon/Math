#ifndef CRYPTID_NDARRAY_H
#define CRYPTID_NDARRAY_H


#include "matrix_hierarchy.h"

namespace cryptidmath
{

// NDARRAY_DEFINITION
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    class NDArray 
    {
    protected:
        std::shared_ptr<std::array<Element,n_rows * m_cols>> data_;
    public:
        NDArray(const Element& value = 0);
        NDArray(const std::initializer_list<Element>& init_list);
        virtual ~NDArray();
        
// NDARRAY_getters_&_setters
        Element& get(size_t row, size_t column);
        const Element& get(size_t row, size_t column) const;
        void set(size_t row, size_t column, const Element& value);

// NDARRAY_MEMBER_OPERATORS
        Derived& operator++();
        Derived operator++(int);
        Derived& operator--();
        Derived operator--(int);
    // NDARRAY_CONSTANT_MEMBER_OPERATIONS
        Derived& operator*=(const Element& k);
        Derived& operator/=(const Element& k);
        Derived& operator+=(const Element& k);
        Derived& operator-=(const Element& k);
    // NDARRAY_NDARRAY_MEMBER_OPERATIONS
        Derived& operator+=(const Derived& rm);
        Derived& operator-=(const Derived& rm);

// NDARRAY_SHAPE_TESTS
        virtual bool is_square() const = 0;
        virtual bool is_row() const = 0;
        virtual bool is_column() const = 0;
        NDArrayType type() const = 0;
        Derived& null();

// NDARRAY_COMMON_MEMBER_OPERATIONS
        virtual void print(std::ostream& stream = std::cout, bool stack = false) const = 0;
        
    protected:
        void ensure_ownership();
    };

// MATRIX_SECTION
// VECTOR_SECTION


// NDARRAY_CONSTRUCTORS
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    NDArray<Element,n_rows,m_cols,Derived>::
    NDArray(const Element& value)
        :data_(std::make_shared<std::array<Element,n_rows*m_cols>>())
    {
        data_->fill(value);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    NDArray<Element,n_rows,m_cols,Derived>::
    NDArray(const std::initializer_list<Element>& init_list)
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

// NDARRAY_getters_and_setters
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    inline Element& NDArray<Element,n_rows,m_cols,Derived>::
    get(size_t row, size_t column)
    {
        ensure_ownership();
        return (*data_)[row*m_cols + column];
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    inline const Element& NDArray<Element,n_rows,m_cols,Derived>::
    get(size_t row, size_t column) const 
    {
        return (*data_)[row*m_cols + column];
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    inline void NDArray<Element,n_rows,m_cols,Derived>::
    set(size_t row, size_t column, const Element& value)
    {
        ensure_ownership();
        (*data_)[row*m_cols + column] = value;
    }

// NDARRAY_MEMBER_OPERATORS
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator++()
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            ++e;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived
    NDArray<Element,n_rows,m_cols,Derived>::
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

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator--()
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            --e;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived
    NDArray<Element,n_rows,m_cols,Derived>::
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

    // NDARRAY_CONSTANT_MEMBER_OPERATIONS
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator*=(const Element& k)
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            e *= k;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator/=(const Element& k)
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            e /= k;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator+=(const Element& k)
    {
        ensure_ownership();
        for (auto&& e : *data_)
        {
            e += k;
        }
        return *this;
    }
    
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator-=(const Element& k)
    {
        this->operator+=(-k);
    }
    
    // NDARRAY_NDARRAY_MEMBER_OPERATIONS
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator+=(const Derived& rm)
    {
        for (size_t i{}; i < n_rows*m_cols; ++i)
        {
            *data_[i] += *rm.data_[i];
        }
        return *this;
    }
    
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& 
    NDArray<Element,n_rows,m_cols,Derived>::
    operator-=(const Derived& rm)
    {
        for (size_t i{}; i < n_rows*m_cols; ++i)
        {
            *data_[i] -= *rm.data_[i];
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    Derived& NDArray<Element,n_rows,m_cols,Derived>::null()
    {
        if (is_square())
        {
            for (size_t row_index = 0; row_index < n_rows; ++row_index)
            {
                for (size_t col_index = 0; col_index < m_cols; ++col_index)
                {
                    set(row_index,col_index,0);
                }
            }
        }
        return *this;
    }


// NDARRAY_PRIVATE
    template <Arithmetic Element, size_t n_rows, size_t m_cols, typename Derived>
    void NDArray<Element,n_rows,m_cols,Derived>::ensure_ownership()
    {
        if (data_.use_count() > 1){
            data_ = std::make_shared<std::array<Element,n_rows*m_cols>>(*data_);
        }
    }


// MATRIX_SECTION
// MATRIX_DEFINITION
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Matrix final : public NDArray<Element,n_rows,m_cols,Matrix<Element,n_rows,m_cols>>
    {
        using Mother = NDArray<Element,n_rows,m_cols,Matrix<Element,n_rows,m_cols>>;
    public:
        using Mother::Mother;
// ROW_definition
        class Row;


// MATRIX_EXTRA_GETTERS_SETTERS
        Matrix<Element,1,m_cols> getRow(size_t row) const;
        Matrix<Element,n_rows,1> getColumn(size_t col) const;

// MATRIX_SHAPE_TESTS
        bool is_square() const override { return n_rows == m_cols; }
        bool is_row() const override { return false; }
        bool is_column() const override { return false; }
        NDArrayType type() const override { 
            if ( is_square() ) return NDArrayType::SQUARE;
            else return NDArrayType::OTHER;
        }

// MATRIX_COMMON_MEMBER_OPERATIONS
        Matrix& I();
        Matrix<Element,m_cols,n_rows> transpose() const override;
        Element determinant() const;
        Element det() const { return determinant(); }
        void swap_row(size_t a, size_t b);
        void swap_row(const Row& a, const Row& b);
        void print(std::ostream& stream = std::cout, bool stack = false) const override;

// MATRIX_ROW_[]
        Row operator[](size_t row);
        const Row operator[](size_t row) const;

    private:
        Element det_qrt(size_t depth, size_t left_row, size_t right_row) const;
        Element det_recursive(std::array<bool,m_cols>& mask, size_t depth) const;

// MATRIX_NON_MEMBER_OPERATORS
// MATRIX_COMMON_NON_MEMBER_OPERATIONS
// MATRIX_OSTREAM_OVERLOAD
// MATRIX_FORMAT
    };


// MATRIX_EXTRA_GETTERS_SETTERS
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,1,m_cols> Matrix<Element,n_rows,m_cols>::getRow(size_t row) const
    {
        Matrix<Element,1,m_cols> result(0);
        for (size_t i = 0; i < m_cols; ++i)
        {
            result[1][i] = this->get(row,i);
        }
        return result;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,1> Matrix<Element,n_rows,m_cols>::getColumn(size_t col) const
    {
        Matrix<Element,n_rows,1> result(0);
        for (size_t i = 0; i < n_rows; ++i)
        {
            result[i][1] = this->get(i,col);
        }
        return result;
    }

// MATRIX_COMMON_MEMBER_OPERATIONS

    // right now it returns the same matrix if the operation cannot be procured
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>& Matrix<Element,n_rows,m_cols>::I()
    {
        if (is_square())
        {
            for (size_t row_index = 0; row_index < n_rows; ++row_index)
            {
                for (size_t col_index = 0; col_index < m_cols; ++col_index)
                {
                    set(row_index,col_index,( (row_index == col_index) ? 1 : 0 ));
                }
            }
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,m_cols,n_rows> Matrix<Element,n_rows,m_cols>::transpose() const
    {
        Matrix<Element,m_cols,n_rows> t_m;
        for (size_t row_index = 0; row_index < n_rows; ++row_index)
        {
            for (size_t col_index = 0; col_index < m_cols; ++col_index)
            {
                t_m.set(col_index,row_index,this->get(row_index,col_index));
            }
        }
        return t_m;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Element Matrix<Element,n_rows,m_cols>::det_qrt(size_t depth, size_t left_col, size_t right_col) const 
    {
        return this->get(depth,left_col)*this->get(depth+1,right_col) - this->get(depth,right_col)*this->get(depth+1,left_col);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Element Matrix<Element,n_rows,m_cols>::det_recursive(std::array<bool,m_cols>& mask, size_t depth) const
    {
        if (depth == n_rows - 2)
        {
            size_t left_col = 0;
            while (!mask[left_col])
                left_col++;
            size_t right_col = left_col + 1;
            while (!mask[right_col])
                right_col++;
            return det_qrt(depth,left_col,right_col);
        }
        Element sign = 1;
        Element result = 0;
        for (size_t i{}; i < m_cols; ++i)
        {
            if (!mask[i])
                continue;
            mask[i] = false;
            result += sign * this->get(depth,i) * det_recursive(mask,depth+1);
            mask[i] = true;
            sign *= -1;
        }
        return result;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Element Matrix<Element,n_rows,m_cols>::determinant() const
    {
        if (n_rows != m_cols)
        {
            throw std::invalid_argument(NOT_SQUARE_MSG);
        }
        if (n_rows == 1)
        {
            return this->get(0,0);
        }
        if (n_rows == 2)
        {
            return det_qrt(0,0,1);
        }
        std::array<bool,m_cols> mask;
        mask.fill(true);
        return det_recursive(mask,0);
    }
    
// MATRIX_NON_MEMBER_OPERATORS:
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
    void Matrix<Element,n_rows,m_cols>::swap_row(size_t a, size_t b)
    {
        for (size_t i{}; i < n_rows; ++i)
        {
            Element temp = this->get(a,i);
            set(a,i,this->get(b,i));
            set(b,i,temp);
        }
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    void Matrix<Element,n_rows,m_cols>::swap_row(const Row& a, const Row& b)
    {
        for (size_t i{}; i < n_rows; ++i)
        {
            Element temp = this->get(a.row_,i);
            set(a.row_,i,this->get(b.row_,i));
            set(b.row_,i,temp);
        }
    }


    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    void Matrix<Element,n_rows,m_cols>::
    print(std::ostream& stream, bool stack) const
    {
        if (n_rows > 1 || m_cols > 1)
            stream << MATRIX_BRACKET_OPEN;
        if (stack)
        {
            stream << std::endl;
        }
            for (size_t row = 0; row < n_rows; ++row)
        {
            if (row > 0)
            {
                stream << SEPARATOR;
            }
            stream << MATRIX_BRACKET_OPEN << this->get(row,0);
            for (size_t col = 1; col < m_cols; ++col)
            {
                stream << SEPARATOR << this->get(row,col);
            }
            stream << MATRIX_BRACKET_CLOSE;
            if (stack)
                stream << std::endl;
        }
        stream << MATRIX_BRACKET_CLOSE;
        if (stack && (type() == NDArrayType::SQUARE || type() == NDArrayType::OTHER))
            stream << std::endl;
    }

// jump to NDARRAY_DEFINITION
// jump to ROW_definition
// jump to MATRIX_DEFINITION

// ROW_definition
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Matrix<Element,n_rows,m_cols>::Row
    {
    private:
        const Matrix<Element,n_rows,m_cols>& M_;
        size_t row_;
// ROW_CONSTRUCTOR
        Row(const Matrix<Element,n_rows,m_cols>& M, size_t row);   
        friend class Matrix;
    public:
// ROW_MATRIX[]
        Element& operator[](size_t column);
        const Element& operator[](size_t column) const;

// ROW_MEMBER_OPERATORS
        
        Row& operator*=(const Element& k);
        Row& operator/=(const Element& k);
        Row& operator+=(const Row& other_row);
        Row& operator+=(const Matrix<Element,1,m_cols>& row_vector);
        Row& operator-=(const Row& other_row);
        Row& operator-=(const Matrix<Element,1,m_cols>& row_vector);
        
// MULT_OPERATOR_k_ROW
#ifdef MEMBER_OPkROW
        friend Matrix<Element,1,m_cols> operator*(const Element& k, const Row& other_row)
        {
            Matrix<Element,1,m_cols> result{};
            for (size_t i{}; i < m_cols; ++i)
            {
                result[0][i] = k*other_row.M_[other_row.row_][i];
            }
            return result;
        }
        friend Matrix<Element,1,m_cols> operator/(const Row& other_row, const Element& k)
        {
            Matrix<Element,1,m_cols> result{};
            for (size_t i{}; i < m_cols; ++i)
            {
                result[0][i] = other_row.M_[other_row.row_][i] / k;
            }
            return result;
        }
        friend Matrix<Element,1,m_cols> operator+(const Row& row1, const Row& row2)
        {
            Matrix<Element,1,m_cols> result{};
            for (size_t i{}; i < m_cols; ++i)
            {
                std::cout << i << '<' << m_cols << std::endl;
                std::cout << row1.M_[row1.row_][i] << '+' << row2.M_[row2.row_][i] << std::endl;
                result[0][i] = row1.M_[row1.row_][i] + row2.M_[row2.row_][i];
            }
            return result;
        }
        friend Matrix<Element,1,m_cols> operator-(const Row& row1, const Row& row2)
        {
            Matrix<Element,1,m_cols> result{};
            for (size_t i{}; i < m_cols; ++i)
            {
                std::cout << i << '<' << m_cols << std::endl;
                std::cout << row1.M_[row1.row_][i] << '-' << row2.M_[row2.row_][i] << std::endl;
                result[0][i] = row1.M_[row1.row_][i] - row2.M_[row2.row_][i];
            }
            return result;
        }
#endif
        void swap(const Row& other_row);
    };

// ROW_MEMBER_OPERATORS
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator*=(const Element& k)
    {
        for (size_t i = row_*m_cols; i < (row_+1)*m_cols; ++i)
        {
            (*M_.data_)[i] *= k;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator/=(const Element& k)
    {
        for (size_t i = row_*m_cols; i < (row_+1)*m_cols; ++i)
        {
            (*M_.data_)[i] /= k;
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator+=(const Row& other_row)
    {
        for (size_t i = row_*m_cols, j = other_row.row_*m_cols; i < (row_+1)*m_cols; ++i, ++j)
        {
            (*M_.data_)[i] += (*other_row.M_.data_)[j];
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator+=(const Matrix<Element,1,m_cols>& row_vector)
    {
        for (size_t i = row_*m_cols, j{}; i < (row_+1)*m_cols; ++i, ++j)
        {
            (*M_.data_)[i] += row_vector[0][j];
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator-=(const Row& other_row)
    {
        for (size_t i = row_*m_cols, j = other_row.row_*m_cols; i < (row_+1)*m_cols; ++i, ++j)
        {
            (*M_.data_)[i] -= (*other_row.M_.data_)[j];
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::Row& Matrix<Element,n_rows,m_cols>::Row::operator-=(const Matrix<Element,1,m_cols>& row_vector)
    {
        for (size_t i = row_*m_cols, j{}; i < (row_+1)*m_cols; ++i, ++j)
        {
            (*M_.data_)[i] -= row_vector[0][j];
        }
        return *this;
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    void Matrix<Element,n_rows,m_cols>::Row::swap(const Row& other_row)
    {
        for (size_t i{}; i < m_cols; ++i)
        {
            Element tmp = (*M_.data_)[row_*m_cols+i];
            (*M_.data_)[row_*m_cols+i] = (*M_.data_)[other_row.row_*m_cols+i];
            (*M_.data_)[other_row.row_*m_cols+i] = tmp;
        }
    }

// ROW_CONSTRUCTOR
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Matrix<Element,n_rows,m_cols>::
    Row::
    Row(const Matrix<Element,n_rows,m_cols>& M, size_t row):M_(M),row_(row){}   

// MATRIX_ROW[]
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

// ROW_MATRIX[]
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    Element& Matrix<Element,n_rows,m_cols>::
    Row::operator[](size_t column)
    {
        return const_cast<Matrix&>(M_).this->get(row_,column);
    }

    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    const Element& Matrix<Element,n_rows,m_cols>::
    Row::operator[](size_t column) const
    {
        return M_.this->get(row_,column);
    }

// jump to NDARRAY_DEFINITION
// jump to ROW_definition
// jump to MATRIX_DEFINITION

// VECTOR_SECTION
    template <Arithmetic Element, size_t n_rows, size_t m_cols>
    class Vector final : public NDArray<Element,n_rows,m_cols,Vector<Element,n_rows,m_cols>>
    {
        using Mother = NDArray<Element,n_rows,m_cols,Vector<Element,n_rows,m_cols>>;
    public:
        using Mother::Mother;
// VECTOR_SHAPE_TESTS
        bool is_square() const override { return false; }
        bool is_row() const override { return n_rows == 1 && m_cols > 1; }
        bool is_column() const override { return n_rows > 1 && m_cols == 1; }
        NDArrayType type() const override {
            if ( is_row ) return NDArrayType::ROW;
            return NDArrayType::COLUMN;
        }
        NDArray<Element,m_cols,n_rows> transpose() const override;

// VECTOR_COMMON_MEMBER_OPERATIONS
        void print(std::ostream& stream = std::cout, bool stack = false) const override;
        
    };

}


// jump to NDARRAY_DEFINITION
// jump to ROW_definition
// jump to MATRIX_DEFINITION
#endif