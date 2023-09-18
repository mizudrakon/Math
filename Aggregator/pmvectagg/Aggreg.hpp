#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include "pmvector.hpp"

using namespace std;

class Aggregator {
    vector<string> col_names;
    vector<int> types;//might not be important anymore since we're using the pmvector
    vector<unique_ptr<pmvector>> Data;
    size_t N;
public:
    Aggregator(ifstream& infile);//will read the whole input from the INPUT FILE: test.input
    void Querry(ifstream& infile, ofstream& outfile);//will read the querry and write the result to the OUTPUT FILE: test.output
    void readHead(ifstream& infile, vector<string>& target, char sep);
    void readTypes(ifstream& infile);
    void readLine(ifstream& infile);
    //functions:
    /*
    int SUM(vector<int> iv, int key);
    double SUM(vector<double> dv, double key);
    const string& SUM(vector<string> sv, const string& key);
    int MAX(vector<int> iv, int key);
    double MAX(vector<double> dv, double key);
    const string& MAX(vector<string> sv, const string& key);
    int MIN(vector<int> iv, int key);
    double MIN(vector<double> dv, double key);
    const string& MIN(vector<string> sv, const string& key);
    */

};

inline Aggregator::Aggregator(ifstream& infile)
{
    //read col_names
    readHead(infile,col_names,';');
    //init Data while reading the second 
    readTypes(infile);
    //read the N lines in while loop
    cin >> N;
    size_t i = N;
    while (i --> 0)
    {
        readLine(infile);
    }
    //now we have the Data table loaded in, and that's it for init
}


//gets names
inline void Aggregator::readHead(ifstream& infile, vector<string>& target, char sep)
{
    char c;
    string entry = "";
    if (sep == ','){
        while (entry != "SELECT"){
            infile.get(c);
            entry += c;
        }
        entry = "";
    }
    while (!infile.eof())
    {
        infile.get(c);
        if (c == sep || c == '\n'){
            target.push_back(entry);
            if (c == '\n') return;
            entry = "";
        }
        if (c == ' ') continue;
        entry += c;
    }
}

//readTypes only initializes the data table -> vector of various typed columns
inline void Aggregator::readTypes(ifstream& infile)
{
    char c;
    std::string entry = "";
    while (!infile.eof())
    {
        infile.get(c);
        if (c == ';' || c == '\n'){
            if (entry == "int") {
                types.push_back(1);
            }
            else if (entry == "double") {
                types.push_back(2);
            }
            else if (entry == "string") {
                types.push_back(3);
            }
            else { 
                cout << "invalid type entered!\n";
                return;
            }
            unique_ptr<pmvector> V(new pmvector());
            Data.push_back(move(V));
            if (c == '\n') return;
            entry = "";
        }
        if (c == ' ') continue;
        entry += c;
    }
}

inline void Aggregator::readLine(ifstream& infile)
{
    char c;
    string entry;
    infile.get(c);
    //we read only a certain number of entries
    for (int i = 0; i < col_names.size(); i++)
    {
        entry = "";
        while (c != ';' || c != '\n')
        {
            if (c == ' ')
                entry += c;
            infile.get(c);
        }
        //we use the types vector to know what type it is (normally we should check if it's the correct type)
        if (types.at(i) == 1) //int
            Data.at(i)->push_back(stoi(entry));
        else if (types.at(i) == 2) //double
            Data.at(i)->push_back(stod(entry));
        else //string
            Data.at(i)->push_back(entry);
    }
}

//column=pmvector number in the Data vector
inline int getCol(int i, vector<string>& entry_list, vector<string>& col_names){
    string name = "";
    bool read = false;
    for (char c : entry_list.at(i)){
        if (c == '(') {read = true; continue;}
        if (read && c != ')') {
            name += c;
        }
    }
    //we need to find which column it is
    int chosen_column;
    for (int i = 0; i < col_names.size(); i++)
        if (col_names[i] == name) { chosen_column = i; break; }
    return chosen_column; 
}

//read operation from the querry entry
inline const string& getOp(const string& entry){
    string op = "";
    
    for (char c : entry){
        if (c != '(') break;
        if (c != ' ') {op += c;}
    }
    if (op == "") op = "bare";
    return op;
}

inline void Aggregator::Querry(ifstream& infile, ofstream& outfile)
{
    vector<string> querry_head;//just the querry split into single operations (without select and group by)
    vector<int> querry_columns;
    vector<string> querry_ops;//list of operations in order
    //read querry
    readHead(infile,querry_head,',');
    //last entry is group_by col_name, we need to get col_name
    //we need to find which column it is
    int chosen_column = getCol(querry_head.size()-1, querry_head, col_names);
    for (int i = 0; i < querry_head.size()-2; i++)
    {
        querry_columns.push_back(getCol(i,querry_head,col_names));
        querry_ops.push_back(getOp(querry_head.at(i)));
    }
    //print querry_head
    for (string s : querry_head)
    {
        cout << s << ';';
    }
    cout << endl;
    
    //Now we try to answer the querry
    vector<bool> used(N,false);//marks rows already visited in the chosen group_by column
    vector<int> pos;//marks rows that are supposed to be used for calculation

    //for cycle going through the group_by column (chosen column)
    for (auto i = 0; i < N; i++)
    {
        //no positions found yet
        pos.clear();
        if (!used.at(i)){
            //not used -> add to the list 
            pos.push_back(i);
            used.at(i) = true;
        }
        for (auto j = i+1; j < N; j++)
        {
            if (Data.at(chosen_column)->at(j) == Data.at(i)){
                pos.push_back(j);
                used.at(i) = true;
            }
            
        }
        //run the ops and print the results
        //I need to define basic operations on the box objects in pmvector
        for (auto s : querry_ops)
        {

        }
    }
    //Print values of type double with 6 places precision 
    //- use std::fixed (int i; double d; string s; std::cout << i << ';' << std::fixed << d << ';' << s.)
}
