#ifndef CRYPTID_VECTOR_H
#define CRYPTID_VECTOR_H

#include <array>
#include <memory>
#include <iostream>

#include "my_concepts.hpp"

using std::size_t;

namespace cryptidmath
{
    template <Arithmetic Element, size_t size>
    class Vector 
    {
    private:
        std::shared_ptr<std::array<Element, size>> data_;

    public:
        Vector(const Element& value = 0);
        Vector(const std::initializer_list<Element> init_list);

        void print() const
        {
            for (auto& el : (*data_))
            {
                std::cout << el << ' ';
            }
            std::cout << std::endl;
        }
    };


    template <Arithmetic Element, size_t size>
    Vector<Element,size>::Vector(const Element& value)
        :data_(std::make_shared<std::array<Element,size>>())
    {
        data_->fill(value);
    }

    template <Arithmetic Element, size_t size>
    Vector<Element,size>::Vector(const std::initializer_list<Element> init_list)
        :data_(std::make_shared<std::array<Element,size>>())
    {
        if (init_list.size() > size)
        {
            throw "out of range";
        }
        int i = 0;
        for (auto& el : init_list)
        {
            (*data_)[i++] = el;
        }
    }
}




#endif