#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "input_parse.hpp"
#include "number_vector.hpp"
//#include "pmvector.hpp"

//need to read input line by line




//class that evaluates a vector of type specific terms like
//numbers, variables, brackets, operators
class expression 
{
    std::vector<std::string> terms_list; //is provided by some tailored outside function to be evaluated
    //by default the Iput::read_line() does this here
    
    int priority; //is a countdown since it basically handles the - operation which has a difficult priority among the others
    bool last_brack; //remembers if the last term was a bracket, which also helps with the priority of operations
    number_vector vals;//number vector of int, float and double in number_vector.hpp
    std::vector<char> ops;//queue of operations
    std::vector<char> bracks;//queue of brackets (and generic marks for operations)
    std::vector<char> opening_brackets = {'(', '[', '{'};
    std::vector<char> closing_brackets = {')', ']', '}'};
    std::vector<char> operations = {'+', '-', '*', '/', '%', '='};

public: 
    expression():priority(0),last_brack(false),terms_list(Input::read_line())
    {
    }
    void evaluation(var_map& variables);
    bool add_term(int t, var_map& variables);//
    std::string do_op(var_map& variables);
    bool priority_op();
};

void expression::evaluation(var_map& variables)
{
    if (terms_list.back().front() == '@')
    {
        std::cout << "invalid\n";
        return;
    }
    for (int i = 0; i < terms_list.size(); i++)
    {

        if (!add_term(i,variables))
        {
            std::cout << "invalid\n";
            return;
        }
    }
    if (vals.size() == 0 || bracks.size() > 0) {
        std::cout << "invalid\n";
        return;
    }
    std::cout << vals.front()->value() << std::endl;
}

inline bool expression::priority_op()
{
    if (ops.size() == 0 || (bracks.size() > 0 && bracks.back() != 'o')) return false;
    char op = ops.back();
    
    return ((op == '*' || op == '/' || op == '%') && bracks.back() == 'o');
}

//uses double to calculate operations and converts to string
inline std::string expression::do_op(var_map& variables)
{
    double a,b,r;
    std::stringstream sresult;
    std::string var;
    int prec = vals.back()->prec();
    char op = ops.back();
    bracks.pop_back();
    ops.pop_back();
    char type_a,type_b; 
    b = stod(vals.back()->value());
    type_b = vals.back()->type();
    vals.pop_back();
    if (op == '=')
    {
        var = terms_list.front();
        var.pop_back();
        variables.save_variable(var,b);
        prec = precision(b);
        sresult.precision(prec);
        sresult << std::fixed << b;
        return sresult.str();
    }

    a = stod(vals.back()->value());
    type_a = vals.back()->type();
    if (vals.back()->prec() > prec) prec = vals.back()->prec();
    vals.pop_back();
    if (op == '+'){
        r = a + b;
        prec = precision(r);
        sresult.precision(prec);
        sresult << std::fixed << r;
        return sresult.str();
    }
    if (op == '-'){
        r = a - b;
        prec = precision(r);
        sresult.precision(prec);
        sresult << std::fixed << r;
        return sresult.str();
    }
    if (op == '*'){
        r = a * b;
        prec = precision(r);
        sresult.precision(prec);
        sresult << std::fixed << r;
        return sresult.str();
    }
    if (op == '/'){
        r = a / b;
        prec = precision(r);
        sresult.precision(prec);
        if (type_a == 'i' && type_b == 'i')
            sresult.precision(0);
        sresult << std::fixed << r;
        return sresult.str();
    }
    else {
        r = ((long)a % (long)b);
        prec = precision(r);
        sresult.precision(prec);
        sresult << std::fixed << r;
        return sresult.str();
    } 
    return sresult.str();

}

/* what can be added?
    - a number 8923.2389, variable $xyz, xyz=
    - bracket ()[]{}
    - operation
   *numbers get added to the vals vector, variables get converted to numbers and added too
   *brackets and operators get added to brackets vector
   *a closing bracket forces evaluation of an enclosed part of the formula
*/
//we've got the terms parsed, so we check their type by looking at the front symbol only
inline bool expression::add_term(int t, var_map& variables)
{
    if (terms_list[t] == "eof") //last term when input parser reaches \n 
    {
        
        //at this point there are no allowed open brackets
        while (ops.size() > 0)
        {
            if (vals.size() > 0)
                vals.push_back(do_op(variables));
            
        }
        if (vals.size() > 1 || vals.size() == 0) return false;
        return true;
    } 

    //variable recall 
    if (terms_list.at(t).front() == '$')
    {
        if (terms_list.at(t).length() > 1 && variables.exist(terms_list.at(t).substr(1)))
        {
            vals.push_back(std::to_string(variables.value(terms_list.at(t).substr(1))));
            
            if (priority > 0 && t+1 < terms_list.size())
            {
                if(contains(terms_list.at(t+1).front(), {'*','/','%'})) priority++;
            }

            if (priority_op() || priority == 1)
            {
                vals.push_back(do_op(variables));
                if (priority) priority--;
            }
            return true;
        }
        return false;
    }
    //variable assignemnt
    if (std::isalpha(terms_list.at(t).front()))
    { 
        if (vals.size() == 0)
        {
            ops.push_back('=');
            bracks.push_back('o');
            return true;
        }
        return false;
    }
    //number
    if (std::isdigit(terms_list.at(t).front()))
    {

        if (priority > 0 && t+1 < terms_list.size())
        {
            if(contains(terms_list.at(t+1).front(), {'*','/','%'})) priority++;
        }

        vals.push_back(terms_list.at(t));
        if (priority_op() || (priority == 1))
        {
            vals.push_back(do_op(variables));
            if (priority) priority--;
        }
        return true;
    }

    //at this point it has to be an operation or a bracket
    char t0 = terms_list.at(t).front();
    if (contains(t0, opening_brackets))//is an opening bracket -> just add
    {
        bracks.push_back(t0);
        return true;
    }
    //closing bracket leads to evaluate the whole bracket
    int b = contains(t0, closing_brackets);
    if (b)//it is a closing bracket if b > 0
    {
        b--;//the index of a matching opening bracket
        int c;//looks at the bracket list
        //reach the closest opening bracket - it needs to match else failure
        while ((c = bracks.back()) != opening_brackets.at(b))
        {
            if (c == 'o') // 'o' marks an operation in the bracket vector
            {
                vals.push_back(do_op(variables));// evaluates the last two terms into one
            }
            else 
            {
                return false;
            }
        }
        if (bracks.size() > 0)
        {
            bracks.pop_back(); //when the bracket is found, we remove it
            //unless there are too many open brackets 
        } 
        else return false;
        if (priority_op() || priority == 1) //if there's an operator with priority, we can evaluate it
        {
            vals.push_back(do_op(variables));
            if (priority) priority--;
        }
        return true;
    }
    if (contains(t0, operations))//if we get a new operation
    {
        //here add special case for -

        //only + or - can be at the start, since we get implicit 0
        if (vals.size() == 0){
            if (t0 != '-' || t0 != '+')
            {
                return false;
            }
            vals.push_back("0");
        }
        if (t0 == '-' && t+1 < terms_list.size())//priority for -
        {
            char next = terms_list.at(t+1).front();
            if (isdigit(next) || next == '$')
            {
                priority++;
            }
        }

        //there can't be more operators than values (operators are binary)
        if (ops.size() > vals.size()) //only 
        {    
            //std::cout << "more ops than vals!!!\n";
            return false;
        } 
        ops.push_back(t0);
        bracks.push_back('o');//pushing info about binary operation into bracks
        return true;
    }
    //std::cout << "general failure\n";
    return false;
}
