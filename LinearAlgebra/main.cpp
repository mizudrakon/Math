#define OPERATION_TEST

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

#ifdef OWNERSHIP_TEST
void print_eo_count()
{
    std::println("ensure ownership called: {}", cryptidmath::eo_counter);
    cryptidmath::eo_counter = 0;
}
#endif


int main()
{
#ifdef OPERATION_TEST
    cryptidmath::Vector<int,3> a(3);
    std::println("printing a: {}",a);
    cryptidmath::Vector<int,3> b{1,2,3};
    std::println("printing b: {}",b);
    std::println("b[0]:{},b[1]:{},b[2]:{}",b[0],b[1],b[2]);
    std::println("a[0]=0");
    a[0] = 0;
    std::println("a[1]=2*a[1]");
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
        std::println("{}*{}",a,c);
        std::println("{}",a * c);
    }
    catch (const std::invalid_argument& e)
    {
        std::println("{}",e.what());
        std::println("And that is how it should be.");
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
    std::println("{} != {}", b, b_copy);
    b *= 2;
    std::println("b *= 2: {}", b);
    b += 3;
    std::println("b += 3: {}", b);
    std::print("b:{} *= a:{}: ", b, a);
    b *= a;
    std::println("b:{}",b);
    b -= a;
    std::println("b -= a: {}",b);
#endif

#ifdef OWNERSHIP_TEST
    cryptidmath::Vector<int,3> a{1,2,3};
    print_eo_count();

    std::println("println using []: {}",a[0]);
    print_eo_count();

    std::cout << "std::cout<< []:" << a[0] << std::endl;
    print_eo_count();

    std::println("iterator: {}",*a.begin());
    print_eo_count();

    const int q = a[1];
    std::println("assigning a const q = a[1] and printing q:{}",q);
    print_eo_count();

    std::println("printing a: {}",a);
    print_eo_count();

    std::println("cout << a: {}",a);
    std::cout << a << std::endl;
    print_eo_count();
    
    std::println("++a: {}",++a);
    print_eo_count();
    std::println("a++: {}",a++);
    print_eo_count();

    std::println("assignment: ");
    auto b = a;
    print_eo_count();
    std::println("b++: {}", b++);
    print_eo_count();
    std::println("a+b: {}",a+b);
    print_eo_count();

    const cryptidmath::Vector<int,3> c{1,2,3};
    std::println("constant vector print: {},{},{}",c[0],c[1],c[2]);
    print_eo_count();
    std::println("dot {}*{}: {}",a,b,a*b);
    print_eo_count();


#endif
}