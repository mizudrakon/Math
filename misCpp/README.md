### MisCpp   
* same as MisC = a collection of some smaller projects,  
* mostly jsut stuff I wanted to implement even though nobody's asking for it...
* seriously: you can find code for quicksort and such everywhere, this is all mostly about me.
  

#### Quick  
* quicksort in place allowing a custom comparator like the std::sort in algorithm.
  * I should implement a trivial sort function to do some \< n case...
  * not sure what else now, it's in-place, so memory-wise it should be pretty ok, it's using a queue for start, end indexes of every partial range instead of being called recursively, which should save us some stack...
* quickselect - returns k-th largest element, but it's an untested first implementation.

#### Matrix   
* I'm taking my school Matrix class and I'm going to improve it.