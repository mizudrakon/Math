#ifndef LVECTOR_H
#define LVECTOR_H

#include <initializer_list>


template <typename T>
class lvector
{
    T* data;
    size_t size;

public:
    lvector() = delete;
    lvector(size_t sz):size(sz),data(new T[sz]){}
    lvector(std::initializer_list<T> ls){
        data = new T[ls.size()];
        size = 0;
        for (auto e : ls){
            data[size] = e;
            ++size;
        }
    }
    ~lvector(){delete[] data;}

    lvector& operator=(lvector vec){
        
    }
};


#endif