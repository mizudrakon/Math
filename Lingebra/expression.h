#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ratio.h"
#include "exp_num.h"
#include <memory>

/* Expression is a binary tree
 * nodes are either values or operations
 * some operations can be functions
 * Expressions are designed to be used as an Arythmetic concept type
 * -> support for all the basic arithmetic operations
*/

enum class op_id { non, plus, minus, mult, div, mod };
template <Arithmetic T>
struct node //can be operation or value node
{
    //op_id - if non it's a value node
    op_id op;
    std::unique_ptr<exp_num<T>> pvalue;
    //daughter_left: op_node or value_node
    std::unique_ptr<node> pleft;
    //daughter_right: op_node or value_node
    std::unique_ptr<node> pright;

};

template <Arithmetic T>
class expression
{
    std::unique_ptr<node<T>> head;

public:
    expression():head(std::make_unique<node<T>>(make_node(op_id::non,nullptr))){
        print("expr default ctor\n");
    }
    expression(const expression& orig){
        head = std::make_unique<node<T>>(make_node(orig->head.op,orig->head))
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