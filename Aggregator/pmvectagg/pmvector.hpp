#include <iostream>
#include <memory>
#include <vector>
#include <string>

class box 
{
public:
    virtual ~box() = default;
    virtual void print() const = 0;
    virtual int_box& operator+=(const int_box& b) = 0;
    virtual db_box& operator+=(const db_box& b) = 0;
    virtual str_box& operator+=(const str_box& b) = 0;
};

class int_box : public box
{
    int value;
public:
    ~int_box() {std::cout << "int_dtor\n";}
    int_box(): value(0) {}
    int_box(int i): value(i) {}
    int_box(const int_box& b):value(b.value) {}
    bool operator==(const int_box& b) {
        return b.value == value;
    }
    int_box& operator+=(const int_box& b) override {
        this->value += b.value;
        return *this;
    }
    void print() const override { std::cout << value << "\n"; }
};

class db_box : public box
{
    
    double value;
public:
    ~db_box() {std::cout << "db_dtor\n";}
    db_box() : value(0.0) {}
    db_box(const db_box& b):value(b.value) {}
    db_box(double d) : value(d) {}
    bool operator==(const db_box& b) {
        return b.value == value;
    }
    db_box& operator+=(const db_box& b) override {
        this->value += b.value;
        return *this;
    }
    void print() const override {  std::cout << value << "\n"; }
};

class str_box : public box
{
    
    std::string str;
public:
    ~str_box() {std::cout << "str_dtor\n";}
    str_box() : str("") {}
    str_box(const std::string& s) : str(s) {}
    str_box(const str_box& b):str(b.str) {}
    bool operator==(const str_box& b) {
         return b.str == str;
    }
    str_box& operator+=(const str_box& b) override {
        this->str += b.str;
        return *this;
    }
    void print() const override {  std::cout << str << "\n"; }
};

class pmvector 
{
    std::vector<std::unique_ptr<box>> vec;
public:
    void push_back(int i) {
        std::unique_ptr<int_box> I(new int_box(i));
        vec.push_back(std::move(I));
    }
    void push_back(double d) {
        std::unique_ptr<db_box> D(new db_box(d));
        vec.push_back(std::move(D));
    }
    void push_back(const std::string& s) {
        std::unique_ptr<str_box> S(new str_box(s));
        vec.push_back(std::move(S));
    }
    void print(size_t i) {
        vec.at(i)->print();
    }   
    void SUM(const std::vector<int>& pos);
    void MAX(const std::vector<int>& pos);
    void MIN(const std::vector<int>& pos);

    const std::unique_ptr<box> at(size_t i){
        return std::move(vec.at(i));
    }
    void print_all()
    {
        for ( const auto& e : vec )
            e->print();
    }
};

inline void pmvector::SUM(const std::vector<int>& pos)
{
    auto& b = vec.at(pos.at(0));
    for (auto i : pos){
        if (i == 0) continue;
        b += vec.at(i);
    }
}
inline void pmvector::MAX(const std::vector<int>& pos){}
inline void pmvector::MIN(const std::vector<int>& pos){}