#include "Vector.h"

namespace cryptidmath 
{

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
        ensure_ownership();
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
        for (size_t i = 0; i < lsize; i++)
        {
            ls[i] += rs[i];
        }
        return ls;
    }

    template<Arithmetic Element, size_t lsize, size_t rsize>
    Vector<Element,lsize> operator-(Vector<Element,lsize> ls,const Vector<Element,rsize>& rs)
    {
        compare_sz(lsize,rsize);
        for (size_t i = 0; i < lsize; i++)
        {
            ls[i] -= rs[i];
        }
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
        for (size_t i = 0; i < lsize; i++)
        {
            rs_vec[i] = k - rs_vec[i];
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
}