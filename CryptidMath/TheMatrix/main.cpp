#include "lvector.h"

int main(void)
{
    std::cout << "array test:" << std::endl; 
    std::array<int,5> a{1,2,3,4,5};
    for (auto i = 0; i < 5; i++)
    {
        std::cout << std::format("{} ",a[i]);
    }
    std::cout << std::endl;
    
    std::cout << "vector test:" << std::endl; 
    lvector<int,5> vfive(1);
    vfive.Print();
    lvector<int,5> afive{1,2,3,4,5};
    afive.Print();
    lvector<int,5> mfive(std::move(vfive));
    mfive.Print();
    lvector<int,5> efive;
    efive.Print();
}