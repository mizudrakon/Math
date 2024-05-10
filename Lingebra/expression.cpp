#include "expression.h"

template <ExpressionValue T>
auto expression<T>::make_node(op_id op,std::unique_ptr<exp_num<T>> pval){
    auto N = std::make_unique<node<T>>(new node<T>());
    N->op = op;
    N->pvalue = pval;
    N->pleft = nullptr;
    N->pright = nullptr;
    return N;
}