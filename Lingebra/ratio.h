#ifndef RATIO_H
#define RATIO_H

#include<format>
#include<string>
#include<tuple>
#include<cmath>
#include<iostream>

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
inline T gcd(T a, T b); 

//enum class lcm_return { multiple, a_multiplier, b_multiplier};

template <Arithmetic T>
//lowest common multiple returing a tuple with the required multiplier to a and b
inline std::tuple<T,T,T> lcm(T a, T b); 

//RATIONAL CLASS DEFINITIONS:
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
    //constructors:
    rational() : nom(0),den(1){ 
        //print("default ctor -> 0"); 
    } 
    //default c_tor, implicit 0/1, one argument n gives us n/1
    rational(T n, T d = 1) : nom(n),den(d) { 
        reduce(); 
        //print("arg ctor -> {}/{}\n",nom,den); 
    } 
    //copy c_tor
    rational(const rational& rn) : nom(rn.nom),den(rn.den) { 
        //print("copy ctor {}/{}\n",nom,den); 
    }
    //move c_tor
    rational(rational&& rn) noexcept : nom(std::move(rn.nom)),den(std::move(rn.den)) { 
        //print("move ctor {}/{}\n", nom,den);
    }
    //copy_swap operator, pass by value makes a copy which is returned as the desired new object
    
    ~rational(){ 
        //print("rational {}/{} destructor\n",nom, den);
    }

    //GETTERS and SETTERS for the two int numbers
    T nomin() const { return nom;}
    
    void set_nom(T n) { nom = n; }
    
    T denomin() const { return den; }
    
    void set_den(T d) { den = d; }
    
    //functions to get the number as a floating point rather than a fraction
    double dbl() const { return static_cast<double>(nom) / static_cast<double>(den);}
    float flt() const { return static_cast<float>(nom) / static_cast<float>(den);}
    
    //MEMBER OPERATORS
    rational& operator=(rational rn)noexcept; 

    rational& operator+=(const rational& frac);

    rational& operator-=(const rational& frac);

    rational& operator*=(const rational& frac);
    
    rational& operator/=(const rational& frac);
    
    rational& operator%=(rational frac);

    //INCREMENTS
    rational& operator++();
    
    rational operator++(int);

    rational& operator--();
    
    rational operator--(int);

    //function for reducing the fraction
    //just divide everything with greatest common denominator, which could be one therefore no change
    void reduce();

    //getting it as string for printing
    /*
       * We want x/1 to be printed as simple x, 
       * and x/x as 1 (which should be already kept as 1/1)
       * 0/anything should just be 0
       * any farc < 0 should be written as (-p/q)
    */
    std::string str() const;
};

template <Arithmetic T>
inline constexpr rational<T> make_rational(T n, T d);

//NONMEMBER OPERATOR OVERLOADS:
template <Arithmetic T>
//spaceship operator for comparison
inline auto operator<=>(const rational<T>& lhs, const rational<T>& rhs);

template <Arithmetic T>
inline auto operator<=>(const rational<T>& frac, const T& t){
    return frac.nomin() <=> frac.denomin()*t;
}


template <Arithmetic T>
inline auto operator==(const rational<T>& lhs, const rational<T>& rhs);

template <Arithmetic T>
inline auto operator* (const rational<T>& lhs, const rational<T>& rhs);

template <Arithmetic T>
inline auto operator/ (const rational<T>& lhs, const rational<T>& rhs);

template <Arithmetic T>
inline auto operator%(rational<T> lhs, rational<T> rhs);

template <Arithmetic T>
inline auto operator+ (const rational<T>& lhs, const rational<T>& rhs);

template <Arithmetic T>
inline auto operator- (const rational<T>& lhs, const rational<T>& rhs);
 
template <Arithmetic T>
//for negation with -
inline auto operator-(const rational<T>& frac);

template <Arithmetic T>
inline std::ostream& operator<<(std::ostream& os, const rational<T>& frac){
    os << frac.str();
    return os;
}

//formater for rational class to print it directly with print()
template<Arithmetic T>
struct std::formatter<rational<T>> : std::formatter<std::string> {
    template<typename Context>
    auto format(const rational<T>& frac, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", frac.str());
    }
};


#include "ratio.cpp"
#endif