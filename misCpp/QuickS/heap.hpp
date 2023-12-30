#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <concepts>


template <class C>
concept Container = requires(C lst){
    lst.begin();
    lst.end();
    lst[0];
};

template <typename T, Container C>
class heap
{
    const size_t size; //counts elements
    const T EMPTY; //marks empty position
    const bool max;
    std::vector<T> data; //container for the heap
    

public:
    heap():size(0),EMPTY(T(0)),max(true);
    heap(const vector<T>& vec, bool mx, T limit): size(sz),max(mx),EMPTY(limit){}
    ~heap();
};

#endif