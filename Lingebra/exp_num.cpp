#include "exp_num.h"

void exp_num::Reduce() {
    exponent.reduce();
    if (exponent.denomin() == 1){
        base = int(pow(base, exponent.nomin()));
        exponent = 1;
    }
}



std::string exp_num::str() const {
    if (exponent == 0) return "1"; //reduce to 1
    std::string text = std::to_string(base); 
    if (exponent == 1) return text;//no need to print exponent
    return text + "^{" + exponent.str() + '}';//also print exponent, if it's nontrivial
}