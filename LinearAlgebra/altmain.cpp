#include "altMatrix.h"
#include <print>

using namespace cryptidmath;

int main()
{
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
    m.swap_row(0,2);
    m[1] -= 3*m[2];
    std::println("{}",m);


// we need print row and operations with rows

}