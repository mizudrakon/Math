#include "ratio.h"
#include "my_concepts.hpp"

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