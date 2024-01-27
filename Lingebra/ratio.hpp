#pragma once

#include<format>
#include<string>
#include<tuple>
#include<cmath>

#include "my_concepts.hpp"

//class declarations
template <Arithmetic T> class rational;
template <Arithmetic T> class exp_num;

//print function
constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text, std::make_format_args(args...)).c_str(),stdout);
}

template <Arithmetic T>
//greatest common divisor... classic Eukleides
inline T gcd(T a, T b){ 
    while(true)
    {
        if (a < b) std::swap(a,b);
        if (b == 0) return a;
        a %= b;
    }
    return 1;
}

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

/*
    -rational is meant to keep numbers as fractions rather than doubles,
    but we can't just keep every number as a fraction recursively, and 
    we need to simplify the fractions wherever possible...
    -should I leave it with implicite cast to int, or make it explicite?...
*/

template <Arithmetic T>
class rational 
{
    T nom {0};
    T den {1};
public:
    rational() = delete;
    //default c_tor, implicit 0/1, one argument n gives us n/1
    rational(T n = 0, T d = 1):nom(n),den(d){}
    //copy c_tor
    rational(const rational& rn):nom(rn.nom),den(rn.den){}
    //move c_tor
    rational(rational&& rn) noexcept : nom(std::move(rn.nom)),den(std::move(rn.den)){}
    //copy_swap operator, pass by value makes a copy which is returned as the desired new object
    rational& operator=(rational rn)noexcept {
        std::swap(nom, rn.nom);
        std::swap(den, rn.den);
        return *this;
    }
    //we need member operators to allow *=, /=, %=
    rational& operator*=(const rational& frac){
        nom *= frac.nom;
        den *= frac.den;
        reduce();
        return *this;
    }
    rational& operator/=(const rational& frac){
        nom /= frac.nom;
        den /= frac.den;
        reduce();
        return *this;
    }
    rational& operator%=(rational frac){
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

    //d_tor doesn't really have anything special to do
    ~rational(){}

    //getters and setters for the two int numbers
    T nomin() const { return nom;}
    void set_nom(T n) { nom = n; }
    T denomin() const { return den; }
    void set_den(T d) { den = d; }
    //functions to get the number as a floating point rather than a fraction
    double dbl() const { return static_cast<double>(nom) / static_cast<double>(den);}
    float flt() const { return static_cast<float>(nom) / static_cast<float>(den);}
    
    //function for reducing the fraction
    void reduce();

    //getting it as string for printing
    std::string str() const{
        
        return std::to_string(nom) + "/" + std::to_string(den);
    }
};

template <Arithmetic T>
//just divide everything with greatest common denominator, which could be one therefore no change
inline void rational<T>::reduce(){
    T c = gcd(nom,den);
    nom /= c;
    den /= c;
}


template <Arithmetic T>
//spaceship operator for comparison
inline auto operator<=>(const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.denomin() == rhs.denomin()) return lhs.nomin() <=> rhs.nomin();
    auto a = lhs, b = rhs;
    a.reduce();
    b.reduce();
    auto [com_den, am, bm] = lcm(a.denomin(),b.denomin());
    
    return (a.nomin()*am) <=> (b.nomin()*bm);
};

template <Arithmetic T>
inline auto operator==(const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.nomin() == rhs.nomin() && lhs.denomin() == rhs.denomin())
        return true;
    auto a = lhs, b = rhs;
    a.reduce();
    b.reduce();
    return a.nomin() == b.nomin() && a.denomin() == b.denomin();
};

template <Arithmetic T>
inline auto operator* (const rational<T>& lhs, const rational<T>& rhs){
    rational new_r(lhs.nomin()*rhs.nomin(),lhs.denomin()*rhs.denomin());
    return new_r;
};


template <Arithmetic T>
inline auto operator/ (const rational<T>& lhs, const rational<T>& rhs){
    rational new_r(lhs.nomin()*rhs.denomin(),lhs.denomin()+rhs.nomin());
    return new_r;
};

template <Arithmetic T>
inline auto operator%(rational<T> lhs, rational<T> rhs){
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
inline auto operator+ (const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.denomin() != rhs.denomin()){
        lhs.reduce();
        rhs.reduce();
    }
    auto [com_denom,lhsm,rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm+rhs.nomin()*rhsm,com_denom);
    return result;
};

template <Arithmetic T>
inline auto operator- (const rational<T>& lhs, const rational<T>& rhs){
    if (lhs.denomin() != rhs.denomin()){
        lhs.reduce();
        rhs.reduce();
    }
    auto [com_denom, lhsm, rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm - rhs.nomin()*rhsm, com_denom);
    return result;
};

template <Arithmetic T>
//for negation with -
inline auto operator-(const rational<T>& frac){
    return 0-frac;
}

//formater for rational class to print it directly with print()
template<typename T>
struct std::formatter<rational<T>> : std::formatter<std::string> {
    template<typename Context>
    auto format(const rational& frac, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", frac.str());
    }
};

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
    exp_num(int b = 0, rational<T> exp = 1) : base(b),exponent(exp){}
    exp_num(const exp_num& en) : base(en.base),exponent(en.exponent){}
    exp_num(exp_num&& en) noexcept : base(std::move(en.base)),exponent(std::move(en.exponent)){}

    int Base() const { return base; }
    void SetBase(int x) { base = x; } 
    rational<T> Exp() const { return exponent; }
    void SetExp(rational<T> r) { exponent = r; }
    void Reduce() {
        if (exponent.denomin() == 1){
            base = int(pow(base, exponent.nomin()));
            exponent = 1;
        }
    }
};

template <Arithmetic T>
//multiplies only if a nad b are of the same base or their exps are 1
exp_num<T>& operator*(const exp_num<T>& a, const exp_num<T>& b){
    exp_num answ{0};
    //exponents = 1 just makes them regular integers
    if (a.Exp() == b.Exp() && a.Exp() == 1){
        answ.SetBase(a.Base()*b.Base());
    }
    //same base allows us to add exponents
    else if (a.Base() == b.Base()){
        answ.SetBase(a.Base());
        answ.SetExp(a.Exp() + b.Exp());
    }
    answ.Reduce(); 
    return answ;
}
//assuming this goes the same as with rational...

//op /

//op +

//op -
