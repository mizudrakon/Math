#ifndef CRYPTID_PRIMES_H
#define CRYPTID_PRIMES_H

#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

/*  BRAINSTORM:
    - sieve of eratosthenes for a given number
        - number of primes lesser then a given number
    - prime decomposition for a given number
    - division tree for a given number?
*/

namespace cryptidmath
{
    namespace primes
    {
        std::vector<int> Sieve(int query)
        {
            std::unique_ptr<int[]> array = std::make_unique<int[]>(query+1);
            // 0 dividion is undefined, 1 divides all but isn't prime
            array[0] = 0;
            array[1] = 0;
            for (int i = 2; i <= query; i++)
            {
                // detect undefined and mark them 1 first
                if (array[i] != 1 && array[i] != 2)
                    array[i] = 2;
                // skip those marked 0 already
                if (array[i] == 1)
                    continue;
                // if next is too large, we're done with i
                if (2*i > query)
                    continue;
                // mark multiplies by 0
                for (int j = 2*i; j <= query; j += i)
                {
                    array[j] = 1;
                }
            }
            std::vector<int> result;
            for (int i = 2; i <= query; i++)
            {
                if (array[i] == 2){
                    result.push_back(i);
                }
            } 
            return result;
        }

        // vector of pairs: prime, exponent value
        std::vector<std::pair<int,int>> decomposition(int query)
        {
            int sieve_len = (query%2) ? query/2 : std::sqrt(query); 
            auto sieve = Sieve(sieve_len);
            std::vector<std::pair<int,int>> result;
            for (auto p : sieve)
            {
                int exp = 0;
                while (query % p == 0 && query > 1)
                {
                    query /= p;
                    exp++;
                }
                if (exp)
                {
                    result.push_back({p,exp});
                }
            }
            return result;
        }
    }
} 

#endif