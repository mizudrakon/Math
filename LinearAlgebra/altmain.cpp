//#include "altMatrix.h"
#include "MatrixVectorHierarchy.h"
#include <print>

using namespace cryptidmath;

#define PRINTING_TESTS
int main()
{
    
#ifdef OP_TESTS
    Matrix<int, 1, 3> rowv1{1,2,3};
    Matrix<int, 1, 3> rowv2{1,1,1};
    Matrix<int, 3, 1> colv1{1,2,3};
    Matrix<int, 3, 1> colv2{1,1,1};

    // ROW * COL -> dot product
    std::println("{}*{}: {}",rowv1,colv1,rowv1*colv1);
    //rowv1 *= colv1;

    // COL * ROW -> MATRIX
    std::println("{}*{}: {}",colv2,rowv2,colv2*rowv2);
    //rowv2 *= colv2;
    std::println("Working on a matrix:"); 
    auto m = colv1*rowv1;
    std::println("{}",m);
//    std::println("{}",m[1]);
    std::println("Swapping row 0 and 2\n{}",m);
    m.swap_row(0,2);
    std::println("m[0]-3*m[2]");
    m[0] -= 3*m[2];
    std::println("{}",m);
    m[0] += 5*m[2];
    std::println("m[0]+5*m[2]\n{}",m);
    auto mr = m[0] - m[1]; 
    std::println("separate test done:{}",mr);
    m[2] += m[0] - m[1];
    std::println("m[2]+m[0]+m[1]\n{}",m);
#endif 
#ifdef PRINTING_TESTS
    Matrix<int,1,3> rowM = {1,2,3};
    Matrix<int,3,1> colM = {1,2,3};
    Matrix<int,3,3> sqrM = {1,2,3, 4,5,6, 7,8,9};

    rowM.print(std::cout,true);
    std::println();
    colM.print(std::cout,true);
    std::println();
    sqrM.print(std::cout,true);
    std::println();
#endif
    

// we need print row and operations with rows

}