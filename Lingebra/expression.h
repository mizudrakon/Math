#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ratio.h"
#include "exp_num.h"

/* Expression is a binary tree
 * nodes are either values or operations
 * some operations can be functions
*/

template <Arithmetic T>
struct value_node 
{
    T value;
};

struct op_node 
{

};

class expression
{

};



#endif