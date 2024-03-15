#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ratio.h"
#include "exp_num.h"

/* Expression is a binary tree
 * nodes are either values or operations
 * some operations can be functions
*/

struct value_node 
{
    //value: rational<exp_num<int>>
    //parent: op_node*
};

enum class op_id { plus, minus, mult, div, mod };
struct op_node 
{
    //op_id
    //parent: can only be op_node
    //daughter_left: op_node or value_node
    //daughter_right: op_node or value_node
};

class expression
{

};



#endif