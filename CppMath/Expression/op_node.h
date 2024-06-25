#ifndef OP_NODE_H
#define OP_NODE_H

#include "node.h"
#include "val_node.h"


class op_node:public node
{
    Op op;
    //node owns it's sons
    unique_ptr<node> left;
    unique_ptr<node> right;
public:
    //ctors
    op_node() = delete;//no default, currently don't know what to do with it
    /*
    op_node(const op_node& o):op(o.op){
        if (o.left->getOp() == Op::val)
            left = o.left;
    }
    */
    op_node(Op o):op(o){ print("OP_NODE {} ctor\n", str()); }
    op_node(const node& opn):
        op(static_cast<op_node>(opn).op),
        left(std::move(static_cast<op_node>(opn).left)),
        right(std::move(static_cast<op_node>(op).right))
    {
        print("OP_NODE copy ctor\n");
        if (left->getOp() == Op::val)
            left = make_unique<value_node>(*opn.getLeft());
        else left = make_unique<op_node>(*opn.getLeft());
        if (left->getOp() == Op::val)
            right = make_unique<value_node>(*opn.getRight());
        else right = make_unique<op_node>(*opn.getRight());
        //this should work recursively
    }
    op_node(node&& opn):op(move(static_cast<op_node>(opn).op)),left(move(static_cast<op_node>(opn).left)),
    right(move(static_cast<op_node>(opn).right))
    {
        print("op_node move ctor\n");
    }
    ~op_node(){
        print("op_node {} dest\n", str());
    }
    std::string str() const override;
    
    //getters:
    int getVal() const override { return 0; }//there is no value
    void setVal(int o) override { return; }
    Op getOp() const override { return op; }

    //return observers:
    node* getLeft() const override {return left.get();}
    node* getRight() const override {return right.get();}
    
    //setters:
    void setLeft(unique_ptr<node> nd) override {
        left = std::move(nd);
    }
    void setRight(unique_ptr<node> nd) override {
        right = std::move(nd);
    }

    bool op_allowed(Op rhs_op, int rhs) const override;

    //we need to compare operators to compare expressions    
    bool operator==(const node& nd) const override {
        return op == nd.getOp();
    }
    
    op_node& operator=(op_node nd) noexcept {
        print("op_node swap copy ctor\n");
        std::swap(op, nd.op);
        std::swap(left, nd.left);
        std::swap(right, nd.right);
        return *this;
    }

    //arithmetic operations don't make much sense for operations
    node& operator+=(int O) override {
        return *this;
    }
    node& operator-=(int O) override {
        return *this;
    }

    //original node becomes the new left, right is the rhs valu
    void addNode_up(Op o, int rhs){
        //make a copy that kidnapps the children
        auto new_node = make_unique<op_node>(op);
        new_node->setLeft(std::move(left));
        new_node->setRight(std::move(right));
        //move to current left son 
        setLeft(std::move(new_node));
        //change op to new op
        op = o;
        //current right will be the multiplier
        setRight(make_unique<value_node>(rhs));
    }

    void addNode(unique_ptr<node> son, Op o, int rhs){
        auto new_node = make_unique<op_node>(op);
        new_node->setLeft(std::move(son));
        new_node->setRight(make_unique<value_node>(rhs));
        son = std::move(new_node);
    } 

    node& operator*=(int rhs) override {
        //distribution
        if (op == Op::div)
        {
            *left *= rhs;   
        }
        else if (op == Op::plus || op == Op::minus){
            *left *= rhs;
            *right *= rhs;
        } 
        //chain up
        else {
            addNode_up(Op::mult,rhs);
        }
        return *this;
    }
    //similar to *=
    node& operator/=(int rhs) override {
        if (op == Op::div)
        {
            *right *= rhs;
        }
        else if (op == Op::plus || op == Op::minus){
            if (left->getOp() == Op::val && left->getVal() % rhs != 0)
                addNode(std::move(left), Op::div, rhs);
            else *left /= rhs;
            if (right->getOp() == Op::val && right->getVal() % rhs != 0)
                addNode(std::move(right), Op::div, rhs);
            else *right /= rhs;
        } else {
            addNode_up(Op::div, rhs);
        }
        return *this;
    }
};

//I'll probably change printNode to str()
inline std::string op_node::str() const
{
    switch (op)
    {
        case Op::plus:
            return "+";
            break;
        
        case Op::minus:
            return "-";
            break;
            
        case Op::mult:
            return "*";
            break;

        case Op::div:
            return "/";
            break;
        /*
        case Op::pow:
            return "^";
            break;

        case Op::root:
            return "^1/";
            break;
        */
        default:
            break;
    }
    return "FAIL";
}

//formater for expression class
template<>
struct std::formatter<op_node> : std::formatter<int> {
    template<typename Context>
    auto format(const op_node& opn, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", opn.str());
    }
};

bool op_node::op_allowed(Op rhs_op, int rhs) const {
//should check if there's any point of accessing node's children
    //a subtree defined by multiplication or division can't be modified with addition or subtraction
    //(it's an indivisible unit)
    if (rhs_op == Op::plus || rhs_op == Op::minus){
        if (op == Op::mult || op == Op::div)
            return false;
    }
    //for the reverse situation, we have the distributivity rule letting us access the lower nodes.
    //more operations to come that won't let us enter the subtree
    //if POW
    //if ROOT
    //...
    return true;
}

#endif