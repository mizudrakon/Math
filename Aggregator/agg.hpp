#include "column.hpp"
#include <map>

using namespace std;

class Aggregator 
{
    vector<string> col_names;
    map<string, size_t> column_names;
    vector<int> types;
    vector<unique_ptr<column>> Data;
    size_t N;
public:
    Aggregator(){}
    void read_data(){
        readHead(col_names, ';');
        readTypes();
        cin >> N;
        size_t i = N;
        while (i --> 0){
            readLine();
        }
    }
    //void Querry();
    string strip(const string& s, char d);
    string readEntry(const string& s, size_t& i, char d);
    void readHead(vector<string>& head, char d);
    void readTypes();
    void readLine();
    size_t data_height(){ return N; }
    void printRow(size_t i);
    size_t getCol(const string& entry, const string& op);
    void querry();
    size_t data_width() { return Data.size(); }
    void printData() {
        for (size_t t = 0; t < data_height(); t++){
            printRow(t);
        }
    }
};

inline string Aggregator::strip(const string& s, char d)
{
    string result = "";
    bool ignore = false;
    if (d == ')') ignore = true; //we want to get what's inside ()
    for (auto a : s)
    {
        if (a == ' ') continue;
        if (ignore && a == '(') {
            ignore = false; 
            continue;
        }
        if (a == d) return result;
        if (!ignore) result += a;
    }
    if (d == '(') return "bare";
    return result;
}

inline string Aggregator::readEntry(const string& s, size_t& i, char d){
    string entry = "";
    for (;i < s.size(); i++){
        if (s.at(i) == ' ') continue;
        if (s.at(i) == d || s.at(i) == '\n'){
            if (s.at(i) == '\n') i = s.size();
            if (s.at(i) == d) i++;
            return entry;
        }
        entry += s.at(i);
    }
    return entry;
}

inline void Aggregator::readHead(vector<string>& head, char d)
{
    string line;
    cin >> line;
    size_t t = 0;
    size_t i = 0;
    while (t < line.size())
    {
        //head.push_back(readEntry(line,t,d));
        column_names.insert(pair<string,size_t>(readEntry(line,t,d), i++));
    }
}

inline void Aggregator::readTypes()
{
    string line;
    cin >> line;
    string entry;
    size_t t = 0;
    while (t < line.size())
    {
        entry = readEntry(line, t, ';');
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
        unique_ptr<column> V(new column());
        Data.push_back(move(V));
    }
}

inline void Aggregator::readLine()
{
    string line;
    cin >> line;
    string entry;
    size_t t = 0;
    size_t i = 0;
    while (t < line.size() && i < types.size())
    //we read only a certain number of entries
    {
        entry = readEntry(line, t, ';');
        //we use the types vector to know what type it is (normally we should check if it's the correct type)
        if (types.at(i) == 1) //int
        {    
            Data.at(i)->push_back(stoi(entry));
            //cout << "got int\n";
        }
        else if (types.at(i) == 2) //double
        {    
            Data.at(i)->push_back(stod(entry));
            //cout << "got db\n";
        }
        else //string
        {    
            Data.at(i)->push_back(entry);
            //cout << "got str\n";
        }
        i++;
    }
}

//just prints a vector
template <typename T>
inline void print_vector(const vector<T>& vec){
    for (size_t i = 0; i < vec.size()-1; i++){
        cout << vec.at(i) << ';';
    }
    cout << vec.at(vec.size()-1) << endl;
}

inline void Aggregator::printRow(size_t i)
{
    for (size_t t = 0; t < Data.size(); t++){
        Data.at(t)->print(i);
        cout << ' ';
    }
    cout << endl;
}

inline size_t Aggregator::getCol(const string& entry, const string& op){
    string name;
    //we need to find which column it is
    if (op == "bare") name = strip(entry,',');
    else name = strip(entry,')');
    size_t col = column_names.at(name);
    /*
    size_t col = col_names.size()*N;
    for (size_t i = 0; i < col_names.size(); i++)
        if (col_names.at(i) == name) { col = i; break; }
    */
    //cout << "!!!name: '" << name << "' col number: " << column << endl;
    return col; 
}

//reads and answers a querry from stdin
inline void Aggregator::querry()
{
    vector<string> querry_head;//just the querry split into single operations (without select and group by)
    vector<size_t> querry_columns;
    vector<string> querry_ops;//list of operations in order
    //read querry
    //cout << "reding the querry\n";
    string s;
    //size_t t = 0;
    while (cin >> s){
        //cout << "read: " << s << endl;
        if (s == "SELECT" || s == "GROUP_BY" || s == "" || s == " ") continue;
        querry_head.push_back(strip(s,','));
        //cout << "calling getCol on '" << s <<"':\n";
        querry_ops.push_back(strip(s,'('));
        querry_columns.push_back(getCol(s,querry_ops.back()));
    }
    //cout << "querry read\n";
    //last entry could be group_by col_name, we need to get col_name
    //we need to find which column it is
    
    size_t chosen_column;
    if (querry_ops.back() == "bare")
    {
        chosen_column = querry_columns.back();
        querry_head.pop_back();
        querry_ops.pop_back();
        querry_columns.pop_back();
    }
    else chosen_column = querry_columns.front();
    //cout << "!!!!GROUP chosen column = " << chosen_column << endl; 
    //cout << "the last entry is " << entry << endl;

    //printing the head of the querry table
    print_vector(querry_head);
    //Now we try to answer the querry
    //vector<bool> used(N,false);//marks rows already visited in the chosen group_by column
    map<string,size_t> used;
    vector<vector<size_t>> positions;//marks rows that are supposed to be used for calculation
    //for cycle going through the group_by column (chosen column)
    for (size_t i = 0; i < N; i++)
    {
        string s = Data.at(chosen_column)->at(i);
        if (used.find(s) == used.end()){
            vector<size_t> v;
            v.push_back(i);
            positions.push_back(v);
            used.insert(pair<string,size_t>(s,i));
        }
        else
        {
            positions.at(used.at(s)).push_back(i);   
        }
        //this should fill in the map, that we won't use anymore


    }
    //cout << "we got a map of positions\n";

    for (auto pos : positions)
    {
        //run the ops and print the results
        //I need to define basic operations on the box objects in pmvector
        //Data.at(2)->print_all();
        for (size_t i = 0; i < querry_columns.size(); i++)
        {
            if (querry_ops.at(i) == "bare"){
                Data.at(querry_columns.at(i))->print(pos.front());
            }
            if (querry_ops.at(i) == "SUM"){
                Data.at(querry_columns.at(i))->SUM(pos);
            }
            if (querry_ops.at(i) == "MAX"){
                Data.at(querry_columns.at(i))->MAX(pos);
            }
            if (querry_ops.at(i) == "MIN"){
                Data.at(querry_columns.at(i))->MIN(pos);
            }
            if (i < querry_columns.size()-1) cout << ';';
        }
        cout << endl;
    }
    //Print values of type double with 6 places precision 
    //- use std::fixed (int i; double d; string s; std::cout << i << ';' << std::fixed << d << ';' << s.)
}