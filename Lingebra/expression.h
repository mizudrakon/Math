#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ratio.h"
#include "exp_num.h"
#include <memory>

using std::unique_ptr, std::make_unique;

/* Expression is a binary tree
 * nodes are either values or operations
 * some operations can be functions
 * Expressions are designed to be used as an Arythmetic concept type
 * -> support for all the basic arithmetic operations
*/

enum class op_id { non, plus, minus, mult, div, mod };

//template <Arithmetic T>
struct node //can be operation or value node
{
    //op_id - if non it's a value node
    op_id op;
    unique_ptr<rational<exp_num<int>>> p_value;
    //daughter_left: op_node or value_node
    unique_ptr<node> p_left;
    //daughter_right: op_node or value_node
    unique_ptr<node> p_right;

};

template <ExpressionValue T>
class expression
{
    unique_ptr<node<T>> head;

public:
    expression():head(make_unique<node<T>>(make_node(op_id::non,nullptr))){
        head->p_value = make_unique<T>
        print("expr default ctor\n");
    }

    expression(const expression& orig){
        head = make_unique<node<T>>(make_node(orig->head.op,orig->head))
        auto p_node = head.get();
        if (p_node.pleft != nullptr){

        }
    }
    expression(expression&& orig):head(std::move(orig.head)){
        print("expr move ctor\n");
    }
    //create a node 
    auto make_node(op_id op,std::unique_ptr<exp_num<T>> pval);

    auto operator=(const expression& rhse);
    auto operator=(expression&& rhse) noexcept;
    auto operator+=(const expression& rhse);
    auto operator-=(const expression& rhse);
    auto operator*=(const expression& rhse);
    auto operator/=(const expression& rhse);
    
};


#include "expression.cpp"
#endif