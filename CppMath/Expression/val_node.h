#ifndef VAL_NODE_H
#define VAL_NODE_H

#include "node.h"

class val_node:public node
{
    int val{0};
public:
    val_node(){ 
        
        test_msg("default value node ctor\n");
    
    }
    val_node(int value):val(value){ 
        
        test_msg("value node int ctor\n");
    
    }
    
    //THE PROBLEM IS ON THE NEXT LINE: can't access variable vn
    
    val_node(const node& vn):val(vn.getVal()){ 
        
        test_msg("value {} node copy ctor\n", val);
    
    }
    val_node(node&& vn):val(move(static_cast<val_node>(vn).val)){ 
        
        test_msg("value node move ctor\n"); 
    
    }
    ~val_node(){
        test_msg("value {} node dest\n",val);
    } 

    std::string str() const override { return std::string("{"+std::to_string(val)+"}"); }
    //getters:
    int getVal() const override {return val;}
    void setVal(int v) override { val = v; }
    Op getOp() const override { 
        return Op::val;
    }
    void setOp(Op o) override {
        return;
    }

    void copy(const node& nd) override {

    }
    //value nodes are leaves, so there are no sons 
    node* getLeft() const override {return nullptr;}
    node* getRight() const override {return nullptr;}
    //we can't set them

    void setLeft(unique_ptr<node> left) override {
        print("trying to set value node son!\n");
    }
    void setRight(unique_ptr<node> right) override {
        print("trying to set value node son!\n");
    }

    void setLeft(const node& left) override {
        print("trying to set value node son!\n");
    }
    void setRight(const node& right) override {
        print("trying to set value node son!\n");
    }


    bool op_allowed(Op op, int rhs) const override;
   
    bool operator==(const node& nd) const override {
        return val == nd.getVal();
    }

    val_node& operator=(val_node nd) noexcept {
        
        test_msg("val_node swap copy creator\n");
    
        std::swap(val,nd.val);
        return *this;
    }

    node& operator+=(int rhs) override {
        val += rhs;
        return *this;
    }
    node& operator-=(int rhs) override {
        val -= rhs;
        return *this;
    }
    node& operator*=(int rhs) override {
        val *= rhs;
        return *this;
    }
    node& operator/=(int rhs) override {
        val /= rhs;
        return *this;
    }
};

bool val_node::op_allowed(Op op, int rhs) const {
    switch (op)
    {
        case Op::div:
            if (val % rhs)
                return false;
            break;
        default:
            break;
    }
    return true;
}

//formater for expression class
template<>
struct std::formatter<val_node> : std::formatter<int> {
    template<typename Context>
    auto format(const val_node& vn, Context& ctx) const {
        return std::format_to(ctx.out(), "{}", vn.str());
    }
};

#endif