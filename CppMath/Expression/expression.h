#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "my_concepts.hpp"
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
    expression(): head(make_unique<val_node>(0)){ print("default expression ctor\n");}
    expression(int val): head(make_unique<val_node>(val)){print("expression int ctor\n");}
    expression(expression&& exs): head(std::move(exs.head)){print("expression move ctor\n");}
    expression(const expression& rhs){
        print("expression copy ctor\n");
        auto p_rhs = rhs.head.get();
        //copy constructors should work recursively, so it's just about finding out what the head is
        if (p_rhs->getOp() == Op::val)
            head = make_unique<val_node>(*p_rhs);
            //val_node doesn't actually have children
        else
            head = make_unique<op_node>(*p_rhs);
    }
    auto& operator=(expression rhs) noexcept {
        print("expression assignment swap ctor\n");
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

    auto& operator+=(int rhs){
        //print("expression {} op+= {}:\n",head->str(),rhs);
        auto nd = head.get();
        
        //   print("simple case...\n");
        if (head.get()->getOp() == Op::val)
        {
            print("simple case...\n");
            *head+=rhs;
            return *this;
        }
        //difficult case - search for a moddiffyable value node
        print("starting find_val\n"); 
        auto p_valNode = find_val(nd);
        print("ended find_val\n");
        if (p_valNode != nullptr) 
        {    
            print("found val to modify: {}\n", p_valNode->str());
            *p_valNode += rhs; 
            print("val after modification: {}\n", p_valNode->str());
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
    auto& operator--(){
        operator-=(1);
        return *this;
    }
    auto operator--(int){
        auto old = *this;
        operator--();
        return old;
    }
    //will this work???
    auto& operator*=(int rhs){
        if (head->getOp() == Op::val){
            *head*=rhs;
        }
        else if (head->getOp() == Op::div)
            *head->getLeft() *= rhs;
        else if (head->getOp() == Op::plus || head.get()->getOp() == Op::minus){
            *head->getLeft() *= rhs;
            *head->getRight() *= rhs;
        }
        return *this;
    }
    auto& operator/=(int rhs){
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
auto operator+(expression lhs, const expression& rhs)
{
    return lhs;
}
auto operator-(const expression& lhs, int rhs)
{
    auto answ(lhs);
    answ -= rhs;
    return answ;
}
auto operator-(expression lhs, const expression& rhs)
{
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