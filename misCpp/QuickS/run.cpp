#include "quick.hpp"
#include <vector>
#include <iostream>


int main(int argc, char** argv)
{
    std::vector<double> vec{};
    
    for (double d; std::cin >> d;){
        vec.push_back(d);
    }
    
    Quick::print_vec(vec);
    Quick::sort(vec.begin(),vec.end(),[](double x, double y){ return x < y;});
    Quick::print_vec(vec);
}