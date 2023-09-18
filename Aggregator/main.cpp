//#include "aggregator.hpp"
#include "agg.hpp"

int main(int argc, char** argv)
{
    Aggregator A;
    A.read_data();
    //cout << "data read\n";
    cout.precision(6);
    A.querry();
}