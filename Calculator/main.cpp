#include "calculator.hpp"

/*  calculator.hpp contains expression class that evaluates a vector of terms
    static Input class in input_parse.hpp uses read_line() to parse a line from input and feed it to expression
    when evaluating the variables map is used to store and recall variables
*/

using namespace std;
int main(int argc, char** argv)
{
    var_map variables;

    string N;
    std::getline(cin,N);
    
    int n = stoi(N);

    while (n --> 0){
        expression e;
        e.evaluation(variables);
    }
    return 0;
}