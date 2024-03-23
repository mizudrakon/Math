#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ratio.h"
#include "exp_num.h"

/* Expression is a binary tree
 * nodes are either values or operations
 * some operations can be functions
 * Expressions are designed to be used as an Arythmetic concept type
 * -> support for all the basic arithmetic operations
*/

enum class op_id { non, plus, minus, mult, div, mod };
struct node //can be operation or value node
{
    //op_id - if non it's a value node
    op_id op;
    //parent: can only be op_node
    node* mom;
    //daughter_left: op_node or value_node
    node* left;
    //daughter_right: op_node or value_node
    node* right;
};

class expression
{
    node* head;

public:
    expression();

};



#endif