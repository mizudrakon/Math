#include "../exp_num.h"

int main(int args, char* argv[])
{
    using expn = exp_num<int>;
    expn df;
    expn num{2,3};
    print("default constructor: {}, 2^3: {}\n", df, num);

}