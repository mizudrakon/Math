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
    std::println("printing a: {}",a);
    cryptidmath::Vector<int,3> b{1,2,3};
    std::println("printing b: {}",b);
    std::println("{},{},{}",b[0],b[1],b[2]);
    a[0] = 0;
    a[1] = 2*a[1];
    std::println("{}",a);
    std::println("printing a++: {}", a++);
    std::println("printing a: {}",a);
    std::cout << "cout << a: " << a << std::endl;
    std::println("printing ++a: {}",++a);
    std::println("printing -a: {}",-a);

    std::println("Testing iterators:");
    std::println("- for (auto& e : a) loop:");
    for (auto& e : a)
    {
        std::print("{} ",e);
    }
    std::println();
    const cryptidmath::Vector<int,4> c{-1,2,-3,4};
    std::println("- for (auto e : const c) loop:");
    for (auto e : c)
    {
        std::print("{} ",e);
    }
    std::println();

    std::println("- for (auto e = c.cbegin(); e < c.cend(); e++) ) loop:");
    for (auto e = c.cbegin(); e < c.cend(); e++)
    {
        std::print("{} ",*e);
    }
    std::println(); 
    try{
        std::println("{}",a * c);
    }
    catch (const std::invalid_argument& e)
    {
        std::println("{}",e.what());
    }
    b = {1,2,3};
    std::println("printing new b: {}", b);
    std::println("{}*{} = {}",a,b,a * b);
    std::println("{} + {} = {}",a,b,a + b);
    a = b;
    std::println("{} == {}? {}",a,b, a == b);
    cryptidmath::Vector<int,3> z{};
    b = b + z;
    std::println("{} == {}+0? {}",a,b, a == b);
    auto b_copy = b;
    b *= 2;
    std::println("{} != {}", b, b_copy);
    

}