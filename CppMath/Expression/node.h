#ifndef NODE_H
#define NODE_H

#include <format>
#include <string>
#include <memory>

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
    virtual void setOp(Op) = 0;

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

#endif