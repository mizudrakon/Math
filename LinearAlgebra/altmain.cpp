#include "altMatrix.h"
#include <print>

using namespace cryptidmath;

int main()
{
    Matrix<int, 1, 3> rowv1{1,2,3};
    Matrix<int, 1, 3> rowv2{1,1,1};
    Matrix<int, 3, 1> colv1{1,2,3};
    Matrix<int, 3, 1> colv2{1,1,1};

    std::println("{}*{}: {}",rowv1,colv1,rowv1*colv1);
    std::println("{}*{}: {}",colv1,rowv1,colv1*rowv1);
}