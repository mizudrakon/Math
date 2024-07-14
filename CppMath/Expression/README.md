### Expression   
 - a binary tree of operations and values.

### Supported operations
 - \+ and \- are mostly similar, so are \* and \   
 - there are operations to be added later, like: pow, mod%, goniometric functions, hopefully, but so far I'm focusing on the basics.
 #### \+ addition  
 - (\+= int) should be complete $\checkmark$
 - (\+ int) should be complete $\checkmark$
 - (\+= expression) missing $\times$
 #### \- minus 
 - (\-= int) so far works as (\+= -int) $✓$?
 - (\- int) so far works as (copy \+= -int) $✓$?
 - (\-= expression) missing $\times$
 #### \* multiplication  
 - (\*= int) $\checkmark$?  
 - (\* int) works as (copy \*= int) $\checkmark$?   
 - (\*= expression) missing $\times$
 #### / division  
 - (\/= int) $\checkmark$?  
 - (\-= expression) missing $\times$

### more missing
 - printing isn't really done yet, also want to do latex printing.   

I'll update this with test results. CppMath/Expression/tests/testExpress.cpp should test all the ops