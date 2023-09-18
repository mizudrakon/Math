#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <map>

/*  map of variables, so we can look them up by name
    uses double, whitch could lead to problems, since int variables become double too
    this affects the '/' division operation only, since now there is no actual int division from a recalled variable, sorry
*/

using std::vector, std::string, std::unique_ptr, std::shared_ptr;

class var_map 
{
    
    std::map<std::string,double> variables;
    //I should rework this to use "number boxes" as the following number_vector

public:
    var_map(){}
    bool exist(const string& v) const {
        if (variables.find(v) != variables.end()) return true;
        return false;
    }
    double value(const string& v) const { 
        if (exist(v))
            return variables.at(v); 
            return 0;
        }
    void save_variable(const string& v, double value){
        if (exist(v)) variables.at(v) = value;
        else variables.insert(std::pair<string,double>(v,value));
    }
};

//number box system
//the operations between them are currently done by returning them all as strings and using them as doubles
//should have defined operations between them, except I had trouble with that in the past and need more time to play with it
class box 
{
public:
    virtual ~box() = default;
    virtual void print() const = 0;
    virtual string value() const = 0;
    virtual char type() = 0;
    virtual int prec() = 0;
};

class int_box : public box
{
    long val;
public:
    int_box(): val(0) {}
    int_box(long i,int precision): val(i) {}
    void print() const override { std::cout << val; }
    string value() const override {
        return std::to_string(val);
    }
    char type() {return 'i';}
    int prec() {return 0;}
};

class f_box : public box
{
    float val;
    int p;
public:
    f_box(): val(0.0),p(7) {}
    f_box(float f,int precision): val(f),p(precision) {}
    void print() const override { std::cout << val; }
    string value() const override {
        std::stringstream str;
        str.precision(p);
        str << std::fixed << val;
        return str.str();
    }
    char type() {return 'f';}
    int prec() {return p;}
};

class db_box : public box
{
    double val;
    int p;
public:
    db_box() : val(0.0),p(16) {}
    db_box(double d, int precision) : val(d), p(precision) {}
    void print() const override {  std::cout << val; }
    string value() const override {
        std::stringstream str;
        str.precision(p);
        str << std::fixed << val;
        return str.str();
    }
    char type() {return 'd';}
    int prec() {return p;}
};

class number_vector 
{
    vector<unique_ptr<box>> vec;
public:
    void push_back(long i,int precision) {
        auto I = std::make_unique<int_box>(i,precision);
        //shared_ptr<int_box> I(new int_box(i,precision));
        vec.push_back(std::move(I));
    }
    void push_back(float f,int precision) {
        auto F = std::make_unique<f_box>(f,precision);
        //shared_ptr<f_box> F(new f_box(f,precision));
        vec.push_back(std::move(F));
    }

    void push_back(double d, int precision) {
        auto D = std::make_unique<db_box>(d,precision);
        //shared_ptr<db_box> D(new db_box(d,precision));
        vec.push_back(std::move(D));
    }
    void push_back(const string& s); //defined at the end of file

    void print(size_t i) {
        vec.at(i).get()->print();
    }   
    box* at(size_t i) {
        return vec.at(i).get();
    }
    box* back() {
        return vec.back().get();
    }
    box* front() {
        return vec.front().get();
    }
    void pop_back(){
        vec.pop_back();
    }
    size_t size() {
        return vec.size();
    }
    void print_all() const
    {
        for (auto& e : vec)
        {
            e.get()->print();
            std::cout << ' ';
        }
        std::cout << '\n';

    }
};


//conversion to string keeping precision p, so we can use the number_vector and boxes
template<typename T>
inline string to_str(const T number, const int p)
{
    std::ostringstream out;
    out.precision(p);
    out << std::fixed << number;
    return out.str();
}


//keeps the floating point numbers accurate, and helps with keeping the right type
inline int precision(const string& number)
{
    int prec = 0;
    int len = number.length()-1;
    if (number.find('.') != string::npos){
        int i = 0;
        if (len >= 17) // we can't get better precision with double
        {
            len = 16;
        }

        //wanted to remove excess 0s
        while (number.at(len - i) == '0' || number.at(len - i) == '.') 
        {
            if (number.at(len - i) == '.') { break;}
            i++;
        }
        len -= i;
        prec = len-number.find('.'); 
        
    }
    return prec;
}
//shortcut for using it on actual double
inline int precision(double num)
{
    string s = to_str(num,14);
    int p = precision(s);
    return p;
}

//gets string term and converts it to the appropriate number type, which is inserted to the vector
inline void number_vector::push_back(const string& s)
{
    //we need to find how big of a precision is required to pick the right box
    //float: max 7
    //double: max 16
    int prec = precision(s);
    if (s.find('.') == string::npos) {
        push_back(stol(s),prec);
        return;
    }

    if (prec > 7 || s.length() > 8) //convert to double
    {
        push_back(stod(s),prec);
        return;
    }
    else //convert to float
        push_back(stof(s),prec);
}