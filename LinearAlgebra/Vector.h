#ifndef CRYPTID_VECTOR_H
#define CRYPTID_VECTOR_H

#include "LA_dependancies.h"
#include "Matrix.h"

using std::size_t;

namespace cryptidmath
{

#ifdef OWNERSHIP_TEST
    size_t eo_counter = 0;
#endif
    template <Arithmetic Element, size_t size>
    class Vector 
    {
    private:
        std::shared_ptr<std::array<Element, size>> data_;
        VectorOrientation orientation_ = VectorOrientation::COLUMN;
        // default orientation is column, but we can optionally pass it to the constructor

    public:
    // CONSTRUCTORS
        Vector(const Element& value = 0, VectorOrientation orientation = VectorOrientation::COLUMN);
        Vector(const std::initializer_list<Element> init_list);
        // Vector(const Matrix&, size_t row or column index)

    // GETTERS & SETTERS
        Element& get(size_t index);
        const Element& get(size_t index) const;
        void set(size_t index, const Element& value);
        void fill(const Element& value);

        void print(std::ostream& stream = std::cout, const char* separator = SEPARATOR) const;
        
        VectorOrientation orientation() const
        {
            return orientation_;
        }
        void set_orientation(VectorOrientation orientation)
        {
            orientation_ = orientation;
        }

        Element& operator[](size_t index)
        {
            return get(index);
        }        
        const Element& operator[](size_t index) const
        {
            return get(index);
        }
    // Arithmetic 
        Vector& operator++();
        Vector operator++(int);
        Vector& operator--();
        Vector operator--(int);
        Vector operator-() const;
        Vector& operator*=(const Element& k);
        Vector& operator+=(const Element& k);
        Vector& operator-=(const Element& k);
        Vector& operator*=(const Vector<Element,size>& rvec);
        Vector& operator+=(const Vector<Element,size>& rvec);
        Vector& operator-=(const Vector<Element,size>& rvec);

    private:
        void ensure_ownership();

    public:
        using iterator = typename std::array<Element,size>::iterator;
        using const_iterator = typename std::array<Element,size>::const_iterator;

        iterator begin(){
            return data_->begin();
        }
        iterator end()
        {
            return data_->end();
        }
        const_iterator begin() const 
        {
            return data_->begin();
        }
        const_iterator end() const 
        {
            return data_->end();
        }
        const_iterator cbegin() const
        {
            return data_->cbegin();
        }
        const_iterator cend() const
        {
            return data_->cend();
        }
    };

// CONSTRUCTORS begin

    template <Arithmetic Element, size_t size>
    Vector<Element,size>::Vector(const Element& value, VectorOrientation orientation)
        :data_(std::make_shared<std::array<Element,size>>()),orientation_(orientation)
    {
        data_->fill(value);
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size>::Vector(const std::initializer_list<Element> init_list)
        :data_(std::make_shared<std::array<Element,size>>())
    {
        if (init_list.size() != size)
        {
            throw std::length_error(BAD_SIZE_MSG);
        }
        int i = 0;
        for (auto& el : init_list)
        {
            (*data_)[i++] = el;
        }
    }
    
// CONSTRUCTORS end

// GETTERS & SETTERS begin
    template <Arithmetic Element, size_t size>
    inline Element& Vector<Element,size>::get(size_t index)
    {
        if (index >= size)
        {
            throw std::out_of_range(BAD_INDEX_MSG);
        }
        ensure_ownership();
        return (*data_)[index];
    }
    
    template <Arithmetic Element, size_t size>
    inline const Element& Vector<Element,size>::get(size_t index) const
    {
        if (index >= size)
        {
            throw std::out_of_range(BAD_INDEX_MSG);
        }
        return (*data_)[index];
    }
    
    template <Arithmetic Element, size_t size>
    inline void Vector<Element,size>::set(size_t index, const Element& value)
    {
        ensure_ownership();
        (*data_)[index] = value;
    }

    inline void compare_sz(size_t lsize, size_t rsize)
    {
        if (lsize != rsize){
            throw std::invalid_argument(BAD_SIZE_MSG);
        }
    }

    template <Arithmetic Element, size_t size>
    void Vector<Element,size>::fill(const Element& value)
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            el = value;
        }
    }

    template <Arithmetic Element, size_t size>
    void Vector<Element,size>::ensure_ownership()
    {
#ifdef OWNERSHIP_TEST
        eo_counter++;
#endif
        if (data_.use_count() > 1)
        {
            data_ = std::make_shared<std::array<Element,size>>(*data_);
        }
    }

// GETTERS & SETTERS end

    template <Arithmetic Element, size_t size>
    void Vector<Element,size>::print(std::ostream& stream, const char* separator) const
    {
        stream << VECTOR_BRACKET_OPEN << ' ';
        for (auto el_it = data_->begin(); el_it < data_->end(); ++el_it)
        {
            stream << *el_it << ((el_it == data_->end() - 1) ? " " : separator);
        }
        stream << VECTOR_BRACKET_CLOSE 
            << ((orientation_ == VectorOrientation::COLUMN) ? TRANSPOSE : ""); 
    }

