#include "my_concepts.hpp"
#include <memory>
#include <format>
#include <string>

/*  expression as a tree of runtime polymorphic nodes - value or op nodes 
    - so far it's a rough sketch, probably can just look and see from the code below:
*/

constexpr void print(const std::string_view text, auto&&... args){
    fputs(std::vformat(text,std::make_format_args(args...)).c_str(), stdout);
}

using std::make_unique, std::unique_ptr, std::move;
using std::string;

enum class Op {
    plus,
    minus,
    mult,
    div,
    //pow,
    //root,
    val
};

class node
{
public:
    virtual ~node() = default;
    virtual std::string str() const = 0;
    
    virtual int getVal() const = 0;
    virtual Op getOp() const = 0;

    virtual node* getLeft() const = 0;
    virtual node* getRight() const = 0;

    virtual void setLeft(unique_ptr<node> left) = 0;
    virtual void setRight(unique_ptr<node> right) = 0;

    virtual bool operator==(const node& nd) const = 0;
    virtual node& operator+(const node& nd) = 0;
    virtual node& operator-(const node& nd) = 0;
    virtual node& operator*(const node& nd) = 0;

};

class value_node:public node
{
    int val{0};
public:
    value_node(){ print("default value node ctor\n");}
    value_node(int value):val(value){ print("value node int ctor\n");}
    value_node(const node& vn):val(static_cast<value_node>(vn).val){ print("value node copy ctor\n");}
    value_node(node&& vn):val(move(static_cast<value_node>(vn).val)){ print("value node move ctor\n");}
    ~value_node(){
        print("value node dest\n");
    } 

    std::string str() const override { return std::string("{"+std::to_string(val)+"}"); }
    //getters:
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
    node& operator*(const node& nd) override {
        val *= nd.getVal();
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
    //ctors
    op_node() = delete;//no default, currently don't know what to do with it
    op_node(Op o):op(o){}
    op_node(const node& opn):op(static_cast<op_node>(opn).op){
        print("op_node copy ctor\n");
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
        print("op_node dest\n");
    }
    std::string str() const override;
    
    //getters:
    int getVal() const override { return 0; }//there is no value
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
    node& operator*(const node& nd) override {
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

class expression 
{
    unique_ptr<node> head;
public:
    ~expression() = default;//unique pointers should take care of the tree structure
    expression(): head(make_unique<value_node>(0)){ print("default expression ctor\n");}
    expression(int val): head(make_unique<value_node>(val)){print("expression int ctor\n");}
    expression(expression&& exs): head(std::move(exs.head)){print("expression move ctor\n");}
    expression(const expression& rhs){
        auto p_rhs = rhs.head.get();
        //copy constructors should work recursively, so it's just about finding out what the head is
        if (p_rhs->getOp() == Op::val)
            head = make_unique<value_node>(*p_rhs);
            //val_node doesn't actually have children
        else
            head = make_unique<op_node>(*p_rhs);
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

    auto operator==(const expression& rhs) const{
        return head.get() <=> rhs.getHead();
    }
    auto& operator+=(const expression& rhs){
        return *this;
    }
    auto& operator++(){
        return *this;
    }
    auto& operator++(int){
        return *this;
    }
    auto& operator-=(const expression& rhs){
        return *this;
    }
    auto& operator--(){
        return *this;
    }
    auto& operator--(int){
        return *this;
    }
    auto& operator*=(const expression& rhs){
        return *this;
    }
    auto& operator/=(const expression& rhs){
        return *this;
    }
};

auto operator<=>(const expression& lhs, const expression& rhs){
    return lhs.getHead()<=>rhs.getHead();
}
auto operator==(const expression& lhs, const expression& rhs){
    return lhs.getHead()==rhs.getHead();
}
auto operator+(expression lhs, const expression& rhs)
{
    return lhs;
}
auto operator-(expression lhs, const expression& rhs)
{
    return lhs;
}
auto operator*(expression lhs, const expression& rhs)
{
    return lhs;
}
auto operator/(expression lhs, const expression& rhs)
{
    return lhs;
}