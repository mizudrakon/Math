#ifndef EXP_H
#define EXP_H

#include "ratio.h"
#include "my_concepts.hpp"

//EXP_NUM falls into EXPRESSION pit whenever we have different bases and irreducible expenents
//THIS IS A BIT MORE COMPLICATED...

//EXP_NUM CALSS DEFINITIONS:
/*
    -exp_num is meant to keep numbers as possibly primes with exponent for use in fractions
    I mostly want them to keep irrational numbers like sqr(2) 
    -like rational they keep two numbers, but the exponent is kept as rational, since it's 
    mostly intended to use for 0 < exp <= 1
*/
//I need to figure out if I need to force rational into the exponent here...
template <Arithmetic T>
class exp_num
{
    int base {0};
    rational<T> exponent {1};
public:
    //constructors
    exp_num() = delete;
    exp_num(int b = 0, rational<T> exp = 1) : base(b),exponent(exp){}
    exp_num(const exp_num& en) : base(en.base),exponent(en.exponent){}
    exp_num(exp_num&& en) noexcept : base(std::move(en.base)),exponent(std::move(en.exponent)){}

    //getters, setters:
    int GetBase() const { return base; }
    void SetBase(int x) { base = x; } 
    rational<T> GetExp() const { return exponent; }
    void SetExp(rational<T> r) { exponent = r; }
    
    //simplifiying fractions
    void Reduce();

    //getting a string representation
    std::string str() const;

};

template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T>& operator*(const exp_num<T>& a, const exp_num<T>& b);

//assuming this goes the same as with rational...

//op /

//op +

//op -

//formater for rational class to print it directly with print()
template<typename T>
struct std::formatter<exp_num<T>> : std::formatter<std::string> {
    template<typename Context>
    auto format(const exp_num& num, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", num.str());
    }
};

#endif