// Arithmetic begin 
    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator++()
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            ++el;
        }
        return *this;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size> Vector<Element,size>::operator++(int)
    {
        auto V = *this;
        ++(*this);
        return V;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator--()
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            --el;
        }
        return *this;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size> Vector<Element,size>::operator--(int)
    {
        auto V = *this;
        ensure_ownership();
        --(*this);
        return V;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size> Vector<Element,size>::operator-() const
    {
        auto result = *this;
        result.ensure_ownership();
        for (auto& el : *(result.data_))
        {
            el = -el;
        }
        return result;
    }
    
    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator*=(const Element& k)
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            el *= k;
        }
        return *this;
    }
 
    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator+=(const Element& k)
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            el += k;
        }
        return *this;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator-=(const Element& k)
    {
        ensure_ownership();
        for (auto& el : *data_)
        {
            el -= k;
        }
        return *this;
    }

    
    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator*=(const Vector<Element,size>& rvec)
    {
        ensure_ownership();
        for (auto [ls,rs] : std::ranges::zip_view(*data_,*rvec.data_))
        {
            ls *= rs;
        }
        return *this;
    }
 
    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator+=(const Vector<Element,size>& rvec)
    {
        ensure_ownership();
        for (auto [ls,rs] : std::ranges::zip_view(*data_,*rvec.data_))
        {
            ls += rs;
        }
        return *this;
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size>& Vector<Element,size>::operator-=(const Vector<Element,size>& rvec)
    {
        ensure_ownership();
        for (auto [ls,rs] : std::ranges::zip_view(*data_,*rvec.data_))
        {
            ls -= rs;
        }
        return *this;
    }

// ARITHEMTIC NON-MEMBER:
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Element operator*(const Vector<Element,lsize>& ls, const Vector<Element,rsize>& rs)
    {
        compare_sz(lsize,rsize);
        Element result{0};
        for (auto ls_it = ls.cbegin(), rs_it = rs.cbegin(); ls_it < ls.cend();++ls_it,++rs_it)
        {
            result += (*ls_it) * (*rs_it);
        }
        return result;
    }
    
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator+(Vector<Element,lsize> ls,const Vector<Element,rsize>& rs)
    {
        compare_sz(lsize,rsize);
        ls += rs;
        return ls;
    }

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator-(Vector<Element,lsize> ls,const Vector<Element,rsize>& rs)
    {
        compare_sz(lsize,rsize);
        ls -= rs;
        return ls;
    }


    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator*(Vector<Element,rsize> ls_vec, const Element& k)
    {
        ls_vec *= k;
        return ls_vec;
    }
    
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator*(const Element& k, Vector<Element,rsize> rs_vec)
    {
        rs_vec *= k;
        return rs_vec;
    }


    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator+(Vector<Element,rsize> ls_vec, const Element& k)
    {
        ls_vec += k;
        return ls_vec;
    }
    
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator+(const Element& k, Vector<Element,rsize> rs_vec)
    {
        rs_vec += k;
        return rs_vec;
    }

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator-(Vector<Element,rsize> ls_vec, const Element& k)
    {
        ls_vec -= k;
        return ls_vec;
    }
    
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator-(const Element& k, Vector<Element,rsize> rs_vec)
    {
        for (auto& vec_it = rs_vec.begin(); vec_it < rs_vec.end(); ++rs_vec)
        {
            *vec_it = k - *vec_it;
        }
        return rs_vec;
    }

    template<Arithmetic Element, size_t lsize, size_t rsize>
    bool operator==(const Vector<Element,lsize>& ls,const Vector<Element,rsize>& rs)
    {
        compare_sz(lsize,rsize);
        auto ls_it = ls.cbegin();
        auto rs_it = rs.cbegin();
        if (ls_it == rs_it) return true;
        while (ls_it < ls.cend() && *ls_it == *rs_it)
        {
            ++ls_it;
            ++rs_it;
        }
        return ls_it == ls.cend();
    }
   
    template <Arithmetic Element, size_t n_rows, size_t m_cols, size_t vector_size>
    Vector<Element, n_rows> operator*(
        const Matrix<Element, n_rows, m_cols>& matrix,
        const Vector<Element, vector_size>& vector
    )
    {
        if (vector.orientation() != cryptidmath::VectorOrientation::COLUMN || m_cols != vector_size)
        {
            throw std::invalid_argument(ERROR_SIZE_COL_MISMATCH);
        }
        Vector<Element,n_rows> result;
        for (size_t row = 0; row < n_rows; ++row)
        {
            for (size_t col = 0; col < vector_size; ++col)
            {
                result[row] += vector[col] * matrix[row][col];
            }
        }
        return result;
    }
// Arithmetic end

    // ostream& operator<< works for ostream but not print
    template<Arithmetic Element, size_t size>
    std::ostream& operator<<(std::ostream& stream, const Vector<Element,size>& vec)
    {
        vec.print(stream);
        return stream;
    }
}

// FORMATTER OVERLOAD
#include <string_view>
template <Arithmetic Element, size_t size>
struct std::formatter<cryptidmath::Vector<Element,size>> 
    : std::formatter<std::string_view>
{
    template <typename Context>
    auto format(const cryptidmath::Vector<Element,size>& vec, Context& ctx) const
    {
        std::string s = std::format("{} ",cryptidmath::VECTOR_BRACKET_OPEN);
        for (auto el = vec.cbegin(); el < vec.cend(); ++el)
        {
            if (el == vec.cend()-1)
            {
                s += std::format("{} ", *el);
            }
            else
            {
                s += std::format("{}, ", *el);
            }
        }
        s += std::format("{}",cryptidmath::VECTOR_BRACKET_CLOSE);
        if (vec.orientation() == cryptidmath::VectorOrientation::COLUMN)
            s += std::format("{}",cryptidmath::TRANSPOSE);
        return std::formatter<std::string_view>::format(s,ctx);
    }
};
#endif
