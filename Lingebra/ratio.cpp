#include "ratio.h"
#include "my_concepts.hpp"

template <Arithmetic T>
//greatest common divisor... classic Eukleides
inline T gcd(T a, T b){ 
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while(true)
    {
        if (a < b) std::swap(a,b);
        if (b == 0) return a;
        a %= b;
    }
    return 1;
}

enum class lcm_return { multiple, a_multiplier, b_multiplier};

template <Arithmetic T>
//lowest common multiple returing a tuple with the required multiplier to a and b
inline std::tuple<T,T,T> lcm(T a, T b){ 
    T x = a,
        y = b,
        am = 1,
        bm = 1;
    while (a != b)
    {
        if (a < b) {a += x; ++am;}
        else {b += y; ++bm;}
    }
    
    return std::make_tuple(a,am,bm);
}

template <Arithmetic T>
constexpr rational<T> make_rational(T n, T d)    
{
    print("making a {}/{} rational\n", n, d);
    rational<T> num{n,d};
    return num;
}

//MEMBER OPERATION OVERLOADS
template <Arithmetic T>
rational<T>& rational<T>::operator=(rational<T> rn)noexcept {
    std::swap(nom, rn.nom);
    std::swap(den, rn.den);
    return *this;
}

template <Arithmetic T>
rational<T>& rational<T>::operator+=(const rational& frac){
    //if (den != frac.den){
    //    reduce();
        //frac.reduce();
    //}
    auto [com_denom,lhsm,rhsm] = lcm(den,frac.den);
    nom = nom*lhsm+frac.nom*rhsm;
    den = com_denom;
    reduce();
    return *this;
}

template <Arithmetic T>
rational<T>& rational<T>::operator-=(const rational& frac){
    auto [com_denom,lhsm,rhsm] = lcm(den,frac.den);
    nom = nom*lhsm-frac.nom*rhsm;
    den = com_denom;
    reduce();
    return *this;
}

template <Arithmetic T>
rational<T>& rational<T>::operator*=(const rational& frac){
    nom *= frac.nom;
    den *= frac.den;
    if (den != 1) reduce();
    return *this;
}
template <Arithmetic T>
rational<T>& rational<T>::operator/=(const rational& frac){
    nom /= frac.nom;
    den /= frac.den;
    if (den != 1) reduce();
    return *this;
}
template <Arithmetic T>
rational<T>& rational<T>::operator%=(rational frac){
//I want to allow remainder from fraction division too
    reduce(); //our initial fraction will be switched for the result, so we can just reduce it
    frac.reduce(); //frac is a copy of some rational argument which itself doesn't get reduced
    auto [com_den, m, rm] = lcm(den,frac.den); //get lcm and multipliers for the denominators
    //result should be: (nom*m % frac.nom*rm)/lcm
    //we can switch them with the original fraction data with modified parameter data
    frac.set_den(com_den);
    frac.set_nom((nom*m)%(frac.nom*rm));
    std::swap(*this,frac);
    return *this;
}

//INCREMENTS
template <Arithmetic T>
rational<T>& rational<T>::operator++(){
    nom += den;
    reduce();
    return *this;
}


template <Arithmetic T>
rational<T> rational<T>::operator++(int){
    auto old = *this;
    operator++();
    return old;
}

template <Arithmetic T>
rational<T>& rational<T>::operator--(){
    nom -= den;
    reduce();
    return *this;
}

template <Arithmetic T>
rational<T> rational<T>::operator--(int){
    auto old = *this;
    operator--();
    return old;
}

template <Arithmetic T>
void rational<T>::reduce(){
    //we wnat to move a (-) sign to nom (or make two - into +)
    if (den < 0){
        nom *= -1;
        den *= -1;
    }
    bool ng = (nom < 0) ? true : false;
    if (ng) nom *= -1;
    T c = gcd(nom,den);
    nom /= c;
    den /= c;
    if (ng) nom *= -1;
}

//getting it as string for printing
/*
    * We want x/1 to be printed as simple x, 
    * and x/x as 1 (which should be already kept as 1/1)
    * 0/anything should just be 0
    * any farc < 0 should be written as (-p/q)
*/
template <Arithmetic T>
std::string rational<T>::str() const{
    if (nom == den) return "1";
    if (nom == 0) return "0";
    if (den == 1) return std::to_string(nom);
    std::string answ {};
    if (nom < 0) answ += "(";
    answ += std::to_string(nom);
    if (den != 1) answ += "/" + std::to_string(den);
    if (nom < 0) answ += ")";
    return answ;
}



//NONMEMBER OPERATIONS:

template <Arithmetic T>
//spaceship operator for comparison
auto operator<=>(const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.denomin() == rhs.denomin()) return lhs.nomin() <=> rhs.nomin();
    auto a = lhs, b = rhs;
    //a.reduce();
    //b.reduce();
    auto [com_den, am, bm] = lcm(a.denomin(),b.denomin());
    
    return (a.nomin()*am) <=> (b.nomin()*bm);
};

template <Arithmetic T>
auto operator==(const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.nomin() == rhs.nomin() && lhs.denomin() == rhs.denomin())
        return true;
    auto a = lhs, b = rhs;
    a.reduce();
    b.reduce();
    return a.nomin() == b.nomin() && a.denomin() == b.denomin();
};

template <Arithmetic T>
auto operator* (const rational<T>& lhs, const rational<T>& rhs){
    rational new_r(lhs.nomin()*rhs.nomin(),lhs.denomin()*rhs.denomin());
    return new_r;
};


template <Arithmetic T>
auto operator/ (const rational<T>& lhs, const rational<T>& rhs){
    rational new_r(lhs.nomin()*rhs.denomin(),lhs.denomin()+rhs.nomin());
    return new_r;
};

template <Arithmetic T>
auto operator%(rational<T> lhs, rational<T> rhs){
    if (lhs < rhs) return lhs;
    rational result(0);
    if (lhs == rhs) return result;
    lhs.reduce();
    rhs.reduce();
    auto [com_den, lhsm, rhsm] = lcm(lhs.denomin(),rhs.denomin());
    result.set_nom((lhs.nomin()*lhsm) % (rhs.nomin()*rhsm));
    result.set_den(com_den);
    return result;
}

template <Arithmetic T>
auto operator+ (const rational<T>& lhs, const rational<T>& rhs){
    auto [com_denom,lhsm,rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm+rhs.nomin()*rhsm,com_denom);
    result.reduce();
    return result;
};

template <Arithmetic T>
auto operator- (const rational<T>& lhs, const rational<T>& rhs){
    auto [com_denom, lhsm, rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm - rhs.nomin()*rhsm, com_denom);
    result.reduce();
    return result;
};

template <Arithmetic T>
//for negation with -
auto operator-(const rational<T>& frac){
    return 0-frac;
}