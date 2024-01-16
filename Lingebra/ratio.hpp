#ifndef RATIO_H
#define RATIO_H

#include<format>
#include<string>
#include<tuple>
#include<cmath>

class rational;
class exp_num;

//print function
constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text, std::make_format_args(args...)).c_str(),stdout);
}

//greatest common divisor... classic Eukleides
inline int gcd(int a, int b){ 
    while(true)
    {
        if (a < b) std::swap(a,b);
        if (b == 0) return a;
        a %= b;
    }
    return 1;
}

//lowest common multiple returing a tuple with the required multiplier to a and b
inline std::tuple<int,int,int> lcm(int a, int b){ 
    int x = a,
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

class rational 
{
    int nom {0};
    int den {1};
public:
    rational() = delete;
    rational(int n = 0, int d = 1):nom(n),den(d){}
    rational(const rational& rn):nom(rn.nom),den(rn.den){}
    rational(rational&& rn) noexcept : nom(std::move(rn.nom)),den(std::move(rn.den)){}
    rational& operator=(rational rn)noexcept {
        std::swap(nom, rn.nom);
        std::swap(den, rn.den);
        return *this;
    }
    rational& operator*=(const rational& frac){
        nom *= frac.nom;
        den *= frac.den;
        return *this;
    }
    rational& operator/=(const rational& frac){
        nom /= frac.nom;
        den /= frac.den;
        return *this;
    }
    rational& operator%=(rational frac){

        reduce();
        frac.reduce();
        auto [com_den, m, rm] = lcm(den,frac.den);
        rational result((nom*m) % (frac.nom*rm),com_den);
        std::swap(*this,result);
        return *this;
    }

    ~rational(){}

    int nomin() const { return nom;}
    void set_nom(int n) { nom = n; }
    int denomin() const { return den; }
    void set_den(int d) { den = d; }
    double dbl() const { return static_cast<double>(nom) / static_cast<double>(den);}
    float flt() const { return static_cast<float>(nom) / static_cast<float>(den);}
    void reduce();
    std::string str() const{
        
        return std::to_string(nom) + "/" + std::to_string(den);
    }
};

inline void rational::reduce(){
    int c = gcd(nom,den);
    nom /= c;
    den /= c;
}


inline auto operator<=>(const rational& lhs, const rational& rhs){
    if (lhs.denomin() == rhs.denomin()) return lhs.nomin() <=> rhs.nomin();
    auto a = lhs, b = rhs;
    a.reduce();
    b.reduce();
    auto [com_den, am, bm] = lcm(a.denomin(),b.denomin());
    
    return (a.nomin()*am) <=> (b.nomin()*bm);
};

inline auto operator==(const rational& lhs, const rational& rhs){
    if (lhs.nomin() == rhs.nomin() && lhs.denomin() == rhs.denomin())
        return true;
    auto a = lhs, b = rhs;
    a.reduce();
    b.reduce();
    return a.nomin() == b.nomin() && a.denomin() == b.denomin();
};

inline auto operator* (const rational& lhs, const rational& rhs){
    rational new_r(lhs.nomin()*rhs.nomin(),lhs.denomin()*rhs.denomin());
    return new_r;
};


inline auto operator/ (const rational& lhs, const rational& rhs){
    rational new_r(lhs.nomin()*rhs.denomin(),lhs.denomin()+rhs.nomin());
    return new_r;
};

inline auto operator%(rational lhs, rational rhs){
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

inline auto operator+ (rational lhs, rational rhs){
    if (lhs.denomin() != rhs.denomin()){
        lhs.reduce();
        rhs.reduce();
    }
    auto [com_denom,lhsm,rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm+rhs.nomin()*rhsm,com_denom);
    return result;
};

inline auto operator- (rational lhs, rational rhs){
    if (lhs.denomin() != rhs.denomin()){
        lhs.reduce();
        rhs.reduce();
    }
    auto [com_denom, lhsm, rhsm] = lcm(lhs.denomin(),rhs.denomin());
    rational result(lhs.nomin()*lhsm - rhs.nomin()*rhsm, com_denom);
    return result;
};

inline auto operator-(rational frac){
    return 0-frac;
}

//formater for rational class to print it directly with print()
template<>
struct std::formatter<rational> : std::formatter<std::string> {
    template<typename Context>
    auto format(const rational& frac, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", frac.str());
    }
};

//OK, this would be a lot of work, so I need to check if I need to do it
class exp_num
{
    int base {0};
    rational exponent {1};
public:
    exp_num(int b = 0, rational exp = 1) : base(b),exponent(exp){}
    exp_num(const exp_num& en) : base(en.base),exponent(en.exponent){}
    exp_num(exp_num&& en) noexcept : base(std::move(en.base)),exponent(std::move(en.exponent)){}

    int Base() const { return base; }
    void SetBase(int x) { base = x; } 
    rational Exp() const { return exponent; }
    void SetExp(rational r) { exponent = r; }
    void Reduce() {
        if (exponent.denomin() == 1){
            base = int(pow(base, exponent.nomin()));
            exponent = 1;
        }
    }
};


//multiplies only if a nad b are of the same base or their exps are 1
exp_num& operator*(const exp_num& a, const exp_num& b){
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

#endif