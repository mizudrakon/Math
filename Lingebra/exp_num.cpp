#include "exp_num.h"

template <Arithmetic T>
void exp_num<T>::Reduce() {
    exponent.reduce();
    if (exponent.denomin() == 1){
        base = int(pow(base, exponent.nomin()));
        exponent = 1;
    }
}

template <Arithmetic T>
std::string exp_num<T>::str() const {
    if (exponent == 0) return "1"; //reduce to 1
    std::string text = std::to_string(base); 
    if (exponent == 1) return text;//no need to print exponent
    return text + "^{" + exponent.str() + '}';//also print exponent, if it's nontrivial
}


template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T>& operator*(const exp_num<T>& a, const exp_num<T>& b){
    if (a.exponent > )
    exp_num answ{0};
    //exponents = 1 just makes them regular integers
    if (a.GetExp() == b.GetExp() && a.GetExp() == 1){
        answ.SetBase(a.GetBase()*b.GetBase());
    }
    //same base allows us to add exponents
    else if (a.GetBase() == b.GetBase()){
        answ.SetBase(a.GetBase());
        answ.SetExp(a.GetExp() + b.GetExp());
    }
    else {
        
    }
    answ.Reduce(); 
    return answ;
}