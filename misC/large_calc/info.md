### Large number calculator C
 * First of all this is mostly a project to remember the ways of C coming from some serious time with C++;  
 * It's supposed to read and add two large numbers together, so basically reading them as a string.  
  * there are other ways, though... there's a reason this is not how ints, doubles, floats usually work: their encoding is far more optimal than using a string, so it might be better to use an array of unsigned integers (just thinking aloud here).
   

### Done:  
 * So we've got a new struct type with an array of chars and some info about it that gets read from some specified input.
 * Can read numbers of different base, but it needs to be specified.
   
### To Do:   
 * well, the addition,
 * shift left and right (which is mostly useful in multiplication)
 * the array hodling the number should be navigated as a cycle (moveable begin and end pointers)