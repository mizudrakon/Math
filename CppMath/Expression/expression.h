#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../my_concepts.hpp"//so far I'm not using them
#include "op_node.h"
#include "val_node.h"

/*  expression as a tree of runtime polymorphic nodes - value or op nodes 
    - so far it's a rough sketch, probably can just look and see from the code below:
*/

class expression 
{
    unique_ptr<node> head;
public:
    ~expression() = default;//unique pointers should take care of the tree structure
    expression(): head(make_unique<val_node>(0)){ 
        
        test_msg("default expression ctor\n");
    
    }
    expression(int val): head(make_unique<val_node>(val)){
        
        test_msg("expression int ctor\n");
    
    }
    expression(expression&& exs): head(std::move(exs.head)){
        
        test_msg("expression move ctor\n");
    
    }
    
    expression(const expression& rhs) 
    {
        if (rhs.getHead()->getOp()==Op::val){
            val_node rhead = *rhs.getHead();//copy of rhs.head?
            head = make_unique<val_node>(std::move(rhead));
        } else {
            op_node rhead = *rhs.getHead();//PROBLEM!!!
            head = make_unique<op_node>(std::move(rhead));
        }
    }

    //NOT TESTED!
    expression(const expression& lhs, const expression& rhs, Op o): 
        head(make_unique<op_node>(o))
    {
        head->setRight(*lhs.getHead());
        head->setRight(*rhs.getHead());
    }

    auto& operator=(expression rhs) noexcept {
        
        test_msg("expression assignment swap ctor\n");
    
        std::swap(head, rhs.head);
        return *this;
    }

    node* getHead() const{
        return head.get();
    }

    string subtree_str(const node* nd) const{
        if (nd->getOp() == Op::val)
            return nd->str();
        return subtree_str(nd->getLeft()) + nd->str() + subtree_str(nd->getRight());
    }

    string str() const{
        return subtree_str(head.get());
    }

    auto operator==(int rhs) const{
        if (head.get()->getOp() == Op::val)
            return head.get()->getVal() <=> rhs;
    }

    //find a val node after a plus or minus node to modiffy
    node* find_val(node* p_node){
        auto node_op = p_node->getOp();
        if (node_op == Op::val) return p_node;
        if (node_op != Op::plus && node_op != Op::minus){
            return nullptr;
        }
        auto p_res = find_val(p_node->getLeft());
        if (p_res != nullptr)
            return p_res;
        p_res = find_val(p_node->getRight());
        return p_res;
    }

    void swap_ops(node* p_node){
        auto node_op = p_node->getOp();
        if (node_op == Op::val) return;
        if (node_op == Op::plus) p_node->setOp(Op::minus);
        if (node_op == Op::minus) p_node->setOp(Op::plus);
        swap_ops(p_node->getLeft());
        swap_ops(p_node->getRight());
    }

    auto& operator+=(int rhs){
        auto nd = head.get();
        if (rhs == 0) return *this; //0 is neutral for addition and subtraction 
        if (head.get()->getOp() == Op::val)
        {
            //print("simple case...\n");
            *head+=rhs;
            return *this;
        }
        //difficult case - search for a moddiffyable value node
        //print("starting find_val\n"); 
        auto p_valNode = find_val(nd);
        //print("ended find_val\n");
        if (p_valNode != nullptr) 
        {    
            //print("found val to modify: {}\n", p_valNode->str());
            *p_valNode += rhs; 
            //print("val after modification: {}\n", p_valNode->str());
        }
        //the rest - can't moddify
        else {
            auto new_head = make_unique<op_node>(Op::plus);
            new_head->setLeft(std::move(head));
            new_head->setRight(make_unique<val_node>(rhs));
            head = std::move(new_head);
        }
        //print("+= finished\n");
        return *this;
    }

    auto& operator+=(const expression& rhs){
        //either this or rhs has a value node as its head:
        //rhs or both
        //print("checking rhs == val\n");
        if (rhs.getHead()->getOp() == Op::val){
            //print("rhs is val\n");
            operator+=(rhs.getHead()->getVal());
            return *this;
        }
        //this is value, rhs is op
        else if (head->getOp() == Op::val){
            int v = head->getVal();//copy our value
            expression answ(rhs);//make a copy of rhs
            answ += v;//add value to rhs copy
            std::swap(head,answ.head);//take the answer's head!!!
            return *this;
        }
        //both are op
        auto new_head = make_unique<op_node>(Op::plus);
        new_head->setLeft(std::move(head));
        auto rhs_copy = rhs;
        new_head->setRight(std::move(rhs_copy.head));
        head = std::move(new_head);
        return *this;
    }

