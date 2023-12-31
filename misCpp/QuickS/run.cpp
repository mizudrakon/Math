#include "quick.hpp"
#include <iostream>

//testing file
int main(int argc, char** argv)
{
    using std::cout;
    std::vector<double> vec{};
    constexpr size_t k = 6;

    for (double d; std::cin >> d;){
        vec.push_back(d);
    }
    auto f = [](double x, double y){ return x < y;};
    quick::print_vec(vec);
    auto itvec = quick::copy_to_vec<double>(vec.begin(),vec.end(),10);
    quick::sort(itvec.begin(),itvec.end(),f);
    quick::print_vec(itvec);
    auto ksel = quick::select<double>(k, vec.begin(), vec.end(),f);
    auto kth = quick::trivial_select<double>(k,vec.begin(),vec.end(),f); 
    auto five = quick::five_median<double>(vec.begin(),vec.begin()+5,f);
    print("{}rd member is {}\n", k, ksel);
}