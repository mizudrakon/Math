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
double exp_num<T>::to_dbl() const {
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
auto exp_num<T>::operator+=(const exp_num<T>& rhs){
    base += rhs.base;
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator-=(const exp_num<T>& rhs){
    base -= rhs.base;
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator*=(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        base *= rhs.base;
    }
    //same base allows us to add exponents
    else if (base == rhs.base){
        exponent += rhs.exponent;
    }
    Reduce();
    return *this;
} 

template <Arithmetic T>
auto exp_num<T>::operator/=(const exp_num<T>& rhs){
    if (base == rhs.base){
        exponent -= rhs.exponent;
    }
    //exponents = 1 just makes them regular integers
    else if (exponent == 1 && rhs.exponent == 1){
        base /= rhs.base;
    }
    Reduce(); 
    return *this;
} 

template <Arithmetic T>
bool exp_num<T>::add(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        *this += rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::sub(const exp_num<T>& rhs){
    if (exponent == 1 && rhs.exponent == 1){
        *this -= rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::mult_by(const exp_num<T>& rhs){
    
    //print("this exponent: {}, rhs exponent: {}\n", exponent, rhs.exponent);
    //if (exponent == 1) print("this exp == 1\n");
    //if (rhs.exponent == 1) print("rhs exp == 1\n");

    if (base == rhs.base || exponent == 1 && rhs.exponent == 1){
        *this *= rhs;
        return true;
    }
    return false;
}

template <Arithmetic T>
bool exp_num<T>::div_by(const exp_num<T>& rhs){
    
    if ((base == rhs.GetBase()) || (exponent == 1 && rhs.GetExp() == 1 && base % rhs.GetBase() == 0)){
        *this /= rhs;
        return true;
    }
    return false;
}



template <Arithmetic T>
auto operator<=>(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //simple cases:
    if (lhs.GetBase() == rhs.GetBase()) 
        return lhs.GetExp().to_dbl() <=> rhs.GetExp().to_dbl();
    else if (lhs.GetExp() == rhs.GetExp() || 
    ((lhs.GetBase() <=> rhs.GetBase()) == (lhs.GetExp() <=> rhs.GetExp())))
        return static_cast<double>(lhs.GetBase()) <=> static_cast<double>(rhs.GetBase());
    //cases like lhs.base < rhs.base && lhs.exp > rhs.exp:
    return lhs.to_dbl() <=> rhs.to_dbl();
}

template <Arithmetic T>
auto operator<=>(const exp_num<T>& lhs, const T& rhs){
    if (lhs.GetExp() == 1) return lhs.GetBase() <=> rhs;
    return lhs.to_dbl() <=> static_cast<double>(rhs);
}

template <Arithmetic T>
auto operator==(const exp_num<T>& lhs, const exp_num<T>& rhs){
    //some cases that might save time since to_dbl calls pow()
    if (lhs.GetBase() == rhs.GetBase()) return lhs.GetExp() == rhs.GetExp();
    if (lhs.GetExp() == rhs.GetExp()) return lhs.GetBase() == rhs.GetBase();
    if (lhs.GetBase() < rhs.GetBase() && lhs.GetExp() < rhs.GetExp()) return false;
    if (lhs.GetBase() > rhs.GetBase() && lhs.GetExp() > rhs.GetExp()) return false;
    //have to call pow()
    return lhs.to_dbl() == rhs.to_dbl();
}

template <Arithmetic T>
auto operator==(const exp_num<T>& lhs, const T& rhs){
    if (lhs.GetExp() == 1) return lhs.GetBase() == rhs;
    if (lhs.GetExp() >= 1 && lhs.GetBase() > rhs) return false;
    return lhs.to_dbl() == static_cast<double>(rhs);
}

template <Arithmetic T>
exp_num<T> operator+(const exp_num<T>& lhs, const exp_num<T>& rhs){
    return exp_num<T>(lhs.GetBase()+rhs.GetBase(),1);
}

template <Arithmetic T>
exp_num<T> operator-(const exp_num<T>& lhs, const exp_num<T>& rhs){
    return exp_num<T>{lhs.GetBase()-rhs.GetBase(),1};
}

template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T> operator*(const exp_num<T>& lhs, const exp_num<T>& rhs){
    auto answ = std::make_unique<exp_num<T>>(0);
    //exponents = 1 just makes them regular integers
    if (lhs.GetExp() == 1 && rhs.GetExp() == 1){
        answ->SetBase(lhs.GetBase()*rhs.GetBase());
    }
    //same base allows us to add exponents
    else if (lhs.GetBase() == rhs.GetBase()){
        answ->SetBase(lhs.GetBase());
        answ->SetExp(lhs.GetExp() + rhs.GetExp());
    }
    answ->Reduce(); 
    return *answ;
}

template <Arithmetic T>
exp_num<T> operator/(const exp_num<T>& lhs, const exp_num<T>& rhs){
    exp_num<T> answ{0};
    //same base allows us to add exponents
    if (lhs.GetBase() == rhs.GetBase()){
        answ.SetBase(lhs.GetBase());
        answ.SetExp(lhs.GetExp() - rhs.GetExp());
    }
    //exponents = 1 just makes them regular integers
    else if (lhs.GetExp() == 1 && rhs.GetExp() == 1){
        answ.SetBase(lhs.GetBase()/rhs.GetBase());
    }
    answ.Reduce(); 
    return answ;

}

template <Arithmetic T>
exp_num<T> operator%(const exp_num<T>& lhs, const exp_num<T>& rhs){
    exp_num<T> answ;
    return answ;
}