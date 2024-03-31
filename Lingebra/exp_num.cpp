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
double exp_num<T>::to_dbl() {
    return pow(static_cast<double>(base),exponent.to_dbl());
}

template <Arithmetic T>
std::string exp_num<T>::str() const {
    if (exponent == 0) return "1"; //reduce to 1
    std::string text = std::to_string(base); 
    if (exponent == 1) return text;//no need to print exponent
    return text + "^{" + exponent.str() + '}';//also print exponent, if it's nontrivial
}

template <Arithmetic T>
bool operator<=>(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //simple cases:
    if (lhs.GetBase() == rhs.GetBase()) 
        return lhs.GetExp() <=> rhs.GetExp();
    else if (lhs.GetExp() == rhs.GetExp() || 
    ((lhs.GetBase() <=> rhs.GetBase()) == (lhs.GetExp() <=> rhs.GetExp())))
        return lhs.GetBase() <=> rhs.GetBase();
    //cases like lhs.base < rhs.base && lhs.exp > rhs.exp:
    return lhs.to_dbl() <=> rhs.to_dbl();
}

template <Arithmetic T>
exp_num<T>& operator+(const exp_num<T>& lhs, const exp_num<T>& rhs){
    exp_num answ{0};
    if (lhs.GetExp() == 1 && rhs.GetExp() == 1)

}

template <Arithmetic T>
exp_num<T>& operator-(const exp_num<T>& a, const exp_num<T>& b){}


template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T>& operator*(const exp_num<T>& a, const exp_num<T>& b){
    exp_num answ{0};
    //exponents = 1 just makes them regular integers
    if (a.GetExp() = 1 && b.GetExp() == 1){
        answ.SetBase(a.GetBase()*b.GetBase());
    }
    //same base allows us to add exponents
    else if (a.GetBase() == b.GetBase()){
        answ.SetBase(a.GetBase());
        answ.SetExp(a.GetExp() + b.GetExp());
    }
    answ.Reduce(); 
    return answ;
}

template <Arithmetic T>
exp_num<T>& operator/(const exp_num<T>& a, const exp_num<T>& b){
    exp_num answ{0};
    //exponents = 1 just makes them regular integers
    if (a.GetExp() = 1 && b.GetExp() == 1){
        answ.SetBase(a.GetBase()*b.GetBase());
    }
    //same base allows us to add exponents
    else if (a.GetBase() == b.GetBase()){
        answ.SetBase(a.GetBase());
        answ.SetExp(a.GetExp() - b.GetExp());
    }
    answ.Reduce(); 
    return answ;

}