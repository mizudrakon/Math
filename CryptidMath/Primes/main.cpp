#include <iostream>
#include <cmath>
#include "Primes.h"

int main()
{
    int number{115};
    auto s = cryptidmath::primes::Sieve(number);
    for (auto p : s){
        std::cout << p << ' ';
    }
    std::cout << std::endl;
    std::cout << "possible prime divisors of " << number << " are:" << std::endl;
    s = cryptidmath::primes::Sieve(number/2);
    for (auto p : s){
        std::cout << p << ' ';
    }
    std::cout << std::endl;
    std::cout << "prime decomposition: " << std::endl;
    auto pd = cryptidmath::primes::decomposition(number);
    for (auto pair : pd){
        std::cout << pair.first << '^' << pair.second << ' ';
    }
    std::cout << std::endl;
}