#include "lvector.hpp"
#include "ratio.hpp"

int main(){
    lvector a {1,2,3,4,5,6};
    a.Print();
    lvector na = -a;
    print("a, na:\n");
    a.Print();
    na.Print();
    lvector b {3,1,6,5,7,4};
    b.Print();
    a += b;
    a.Print();
    lvector c = a + b;
    c.Print();
}