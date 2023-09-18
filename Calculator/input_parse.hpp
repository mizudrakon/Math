#include <iostream>
#include <sstream>
#include <vector>

using std::vector, std::string;

//used for debugging
template<typename T>
void print_vec(vector<T> vec){
    for (auto a : vec)
        std::cout << a << ", ";
    std::cout << std::endl;
}

//this is used both by input_parse and expression class in calculator.hpp
template <typename T>
inline int contains(T c, const vector<T>& vec)
{
    for (int i = 0; i < vec.size(); i++){
        if (vec.at(i) == c) return i+1;
    }
    return 0; 
    //the result is the position of element in the vector + 1
    //0 means it's not there
}



/*  Input is mainly wrapper for read_line()
    it reads lines from stdin and breaks them up into types used by expression class
    it does a bit of evaluating too, if it encounters an obvious failure, it inserts failure token into the resulting vector
*/
class Input
{
    Input(){}
public:
    static vector<string> read_line();
};

inline size_t inc(size_t& i, size_t l){ if (++i == l) return --i; return i; }

inline vector<string> Input::read_line(){
    vector<char> opening_brackets = {'(', '[', '{'};
    vector<char> closing_brackets = {')', ']', '}'};
    vector<char> operations = {'+', '-', '*', '/', '%'};
    vector<std::string> parsed_line;
    string line;
    std::getline(std::cin,line);
    
    //std::cout << "\n" << line << std::endl;
    
    string term;
    bool dot;
    bool got_something;
    char last = '~';//helps recognize some syntax errors like * at the start or just 2 numbers with no operation
    // last can be: ~ beginning, + operation, 1 number
    size_t i = 0;
    char c = line.at(i);
    size_t len = line.length();
    //the problem is terms and symbols aren't necessarily divided by spaces or anything specific
    //so it's done char by char
    while (i < line.length())
    {
        dot = false; //there can be only one dot
        term = "";
        got_something = false;//helps evaluate where we end each parsing cycle


        while (i < len && c == ' ')//skip spaces
            if (++i < len)
                c = line.at(i);

        if (contains(c, operations))//got operation
        {
            //last term was operation or nothing
            if (last == '+' || last == '~')//repeated operation
            {
                if (c == '-')
                {
                    parsed_line.push_back("0");//-3 would turn into 0,-,3
                }
                else 
                {
                    //we don't allow repeated operations generally
                    parsed_line.push_back("@@@op");//nonsense op failure token
                    return parsed_line;
                }
            }
            term += c;
            parsed_line.push_back(term);

            last = '+';//marks op
            if (++i < len)
                c = line.at(i);
            continue;
        }
        //bracket
        if (contains(c, opening_brackets) || contains(c, closing_brackets))
        {
            term += c;
            parsed_line.push_back(term);

            if (++i < len)
                c = line.at(i);
            continue;
        }
        //numbers or variable assignment or call
        //digits stack up, allowing only a single dot
        while (i < len && (std::isdigit(c) || (c == '.' && term.size() >= 1 && !dot)) )
        {
            if (last == '1')//last number and we got number again
            {

                parsed_line.push_back("@@@num");
                return parsed_line;
            }
            if (c == '.') dot = true;
            term += c;
            got_something = true;
            if (++i < len)
                c = line.at(i);       
        }    
        if (got_something)//got a number
        {
            parsed_line.push_back(term);

            last = '1';//marks number
            got_something = false;
            continue;
        }
        //variable trouble
        if (i < len && c == '$')//marks call to an existing variable
        { 
            term += c;
            if (++i < len)
                c = line.at(i);
            //next character has to be alphabetical
            //also the last entry can't be numerical - it has to be operation
            if (last == 'n' || !std::isalpha(c))
            {
                parsed_line.push_back("@@@num");
                return parsed_line;
            }
        }
        while (i < len && std::isalpha(c))//got some letters
        {
            c = tolower(c);
            term += c;
            //variable assignmen not at the start!
            if (term.front() != '$' && last != '~')
            {
                parsed_line.push_back("@@@bad_var_assignment~");
                return parsed_line;
            }
            got_something = true;
            if (++i < len)
                c = line.at(i);
        }
        //variable assignment without immediate '='
        if (term.length() > 0 && term.front() != '$' && c != '=')
        {
            parsed_line.push_back("@@@bad_var_assignment=");
            return parsed_line;
        }
        //is $var calling, but contains a number or ends with =
        if (term.length() > 0 && term.front() == '$' && (c == '=' || std::isdigit(c)))
        {
            parsed_line.push_back("@@@bad_var_calling");
            return parsed_line;
        }
        
        if (got_something)
        {
            if (c == '=') {
                term += c;
                last = '+';//if var assignemnt -> op
            } 
            else last = 'n';
            parsed_line.push_back(term);
            got_something = false;
            if (++i < len)
                c = line.at(i);
            continue;
        }
        if (i == len-1)
        {
            parsed_line.push_back("eof");//end token
            return parsed_line;
        }
        else 
        {
            //std::cout << c << " bad symbol \n";
            parsed_line.push_back("@@@bad_symbol");
            return parsed_line;
        }
    }
    parsed_line.push_back("eof");
    return parsed_line;
}