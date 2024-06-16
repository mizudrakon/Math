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
    virtual void setVal(int) = 0;
    virtual Op getOp() const = 0;

    virtual node* getLeft() const = 0;
    virtual node* getRight() const = 0;

    virtual void setLeft(unique_ptr<node> left) = 0;
    virtual void setRight(unique_ptr<node> right) = 0;

    virtual bool op_allowed(Op op, int rhs) const = 0;

    virtual bool operator==(const node& nd) const = 0;
    virtual node& operator+=(int) = 0;
    virtual node& operator-=(int) = 0;
    virtual node& operator*=(int) = 0;
    virtual node& operator/=(int) = 0;


};

class value_node:public node
{
    int val{0};
public:
    value_node(){ print("default value node ctor\n");}
    value_node(int value):val(value){ print("value node int ctor\n");}
    
    //THE PROBLEM IS ON THE NEXT LINE: can't access variable vn
    value_node(const node& vn):val(vn.getVal()){ print("value {} node copy ctor\n", val);}
    value_node(node&& vn):val(move(static_cast<value_node>(vn).val)){ print("value node move ctor\n");}
    ~value_node(){
        print("value {} node dest\n", val);
    } 

    std::string str() const override { return std::string("{"+std::to_string(val)+"}"); }
    //getters:
    int getVal() const override {return val;}
    void setVal(int v) override { val = v; }
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

    bool op_allowed(Op op, int rhs) const override;
    
    bool operator==(const node& nd) const override {
        return val == nd.getVal();
    }

    value_node& operator=(value_node nd) noexcept {
        print("value_node swap copy creator\n");
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

bool value_node::op_allowed(Op op, int rhs) const {
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
            op = Op::mult;
            //current right will be the multiplier
            setRight(make_unique<value_node>(rhs));
    }
    
    node& operator*=(int rhs) override {
        //distribution
        if (op == Op::plus || op == Op::minus){
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
        if (op == Op::plus || op == Op::minus){
            *left /= rhs;
            *right /= rhs;
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
class expression 
{
    unique_ptr<node> head;
public:
    ~expression() = default;//unique pointers should take care of the tree structure
    expression(): head(make_unique<value_node>(0)){ print("default expression ctor\n");}
    expression(int val): head(make_unique<value_node>(val)){print("expression int ctor\n");}
    expression(expression&& exs): head(std::move(exs.head)){print("expression move ctor\n");}
    expression(const expression& rhs){
        print("expression copy ctor\n");
        auto p_rhs = rhs.head.get();
        //copy constructors should work recursively, so it's just about finding out what the head is
        if (p_rhs->getOp() == Op::val)
            head = make_unique<value_node>(*p_rhs);
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
        auto node = head.get();
        
        //   print("simple case...\n");
        if (head.get()->getOp() == Op::val)
        {
            *head+=rhs;
        }
        //difficult case - search for a moddiffyable value node
        auto p_valNode = find_val(node);
        if (p_valNode->getOp() == Op::val) 
            p_valNode->setVal(p_valNode->getVal() + rhs); 
        //the rest - can't moddify
        else {
            auto new_head = make_unique<op_node>(Op::plus);
            new_head->setLeft(std::move(head));
            new_head->setRight(make_unique<value_node>(rhs));
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
        else if (head->getOp() == Op::plus || head.get()->getOp() == Op::minus){
            *head->getLeft() *= rhs;
            *head->getRight() *= rhs;
        }
        return *this;
    }
    auto& operator/=(int rhs){
        if (rhs == 0)
            throw std::runtime_error("attempted division by 0\n");
        if (head->getOp() == Op::val)
            if (head->getVal() % rhs == 0)
                head->setVal(head->getVal() / rhs);
            else {
                auto new_head = make_unique<op_node>(Op::div);
                new_head->setLeft(std::move(head));
                new_head->setRight(make_unique<value_node>(rhs));
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