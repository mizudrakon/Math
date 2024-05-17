#include "my_concepts.hpp"
#include <memory>
#include <format>

/*  expression as a tree of runtime polymorphic nodes - value or op nodes 
    - so far it's a rough sketch, probably can just look and see from the code below:
*/

constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text,std::make_format_args(args...)).c_str(), stdout);
}

using std::make_unique, std::unique_ptr;

enum class Op {
    plus,
    minus,
    mult,
    div,
    pow,
    root,
    val
};

class node
{
public:
    virtual ~node() = default;
    virtual void printNode() const = 0;
    
    virtual int getVal() const = 0;
    virtual Op getOp() const = 0;

    virtual node* getLeft() const = 0;
    virtual node* getRight() const = 0;

    virtual void setLeft(unique_ptr<node> left) = 0;
    virtual void setRight(unique_ptr<node> right) = 0;

    virtual bool operator==(const node& nd) const = 0;
    virtual node& operator+(const node& nd) = 0;
    virtual node& operator-(const node& nd) = 0;

};

class value_node:public node
{
    int val{0};
public:
    value_node(){ print("default value node ctor\n");}
    value_node(int value):val(value){ print("value node int ctor\n");}
    value_node(const value_node& vn):val(vn.val){ print("value node copy ctor\n");}
    value_node(value_node&& vn):val(std::move(vn.val)){ print("value node move ctor\n");}
    ~value_node(){
        print("value node dest\n");
    } 

    void printNode() const override { print("{}",val); }
    int getVal() const override {return val;}
    Op getOp() const override { 
        return Op::val;
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

    
    bool operator==(const node& nd) const override {
        return val == nd.getVal();
    }
    node& operator+(const node& nd) override {
        val += nd.getVal();
        return *this;
    }
    node& operator-(const node& nd) override {
        val -= nd.getVal();
        return *this;
    }

};

class op_node:public node
{
    Op op;
    //node owns it's sons
    unique_ptr<node> left;
    unique_ptr<node> right;
public:
    op_node() = delete;
    op_node(Op o):op(o){}
    ~op_node(){
        print("op_node dest\n");
    }
    void printNode() const override;
    int getVal() const override { return 0; }//there is no value
    Op getOp() const override { return op; }

    //return observers:
    node* getLeft() const override {return left.get();}
    node* getRight() const override {return right.get();}
    void setLeft(unique_ptr<node> nd) override {
        left = std::move(nd);
    }
    void setRight(unique_ptr<node> nd) override {
        right = std::move(nd);
    }

    //we need to compare operators to compare expressions    
    bool operator==(const node& nd) const override {
        return op == nd.getOp();
    }
    
    //arithmetic operations don't make much sense for operations
    node& operator+(const node& nd) override {
        return *this;
    }
    node& operator-(const node& nd) override {
        return *this;
    }
};

//I'll probably change printNode to str()
inline void op_node::printNode() const
{
    switch (op)
    {
    case Op::plus:
        print("+");
        break;
    
    case Op::minus:
        print("-");
        break;
        
    case Op::mult:
        print("*");
        break;

    case Op::div:
        print("/");
        break;

    case Op::pow:
        print("^");
        break;

    case Op::root:
        print("^1/");
        break;
    default:
        break;
    }
}

class expression 
{
    unique_ptr<node> head;
public:
    expression(): head(make_unique<value_node>(0)){ print("default expression ctor\n");}
    ~expression() = default;//unique pointers should take care of the tree structure
};