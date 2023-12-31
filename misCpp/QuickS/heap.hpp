#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <concepts>
#include <memory>
#include "my_concepts.hpp"

template <typename T>
auto less = [](T x, T y){ return x < y; };
template <typename T, Container C>
class heap
{
    const size_t size; //counts elements
    std::vector<std::unique_ptr<T>> data; //container for the heap
    

public:
    using std::swap;
    heap():size(0){}
    //create a heap from a range
    heap(const Iterator auto& start, const Iterator auto& end, bool f = less<T>): size(end - start)
    {
        //copy the original vector's data
        for (auto it = start; it < end; ++it)
        {
            data.push_back(std::make_unique<T>(*it));
        }

        for (size_t i = data.size()/2; i > 0; --i)
        {
            bubble_down(i,f);
        }
    }
    ~heap(){}

    //This is basic heap logic, but I need to add basic operations like push(), pop()... 

    size_t lson(size_t i){
        return 2*i+1;
    }
    size_t rson(size_t i){
        return 2*i+2;
    }
    size_t parent(size_t i){
        return (i-1)/2;
    }

    void bubble_down(size_t i, bool f){
        while (i*2 < size){
            if (f(*data[lson(i)],*data[rson(i)])){
                if (f(*data[lson(i)],*data[i]))
                    swap(data[i],data[lson(i)]);
                    i = lson(i);
                    continue;
            } 
            else if (f(*data[rson(i)],*data[i])){
                swap(data[i],data[rson(i)]);
                i = rson(i);
                continue;
            }
            break;
        }
    }

    void bubble_up(size_t i, bool f){
        while (i > 0 && f(*data[i],*data[parent(i)])){
            std::swap(data[i],data[parent(i)]);
            i = parent(i);
        }
    }
};

#endif