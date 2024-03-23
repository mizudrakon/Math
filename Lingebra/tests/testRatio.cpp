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
    std::cout << "ostream: " << num << '\n';

    num += 2;
    //dest: 2/1
    print("num += 2 (8/3):{}\n", num);

    num -= 4;
    //dest: 4/1

    print("num -= 4 (-4/3):{}\n", num);

    auto add = make_rational(1,2);//this gets destroyed at the end
    num += add;
    num -= add; 


    //pre, post fixes
    print("++num (-1/3):{}\n", ++num);
    print("num++ (-1/3 -> 2/3):{}\n", num++);
    //destructor for copy after prefix
    print("num :{}\n", num);
    print("--num (-1/3):{}\n", --num);
    print("num-- (-1/3 -> -4/3):{}\n", num--);
    //destructor for copy after prefix
    print("num :{}\n", num);

    ifrac three{6,2};
    print("3 defined as 6/2: {}\n", three);
    ifrac plus_t = three + num;
    print("{} + {} = {}\n", three, num, plus_t);

    print("{} + 1/3 = ", plus_t);
    plus_t += make_rational<int>(1,3);
    print("{}\n\n", plus_t);

    if (plus_t > three) print("{} > {}\n\n",plus_t,three);
    else print("{} <= {}\n\n",plus_t,three);
    //1/3 gets destroyed, but it is at the end of use

    auto ir = make_rational<int>(2,7);
    print("pause\n");
    auto limit = make_rational<int>(6,1);
    print("pause\n");
    auto step = make_rational<int>(1,7);
    print("pause\n");
    //ok, this avoids remaking it every time    
    //but:
    //dest 6/1?
    //dest 1/7?

    print("STARTING += 1/7:\n");
    print("value:{}, \n",ir);
    ir += step;
    print("value:{}, \n",ir);
    ir += step;
    print("value:{}, \n",ir);
    ir += step;

    print("value:{}, \n",ir);
    ir += step;
    print("value:{}, \n",ir);
    ir -= static_cast<rational<int>>(4)*step; //this is a problem, 4 needs to be cast...
    print("value:{}, \n",ir);


    print("trying a while loop:\n");
    while (step > ir || ir < limit){
        print("value:{}, \n",ir);
        ir += step;
    }
    /*
    for (; ir < limit; ir += step){//these two get destroyed
        print("value:{}, ",ir);
    }
    */
    print("final: {}\n\n", ir.dbl());
    ir = make_rational<int>(1,7);
    limit = make_rational<int>(-6,1);
    step = make_rational<int>(1,7);
    printf("STARTING -= 1/7:\n");
    for (; ir > limit; ir -= step ){
        print("value:{}, ",ir);
    }
    print("final: {}\n", ir.dbl());
    
    // *=
    // /=
    //nonmember

    //destroying -6, 2, 3, -4/3
    //destroying one, zero
}