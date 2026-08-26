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

    constexpr const char *BAD_SIZE_MSG_ = "sizes do not match";
    constexpr const char *BAD_INDEX_MSG_ = "index is out of range";
    constexpr const char    *SEPARATOR = ", ",
                            *TRANSPOSE = "^T";
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

    private:
        void ensure_ownership();
    };


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
            throw std::length_error(BAD_SIZE_MSG_);
        }
        int i = 0;
        for (auto& el : init_list)
        {
            (*data_)[i++] = el;
        }
    }

    template <Arithmetic Element, size_t size>
    inline Element& Vector<Element,size>::get(size_t index)
    {
        if (index >= size)
        {
            throw std::out_of_range(BAD_INDEX_MSG_);
        }
        ensure_ownership();
        return (*data_)[index];
    }
    
    template <Arithmetic Element, size_t size>
    inline const Element& Vector<Element,size>::get(size_t index) const
    {
        if (index >= size)
        {
            throw std::out_of_range(BAD_INDEX_MSG_);
        }
        return (*data_)[index];
    }
    
    template <Arithmetic Element, size_t size>
    inline void Vector<Element,size>::set(size_t index, const Element& value)
    {
        ensure_ownership();
        (*data_)[index] = value;
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
            << ((orientation_ == VectorOrientation::COLUMN) ? TRANSPOSE : "") 
            << std::endl;
    }


}
#endif