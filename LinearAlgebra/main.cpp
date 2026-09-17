//#define MATRIX_TEST
//#define DETERMINANT_TEST
#define ROW_OPERATIONS_TESTS

#include <array>
#include <ostream>
#include "my_concepts.hpp"
#include <iostream>
#include "Vector.h"
#include "Matrix.h"
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

using cryptidmath::Matrix, cryptidmath::Vector;

template<Arithmetic Element, size_t r, size_t c>
bool determinant_test(const Matrix<Element,r,c>& m, Element expected)
{
    std::print("{}",m);
    Element result = m.det();
    std::println("expected = {}, result = {}", expected, result);
    std::println("passed: {}", expected == result);
    return expected == result;
}

int main()
{
#ifdef ROW_OPERATIONS_TESTS
    Matrix<int,3,3> m{
        1, 2, 3,
        3, 2, 1,
        1, 0, 3
    };
    std::print("{}",m);
    m[1] *= 2;
    std::print("r2 * 2:\n{}",m);
    m[1] /= 2;
    std::print("r2 / 2:\n{}",m);
    m[0] += m[1];
    std::print("r1 + r2:\n{}",m);
    m[1] -= m[2];
    std::print("r2 + r3:\n{}",m);
#endif
#ifdef MATRIX_TEST
    cryptidmath::Matrix<int,2,3> m{1,2,3,4,5,6};
    std::cout << m << std::endl;
    std::println("{}",m);
    cryptidmath::Vector<int,3> v{1,2,3};
    std::println("{}",v);
    std::println("matrix*vector:\n{}*{}:\n{}",m,v,m*v);
    auto row2 = m.getRow(1);
    row2.set_orientation(cryptidmath::VectorOrientation::ROW);
    std::println("2nd row of matrix: {}", row2);
    std::println("2nd column of matrix: {}", m.getColumn(1));
    auto t_m = m.transpose();
    std::println("{}",t_m);
#endif

#ifdef DETERMINANT_TEST
    Matrix<int,1,1> m1{5};
    determinant_test(m1,5);
    Matrix<int,2,2> m2{1,0,0,1};
    determinant_test(m2,1);
    Matrix<int,3,3> m3{
        1, 2, 3,
        3, 2, 1,
        1, 0, 3
    };
    determinant_test(m3,-16);

#endif

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