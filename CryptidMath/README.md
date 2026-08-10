### CppMath   
 * this is supposed to be my implementation of all sorts of number types:  
 * matrix should support all matrix operations that I will remember...
 * lvector is going to be a vector that supports the right operations like dot product and element by element addition...
 * I'm making
 * I want to use fractions to represent rational and irrational numbers rather than floats or doubles, since they have a great impact on precision... at the moment I do not see if any of this should fail, but it's a work in progress mostly aimed on learning than any specific practical use.

 ### Rational
 * the point is to have a type that keeps as much information as possible. Fractions are pretty good for this, since we don't need to round them and lose information to use them in the next calculation.   
 * Irrational numbers are the problem, since we just simply can't express them any other way.  
 * The good thing is that we can't get an irrational number using anything $\in \mathbb{Q}$ with only basic operations like +, -, *, /, %, we need to use special functions that explore continuous intervals of $\mathbb{R}$, so no worries there unless you want to look for n-th roots or use goniometry... which is something we do in linear algebra.

### Exponential
 I've started working on exp_num as a number with an exponent, this should be useful to hold irrational numbers like sqrt(2), but it pretty much ends there.

### expression
Expression works as a binary tree of allowed operations as branching nodes and values as the leaves.
* so far it only 