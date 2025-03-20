- all numbers expressible in a fraction form: $\frac{\mathbb{Z}}{\mathbb{Z}}$ ;  
- they are introduced into arithmetic with the division operator;
- when expressed as a float, they end in a period of a finite sequence, like: $\frac{1}{3}=0.(3), \frac{1}{8}=0.125(0), \frac{1}{7}=0.(142857)$.
- they are defined by the denominator *q* in $\frac{p}{q}$ the sequence is:
	$\lfloor\frac{p}{q}\rfloor. \frac{10r_0}{q},\frac{10r_1}{q},\dots ,\frac{10r_i}{q},\dots$   
- every remainder $r_i$ has to fit in $0 \leq r_i \leq q-1$ and the digit is $0\leq\frac{10r_i}{q}\leq 9$ in base 10, so you can technically have the same digit multiple times in the period, but the ramainders have to repeat before webget the *q-th* one, since for $\forall r_i = r_j: r_{i+1} = r_{j+1}$, the next remainder is always uniquely determined.  

