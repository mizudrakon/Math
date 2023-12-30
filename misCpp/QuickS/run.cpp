#include "quick.hpp"
//#include <vector>
#include <iostream>


int main(int argc, char** argv)
{
    using std::cout;
    std::vector<double> vec{};
    
    for (double d; std::cin >> d;){
        vec.push_back(d);
    }
    auto f = [](double x, double y){ return x < y;};
    quick::print_vec(vec);
    //Quick::sort(vec.begin(),vec.end(),[](double x, double y){ return x < y;});
    //quick::select(3, vec.begin(), vec.end(),[](double x, double y){ return x < y;});
    auto itvec = quick::copy_to_vec<double>(vec.begin(),vec.end(),10);
    auto kth = quick::trivial_select<double>(3,vec.begin(),vec.end(),f); 
    auto five = quick::five_median<double>(vec.begin(),vec.begin()+5,f);
    print("{} median of five: {}\n", kth, five);
    cout << kth << " median of five: " << five << '\n';
    quick::print_vec(itvec.begin(), itvec.end());
}