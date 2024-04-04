#include "../exp_num.h"


int main(int args, char* argv[])
{
    using expn = exp_num<int>;
    using frac = rational<int>;
    expn df;
    expn num{2,3};
    print("default constructor: {}, 2^3: {}\n", df, num);
    expn a{2,6};
    expn b{1,3};
    auto fra = make_rational<int>(3,4);
    expn c{5,fra};
    expn d{4};
    expn e{5};
    print("complicated exponent: {}\n", c);
    if (d.add(e)) 
        print("successful add: {}\n", d);
    else
        print("cannot add: {} + {}\n", d, e);

}