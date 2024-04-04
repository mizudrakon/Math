### Lingebra   
 * this is supposed to be my implementation of linear algebra into C:  
 * lvector is going to be a vector that supports the right operations like dot product and element by element addition...
 * matrix should support all matrix operations that I will remember...
 * I want to use fractions to represent rational and irrational numbers rather than floats or doubles, since they have a great impact on precision... at the moment I do not see if any of this should fail, but it's a work in progress mostly aimed on learning than any specific practical use.

 ### ratio.hpp
 * the point is to have a type that keeps as much information as possible. Fractions are pretty good for this, since we don't need to round them and lose information to use them in the next calculation.   
 * Irrational numbers are the problem, since we just simply can't express them any other way.  
 * The good thing is that we can't get an irrational number using anything $\in \mathbb{Q}$ with only basic operations like +, -, *, /, %, we need to use special functions that explore continuous intervals of $\mathbb{R}$, so no worries there unless you want to look for n-th roots or use goniometry... which is something we do in linear algebra.

### exp_num
 I've started working on exp_num as a number with an exponent, this should be useful to hold irrational numbers like sqrt(2), but the question is, what std classes are already in c++, because this seems more and more like me jumping into defining a whole new number class hierarchy just beyond the next corner...

### expression
* is supposed to collect multiple exp_nums that can't be cleanly evaluated into a single integer with fraction exponent.
* (+/-) we can only cleanly add numbers that that have exp = 1, 0, -1, otherwise we have to bring them up to the power of exp.
    * exp = p/q -> bring n to the power of p