    auto& operator++(){
        operator+=(1);
        return *this;
    }
    auto operator++(int){
        auto old = *this;
        operator++();
        return old;
    }
    auto& operator-=(int rhs){
        operator+=(-rhs);
        return *this;
    }

    //NOT TESTED!!!
    auto operator-(){
        auto copy = *this;
        if (copy.head->getOp() == Op::val)
            *copy.head *= -1;    
        else {
            //switch value in the left minimum
            auto it = copy.getHead();
            while (it->getOp() != Op::val){
                it = it->getLeft();
            }
            if (it->getOp() == Op::val)
                *it *= -1;
            //switch all + ops to - and - to +
            swap_ops(copy.head.get());
        }
        return copy;
    }
    //NOT TESTED!!!
    auto& operator-=(expression rhs){
        operator+=(-rhs);
        return *this;
    }
    auto& operator--(){
        operator-=(1);
        return *this;
    }
    auto operator--(int){
    
        test_msg("operator--(int)\n");
    
        auto old = *this;
        operator--();
        return old;
    }
    //will this work???
    auto& operator*=(int rhs){
        if (rhs == 1) return *this;//do nothing
        if (head->getOp() == Op::val)
            *head*=rhs;
        else if (rhs == 0)//will turn any expression 0
            head = make_unique<val_node>(0);
        else if (head->getOp() == Op::div || head->getOp() == Op::mult)
            *head->getLeft() *= rhs;
        else if (head->getOp() == Op::plus || head.get()->getOp() == Op::minus){
            *head->getLeft() *= rhs;
            *head->getRight() *= rhs;
        }
        return *this;
    }
    auto& operator/=(int rhs){
        if (rhs == 1) return *this;//do nothing
        if (rhs == 0)
            throw std::runtime_error("attempted division by 0\n");
        // div requires multiplying the right son
        else if (head->getOp() == Op::div){
            *head->getRight() *= rhs;
        }
        // + or - requires recursion to both sons
        else if (head->getOp() == Op::plus || head.get()->getOp() == Op::minus){
            *head->getLeft() /= rhs;
            *head->getRight() /= rhs;
        }
        // can't divide -> make new father / 
        else if (head->getOp() == Op::val && head->getVal() % rhs != 0)
        {   
            auto new_head = make_unique<op_node>(Op::div);
            new_head->setLeft(std::move(head));
            new_head->setRight(make_unique<val_node>(rhs));
            head = std::move(new_head);
        }
        else
            *head /= rhs;
        return *this;
    }
};

auto operator<=>(const expression& lhs, int rhs){
    if (lhs.getHead()->getOp() == Op::val)
        return lhs.getHead()->getVal()<=>rhs;
    return lhs.getHead()->getVal() <=> 0;
}
auto operator<=>(const expression& lhs, const expression& rhs){
    return lhs.getHead()<=>rhs.getHead();
}
auto operator==(const expression& lhs, const expression& rhs){
    return lhs.getHead()==rhs.getHead();
}
auto operator==(const expression& lhs, int rhs){
    if (lhs.getHead()->getOp() == Op::val)
        return lhs.getHead()->getVal()==rhs;
    return false;
}
auto operator+(const expression& lhs, int rhs)
{
    auto answ(lhs);
    answ += rhs;
    return answ;
}
auto operator+(const expression& lhs, const expression& rhs)
{
    //print("making lhs copy\n");
    auto answ(lhs);
    //print("lhs copy created\n");
    answ += rhs;
    return answ;
}
auto operator-(const expression& lhs, int rhs)
{
    auto answ(lhs);
    answ -= rhs;
    return answ;
}
auto operator-(expression lhs, const expression& rhs)
{
    auto answ(lhs);
    return lhs;
}
auto operator*(const expression& lhs, int rhs)
{
    auto answ(lhs);
    answ *= rhs;
    return answ;
}
auto operator*(expression lhs, const expression& rhs)
{
    return lhs;
}
auto operator/(const expression& lhs, int rhs)
{
    //print("{} op/ {}:\n",lhs.str(),rhs);
    auto answ(lhs);
    answ /= rhs;
    return answ;
}
auto operator/(expression lhs, const expression& rhs)
{
    return lhs;
}

//formater for expression class
template<>
struct std::formatter<expression> : std::formatter<int> {
    template<typename Context>
    auto format(const expression& exp, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", exp.str());
    }
};

#endif