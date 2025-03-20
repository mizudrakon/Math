If you're setting off on a journey from a basic integer to representing real numbers, you'll likely realize there's a really nice bottom up logical continuity ahead of you.
You can add, subtract, and multiply integers without any fear of accidentally leaving integers (well, except you can't go too large or small so you fit the digital representation of 32 bits)...
But what about division? Integer division loses information from the real number perspective, and this goes wrong just all the damn time.
So we switch to rational numbers, just pairs of basic integers with some operator overloaded to do the arithmetic, and voila, we can now divide without fear as well.
This can be enough for many applications and the data structure is quite simple and convenient, but...
Rational numbers are not real, we're missing the irrational domain. Nice thing is that we can't get there using any of the basic arithmetic operations, so in a way it's somewhat acceptable to stay here.
Now what if we want to do some geometry or linear algebra where we calculate vector sizes using square root? This can actually lead us to irrational numbers like $\sqrt{2}$, there is no way to write this as a fraction, so now we can invent a structure where we keep the base as int and exponent as a rational number. $\sqrt{2}=2^{\frac{1}{2}}$, how simple is that? We can now deal with n-th power or root of a number, our number system looks like this: $\frac{n^{p/q}}{d^{k/l}}$. 
That's really nice and all but let's try defining the basic operations for the new type with base and exponent:  
* exponent is of type $\frac{p}{1}$ for both numbers 
	-> if same base: add exponents;
	else: exponentiate