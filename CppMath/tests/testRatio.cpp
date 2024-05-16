#include "../ratio.h"
#include "../exp_num.h"


const auto make_frac = make_rational<int>; 

int main(int argc, char* argv[])
{
    using ifrac = rational<int>;
//    using make_frac = make_rational<int>;
    
#ifdef BASETEST
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

    //ok, this avoids remaking it every time    
    //but:
    //dest 6/1?
    //dest 1/7?
#endif
#ifdef TEST2
    auto a = make_frac(1,2);
    auto b = make_frac(1,7);
    print("a < b: {}:\n", a < b);
    print("a > b: {}:\n", a > b);
    print("a == b: {}:\n", a == b);
    print("a != b: {}:\n", a != b);
     
    
    
    
    print("\n\nmake ir,limit,step:\n");
    auto ir = make_rational<int>(2,7);
    auto limit = make_rational<int>(10,7);
    auto step = make_rational<int>(1,7);
    /*
    print("pause\n");
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
    */

    printf("STARTING += 1/7 in while loop:\n");
    while (ir < limit){
        //print("value:{}, \n",ir);
        ir += step;
    }
    print("as double: {}\n\n", ir.to_dbl());

    /*
    for (; ir < limit; ir += step){//these two get destroyed
        print("value:{}, ",ir);
    }
    */
    ir = make_rational<int>(1,7);
    limit = make_rational<int>(-10,1);
    //step = make_rational<int>(1,7);
    printf("STARTING -= 1/7 in for loop:\n");
    for (; ir > limit; --ir ){
        print("-- {}\n",ir);
    }
    print("as double: {}\n", ir.to_dbl());
    // *=
    print("STARTING *=2:");
    for (auto i = make_frac(1,4); i < 5; i *= 2){
        print("*2 {}\n",i);
    }
    // /=
    print("STARTING /=2:");
    for (auto i = make_frac(8,1); i > step; i /= 2){
        print("/2 {}\n",i);
    }
#endif
    //Test rational<exp_num<int>>
    using exp_frac = rational<exp_num<int>>;

    exp_frac df;
    exp_num<int> exi;

}