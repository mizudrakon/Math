# Expression  

- a binary tree of operations and values.

## Last changes

- all of these are untested - e+e and -expression got some testing done
- fixed copy constructor for expression, added setLeft/Right(const node&) to simplify
- need to finish e*e and e/e before starting pow and sqrt
- operator\*= int modified for 'other' cases NOT TESTED!!! (other cases not implemented yet)
- all non-member operators make an lhs expression copy, moddify it, and return it as the result.

### Supported operations

- \+ and \- are mostly similar, so are \* and \\
- there are operations to be added later, like: pow, mod%, goniometric functions, hopefully, but so far I'm focusing on the basics.

#### \+ addition  

- (\+= int) should be complete $\checkmark$
- (\+ int) should be complete $\checkmark$
- (\+= expression) first draft, not tested $\times$
- (expression \+ expression) first draft using \+=, not tested

#### \- minus

- (\-) signs switch, not tested
- (\-= int) so far works as (\+= -int) $✓$?
- (\- int) so far works as (copy \+= -int) $✓$?
- (\-= expression) missing $\times$

#### \* multiplication  

- (\*= int) $\checkmark$?  
- (\* int) works as (copy \*= int) $\checkmark$?
- (\*= expression)
  - checks simple cases of one expression being just a hidden value -> \*= int
  - else connects the two expr with op_node \*

#### / division  

- (\/= int) $\checkmark$?  
- (\-= expression)
  - works the same as in multiplication

### more missing

- printing isn't really done yet, also want to do latex printing.

I'll update this with test results. CppMath/Expression/tests/testExpress.cpp should test all the ops
