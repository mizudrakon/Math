
#include "MatrixVectorHierarchy.h"
#include <print>
#include <assert.h>

using namespace cryptidmath;

#define MATRIX_OP_TESTS
//#define PRINTING_TESTS

int main()
{
#ifdef MATRIX_OP_TESTS
    Matrix<int, 1, 3> rowv1{1,2,3};
    Matrix<int, 1, 3> rowv2{1,1,1};
    Matrix<int, 3, 1> colv1{1,2,3};
    Matrix<int, 3, 1> colv2{1,1,1};

    // ROW * COL -> dot product
    std::println("ROW * COL -> dot product:");
    std::println("{}*{}: {}?,{}",rowv1,colv1,14,(rowv1*colv1)[0][0]==14);
    //rowv1 *= colv1;

    // COL * ROW -> MATRIX
    std::println("COL * ROW -> MATRIX");
    std::println("{}*{}: {}",colv2,rowv2,colv2*rowv2);
    //rowv2 *= colv2;
    std::println("Working on a matrix:"); 
    auto m = colv1*rowv1;
    std::println("{}",m);
    std::println("Swapping row 0 and 2");
    m.swap_row(0,2);
    std::println("{}",m);
    std::println("m[0]-3*m[2]");
    m[0] -= 3*m[2];
    std::println("{}",m);
    m[0] += 5*m[2];
    std::println("m[0]+5*m[2]\n{}",m);
    std::println("getting row matrix from m[0] - m[1]");
    auto mr = m[0] - m[1]; 
    std::println("{}",mr);
    m[2] += m[0] - m[1];
    std::println("m[2]+m[0]-m[1]\n{}",m);
#endif 
#ifdef PRINTING_TESTS
    std::println("Basic print with no arguments:");
    Matrix<int,2,2> m1={1,2,3,4};
    m1.print();
    std::println();
    Matrix<int,1,3> rowM = {1,2,3};
    Matrix<int,3,1> colM = {1,2,3};
    Matrix<int,3,3> sqrM = {1,2,3, 4,5,6, 7,8,9};
    Matrix<int,13,3> hightM = {1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11, 10,11,12, 11,12,13, 12,13,14, 13,14,15};

    //rowM.print(std::cout,true);
    std::println("{}",rowM);
    //colM.print(std::cout,true);
    std::println("{}",colM);
    sqrM.print(std::cout,true,true);
    std::println("\n{}",sqrM);
    hightM.print(std::cout,true,true);
    std::println();
#endif
}