#ifndef HEAP_H
#define HEAP_H

#include <array>

template <typename T, const int n>
class heap
{
    std::array<T,n> data;
public:
    heap();
    ~heap();
};

#endif