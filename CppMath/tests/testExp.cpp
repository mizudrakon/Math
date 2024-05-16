#include "../exp_num.h"


int main(int args, char* argv[])
{
    using expn = exp_num<int>;
    using frac = rational<int>;

    //test constraints for Arithmetic T:
    expn a{4};
    expn b{2};
    print("a = {}, b = {}\n", a, b);
    print("a + b = {}\n",a + b);
    print("a - b = {}\n", a - b);
    print("a * b = {}\n", a * b);
    expn r = a * b;
    print("{} * {} = {}\n", a, b, r);
    print("a / b = {}\n", a / b);
    expn rd = a / b;
    print("{} / {} = {}\n", a, b, rd);
    print("a % b = {}\n", a % b);
    print("a == b is {}\n", a == b);
    print("a < b is {}\n",a < b);

#ifdef OPEN_TEST    
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
    //+
    print("add {} += {}:\n", d, e);
    if (d.add(e)) 
        print("successful add: {}\n", d);
    else
        print("cannot add: {} + {}\n", d, e);

    //-
    print("add {} -= {}:\n", d, e);
    if (d.sub(e)) 
        print("successful sub: {}\n", d);
    else
        print("cannot sub: {} - {}\n", d, e);

    //*
    print("multiply {} *= {}:\n", d, e);
    if (d.mult_by(e)) 
        print("successful mult: {}\n", d);
    else
        print("cannot multiply {} with {}\n", d, e);

    print("multiply {} *= {}:\n", b, c);
    if (d.mult_by(c)) 
        print("successful mult: {}\n", b);
    else
        print("cannot multiply {} with {}\n", b, c);

    //div
    print("divide {} *= {}:\n", d, e);
    if (d.div_by(e)) 
        print("successful div: {}\n", d);
    else
        print("cannot divide {} with {}\n", d, e);

    print("divide {} /= {}:\n", b, c);
    if (d.div_by(c)) 
        print("successful div: {}\n", b);
    else
        print("cannot divide {} with {}\n", b, c);
#endif



}