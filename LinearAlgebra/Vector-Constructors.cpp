#include "Vector.h"

namespace cryptidmath 
{
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
}