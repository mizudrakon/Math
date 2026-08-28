#ifndef CRYPTID_VECTOR_H
#define CRYPTID_VECTOR_H

#include <array>
#include <memory>
#include <ostream>
#include <iostream>
#include <exception>

#include "my_concepts.hpp"

using std::size_t;

namespace cryptidmath
{

    constexpr const char *BAD_SIZE_MSG = "vector sizes do not match!";
    constexpr const char *BAD_INDEX_MSG = "index is out of range";
    constexpr const char    *SEPARATOR = ", ",
                            *TRANSPOSE = "\u1D40";//should be ^T
    constexpr const char *ALT_TRANSPOSE = "index is out of range";
    constexpr const char    VECTOR_BRACKET_OPEN = '(',
                            VECTOR_BRACKET_CLOSE = ')';

    enum class VectorOrientation 
    {
        COLUMN,
        ROW
    };

    template <Arithmetic Element, size_t size>
    class Vector 
    {
    private:
        std::shared_ptr<std::array<Element, size>> data_;
        VectorOrientation orientation_ = VectorOrientation::COLUMN;
        // default orientation is column, but we can optionally pass it to the constructor

    public:
        Vector(const Element& value = 0, VectorOrientation orientation = VectorOrientation::COLUMN);
        Vector(const std::initializer_list<Element> init_list);
        // Vector(const Matrix&, size_t row or column index)

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
            ensure_ownership();
            return get(index);
        }        
        const Element& operator[](size_t index) const
        {
            return get(index);
        }

        Vector& operator++();
        Vector operator++(int);
        Vector& operator--();
        Vector operator--(int);
        Vector operator-() const;
        Vector& operator*=(const Element& k);
        Vector& operator+=(const Element& k);
        Vector& operator-=(const Element& k);

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
        if (data_.use_count() > 1)
        {
            data_ = std::make_shared<std::array<Element,size>>(*data_);
        }
    }


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

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Element operator*(const Vector<Element,lsize>& ls,const Vector<Element,rsize>& rs);

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator+(Vector<Element,lsize> ls,const Vector<Element,rsize>& rs);

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator-(Vector<Element,lsize> ls,const Vector<Element,rsize>& rs);

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator*(Vector<Element,rsize> ls_vec, const Element& k);
    
    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator*(const Element& k, Vector<Element,rsize> rs_vec);

    template<Arithmetic Element, size_t lsize, size_t rsize>
    bool operator==(const Vector<Element,lsize>& ls,const Vector<Element,rsize>& rs);

    // ostream& operator<< works for ostream but not print
    template<Arithmetic Element, size_t size>
    std::ostream& operator<<(std::ostream& stream, const Vector<Element,size>& vec)
    {
        vec.print(stream);
        return stream;
    }
    // MAKE CONSTANT +* VECTOR
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
#include "Vector-Constructors.cpp"
#include "Vector-Operations.cpp"
#endif
