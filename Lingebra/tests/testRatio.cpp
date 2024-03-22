#include "../ratio.h"


int main(int argc, char* argv[])
{
    using ifrac = rational<int>;
    //declare rational 0;
    ifrac zero;
    ifrac one{1};
    //rational<int> two_thirds{2,3};
    ifrac num(2,3);

    print("-5 % 2 = {}\n", -5%2);

    print("default const number (0) : {}\n", zero);
    print("one argument (1) :{}\n", one);
    print("num (2/3) :{}\n", num);
    num += 2;
    print("num += 2 (8/3):{}\n", num);
    num -= 4;
    print("num -= 4 (-4/3):{}\n", num);
    
    //pre, post fixes
    print("++num (-1/3):{}\n", ++num);
    print("num++ (-1/3 -> 2/3):{}\n", num++);
    print("num :{}\n", num);
    print("--num (-1/3):{}\n", --num);
    print("num-- (-1/3 -> -4/3):{}\n", num--);
    print("num :{}\n", num);

    ifrac three{6,2};
    print("3 defined as 6/2: {}\n", three);
    ifrac plus_t = three + num;
    print("{} + {} = {}\n", three, num, plus_t);

    print("{} + 1/3 = ", plus_t);
    plus_t += make_rational<int>(1,3);
    print("{}\n", plus_t);

    print("+= 1/7\n");
    auto ir = make_rational<int>(1,7);
    for (; ir < make_rational<int>(6,1); ir += make_rational<int>(1,7)){
        print("{},",ir);
    }
    print("final: {}\n", ir.dbl());

    printf("-= 1/7:\n");
    ir = make_rational<int>(1,7);
    for (; ir > make_rational<int>(-6,1); ir -= make_rational<int>(1,7)){
        print("{}, ",ir);
    }
    print("final: {}\n", ir.dbl());
    
    // *=
    // /=
    //nonmember
}