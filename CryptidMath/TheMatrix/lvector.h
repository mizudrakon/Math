#ifndef LVECTOR_H
#define LVECTOR_H

#include <array>
#include <memory>
#include <initializer_list>
#include <iostream>
#include <format>
#include "../my_concepts.hpp"
//#include "ratio.h"

template <Arithmetic T, size_t Length>
class lvector
{
private:
    std::shared_ptr<std::array<T,Length>> data_;

public:
    //lvector() = delete;
    lvector(const T& value = 0);
    lvector(std::initializer_list<T> ls);
    //lvector(const lvector<T,Length>& vec);

    //lvector(lvector&& vec):data(std::move(vec.data)),size(std::move(vec.size)){}
    
    ~lvector(){
        // ratio.h has a function print
        //print("vector_dtor\n");
        std::cout << "lvector dtor" << std::endl;
    }
    
    /*
    const Iterator auto begin() const { return data; }//first element
    const Iterator auto end() const { return data+size; }//one past

    T& operator[](size_t i) const {
        if (i < 0 || i > size - 1)
            throw std::out_of_range("Vector out of range error!");
        return data[i];
    }

    lvector& operator=(const lvector& vec){
        SizeTest(*this,vec);
        for (auto i = 0; i < size; ++i){
            data[i] = vec.data[i];
        }
        return *this;
    }
    lvector& operator+=(const lvector& vec){
        SizeTest(*this,vec);
        for (auto i = 0; i < size; ++i){
            data[i] += vec.data[i];
        }
        return *this;
    }
    lvector operator-(){
        //print("unary (-) called\n");
        lvector new_vec(size);
        auto data_it = data;
        for (auto it = new_vec.begin(); it < new_vec.end(); ++it, ++data_it){
            *it = 0 - *data_it;
        }
        return new_vec;
    }

    lvector& operator-=(const lvector&vec){
        *this += -vec;
        return *this;
    }
    */
    void Print() const {
        std::cout << "[ ";
        for (auto el : *data_){
            std::cout << std::format("{} ", el);
            //print("{} ", el);
        }
        //print("\n");
        std::cout << "]" << std::endl;
    }

};

template <Arithmetic T, size_t Length>
lvector<T,Length>::lvector(const T& value)
    :data_(std::make_shared<std::array<T,Length>>())
{
    data_->fill(value);
}

template <Arithmetic T, size_t Length>
lvector<T,Length>::lvector(std::initializer_list<T> ls)
    :data_(std::make_shared<std::array<T,Length>>())
{
    size_t i = 0;
    for (auto e : ls){
        if (i >= Length)
            break;
        (*data_)[i++] = e;
    }
}

/*
template <Arithmetic T, size_t Length>
lvector<T,Length>::lvector(const lvector<T,Length>& vec):data_()
{
    T* data_it = data;
    for (auto it = vec.begin(); it < vec.end(); ++it){
        *data_it = *it;
        ++data_it;
    }
}
*/
/*
template <Arithmetic T>
inline bool SameSize(const lvector<T>& a, const lvector<T>& b){
    return a.Size() == b.Size();
}

template <Arithmetic T>
inline void SizeTest(const lvector<T>& a, const lvector<T>& b){
    if (!SameSize(a,b))
        throw std::out_of_range("Comparing vectors of different size!\n");
}


template <Arithmetic T>
inline lvector<T> operator+(const lvector<T>& a, const lvector<T>& b){
    SizeTest(a,b);
    auto vec(a);
    for (size_t i = 0; i < vec.Size(); ++i){
        vec[i] += b[i];
    }
    return vec;
}

template <Arithmetic T>
inline lvector<T> operator-(const lvector<T>& a, const lvector<T>& b){
    SizeTest(a,b);
    auto vec(a);
    for (size_t i = 0; i < vec.Size(); ++i){
        vec[i] -= b[i];
    }
    return vec;
}

//dot product
template <Arithmetic T>
inline T dot(const lvector<T>&a, const lvector<T>& b){
    SizeTest(a,b);
    T answ = T(0);
    auto itb = b.begin();
    for (auto it = a.begin(); it < a.end(); ++it, ++itb)
        answ += (*it) * (*itb);
    return answ;
}
*/
#endif