#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <map>

using namespace std;

//column is a sort of polymorphic vector that can collect int, double, string
//push_back()
//eq - checks if first parameter is equal to what is on position t
//print(t)
//get_pos(entry) - returns a vector of all positions where we found the entry
//SUM, MAX, MIN - prints results of the given operation on the entries specified by the pos vector

class column
{
    vector<int> int_vec;
    vector<double> db_vec;
    vector<string> str_vec;
public:
    column(){}
    void push_back(int i) {int_vec.push_back(i);}
    void push_back(double d){db_vec.push_back(d);}
    void push_back(const string& s){str_vec.push_back(s);}


    string at(size_t t){
        if (int_vec.size() > 0)
            return to_string(int_vec.at(t));
        if (db_vec.size() > 0)
            return to_string(db_vec.at(t));
        if (str_vec.size() > 0)
            return str_vec.at(t);
        return "";
    }

    bool eq(int i, size_t t){ return int_vec.at(t) == i; }
    bool eq(double d, size_t t){ return db_vec.at(t) == d; }
    bool eq(const string& s, size_t t){ return str_vec.at(t) == s; }
    bool eq(size_t i, size_t j);

    size_t size() {
        if (int_vec.size() > 0) return int_vec.size();
        if (db_vec.size() > 0) return db_vec.size();
        if (str_vec.size() > 0) return str_vec.size();
        return 0;
    }

    void print(size_t t)
    {
        if (int_vec.size() > t) {cout << int_vec.at(t);}
        if (db_vec.size() > t) {cout.precision(6); cout << fixed << db_vec.at(t);}
        if (str_vec.size() > t) {cout << str_vec.at(t);}
    }

    vector<size_t> get_pos(int i);
    vector<size_t> get_pos(double d);
    vector<size_t> get_pos(const string& s);
    
    void SUM(const vector<size_t>& pos);
    void MAX(const vector<size_t>& pos);
    void MIN(const vector<size_t>& pos);

    void print_all()
    {
        if (int_vec.size() > 0){
            for (auto i : int_vec)
                cout << i << ' ';
            cout << '\n';
        }
        else cout << "int_vec empty\n";
        
        if (db_vec.size() > 0){
            cout.precision(6);
            for (auto i : db_vec)
                cout << fixed << i << ' ';
            cout << '\n';
        }
        else cout << "db_vec empty\n";

        if (str_vec.size() > 0){
            for (auto i : str_vec)
                cout << i << ' ';
            cout << '\n';
        }
        else cout << "str_vec empty\n";
    }
};

bool column::eq(size_t i, size_t j)
{

    if (int_vec.size() > 0){
        return int_vec.at(i) == int_vec.at(j);
    }
    else if (db_vec.size() > 0){
        return db_vec.at(i) == db_vec.at(j);
    }
    else if (str_vec.size() > 0){
        return str_vec.at(i) == str_vec.at(j);
    }
    return false;
}
void column::SUM(const vector<size_t>& pos)
{
    if (int_vec.size() > 0)
    {
        int sum = 0;
        for (auto i : pos)
        {
            sum += int_vec.at(i);    
        }
        cout << sum;
        return;
    }

    if (db_vec.size() > 0)
    {
        double sum = 0.0;
        for (auto i : pos)
        {
            //cout << int_vec.at(i);
            sum += db_vec.at(i);    
        }
        cout.precision(6);
        cout << fixed << sum;
        return;
    }

    if (str_vec.size() > 0)
    {
        string sum = "";
        for (auto i : pos)
        {
            sum += str_vec.at(i);    
        }
        cout << sum;
        return;
    }
}

void column::MAX(const vector<size_t>& pos)
{
    
    if (int_vec.size() > 0)
    {   
        int max = int_vec.at(pos.at(0));
        for (auto i : pos){
            if (int_vec.at(i) > max) max = int_vec.at(i);
        }
        cout << max;
        return;
    }
    if (db_vec.size() > 0)
    {
        double max = db_vec.at(pos.at(0));
        for (auto i : pos){
            if (db_vec.at(i) > max) max = db_vec.at(i);
        }
        cout.precision(6);
        cout << fixed << max;
        return;
    }
    if (str_vec.size() > 0)
    {
        string max = str_vec.at(pos.front());
        for (auto i : pos){
            if (str_vec.at(i) > max) max = str_vec.at(i);
        }
        cout << max;
        return;
    }
}
void column::MIN(const vector<size_t>& pos)
{
    
    if (int_vec.size() > 0)
    {   
        int min = int_vec.at(pos.at(0));
        for (auto i : pos){
            if (int_vec.at(i) < min) min = int_vec.at(i);
        }
        cout << min;
        return;
    }
    if (db_vec.size() > 0)
    {
        double min = db_vec.at(pos.at(0));
        for (auto i : pos){
            if (db_vec.at(i) < min) min = db_vec.at(i);
        }
        cout.precision(6);
        cout << fixed << min;
        return;
    }
    if (str_vec.size() > 0)
    {
        string min = str_vec.at(pos.front());
        for (auto i : pos){
            if (str_vec.at(i) < min) min = str_vec.at(i);
        }
        cout << min;
        return;
    }
}

vector<size_t> column::get_pos(int i)
{
    vector<size_t> pos;
    for (size_t j = 0; j < int_vec.size(); j++)
    {
        if (int_vec.at(j) == i) pos.push_back(j);
    }
    return pos;
}

vector<size_t> column::get_pos(double d)
{
    vector<size_t> pos;
    for (size_t j = 0; j < db_vec.size(); j++)
    {
        if (db_vec.at(j) == d){ 
            pos.push_back(j);
        }
    }
    return pos;
}

vector<size_t> column::get_pos(const string& s)
{
    vector<size_t> pos;
    for (size_t j = 0; j < str_vec.size(); j++)
    {
        if (str_vec.at(j) == s) pos.push_back(j);
    }
    return pos;
}