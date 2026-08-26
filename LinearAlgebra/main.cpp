#include <array>
#include <ostream>
#include "my_concepts.hpp"
#include <iostream>
#include "Vector.h"
#include <print>

template <Arithmetic Element,size_t sz>
void print_ar(const std::array<Element,sz>& ar)
{
    for (auto& e : ar){
        std::cout << e << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    cryptidmath::Vector<int,3> a(3);
    a.print();
    cryptidmath::Vector<int,3> b{1,2,3};
    b.print();
    std::println("{},{},{}",b[0],b[1],b[2]);
    a[0] = 0;
    a[1] = 2*a[1];
    a.print();

}